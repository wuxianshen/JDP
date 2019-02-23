# *  @Copyright (c) tao.jing
# *
# *

cmake_minimum_required(VERSION 2.8)
project(jdp)

if (CMAKE_SYSTEM_NAME MATCHES Linux)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g -O0")
endif ()

FILE(GLOB SOURCE_FILES *.h *.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
