#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <syslog.h>
#include <dirent.h>
#include <ctype.h>

#define SNAME "jdp"
#define NAME  "jav_main_600"
//#define RNAME "/home/taojing/share/JAV/JAV/build/bin/jav_main"
#define RNAME "/home/kang/jav_config/jj-m600.sh"

int judge_pid_exist(int flag, const char* pidName)
{
  DIR *dir;
  struct dirent * next;
  int flags = 0;
  int flagss = 0;

  FILE *status;
  char buffer[1024];
  char name[1024];

  dir = opendir("/proc");
  if (!dir)
  {
    printf("Cannot open /proc\n");
    return 0;
  }


  while ((next = readdir(dir)) != NULL)
  {

    if ((strcmp(next->d_name, "..") == 0) || (strcmp(next->d_name, ".") == 0)) continue;


    if (!isdigit(*next->d_name)) continue;


    sprintf(buffer,"/proc/%s/status",next->d_name);
    if (!(status = fopen(buffer,"r"))) continue;


    if (fgets(buffer,1024,status) == NULL)
    {
      fclose(status);
      continue;
    }
    fclose(status);

    sscanf(buffer,"%*s %s",name);

    if(strstr(name,pidName))
    {
      if(flag == 1)
      {
        flagss = 1;
        closedir(dir);
        return 1;
      }
      else if(flag == 0)
      {
        flagss = 2;
        flags++;
      }
    }
  }
  closedir(dir);
  if(flagss == 1) return 0;
  else if(flagss == 2)
  {
    if(flags > 1) return 0;
    return 1;
  }
}

void init_daemon(void)
{
  int pid = 0;
  int i = 0;

  // [1] fork child process and exit father process
	pid = fork();
	if ( pid < 0 )
  {
	  perror("fork error!");
		exit(1);
  }
  else if( pid > 0 )
  {
    exit(0);
  }

  // [2] create a new session
	setsid();

  for(i = 0;i < NOFILE;i++)
  {
    close(i);
  }

  int ret = chdir(".");

  umask(0);
  while(1)
  {
    if(!judge_pid_exist(1,NAME))
    {
      int ret = system(RNAME);
    }
    sleep(1);
  }
}

int main(void)
{
  /* 屏蔽一些有关控制终端操作的信号
	 * 防止在守护进程没有正常运转起来时，因控制终端受到干扰退出或挂起
	 * */
	signal(SIGINT,  SIG_IGN);// 终端中断
	signal(SIGHUP,  SIG_IGN);// 连接挂断
	signal(SIGQUIT, SIG_IGN);// 终端退出
	signal(SIGPIPE, SIG_IGN);// 向无读进程的管道写数据
	signal(SIGTTOU, SIG_IGN);// 后台程序尝试写操作
	signal(SIGTTIN, SIG_IGN);// 后台程序尝试读操作
	signal(SIGTERM, SIG_IGN);// 终止

  if( judge_pid_exist(0,SNAME) )
  {
    init_daemon();
  }
}
