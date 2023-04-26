cmake_minimum_required(VERSION 3.23)
project(myserver)
# c++17
set(CMAKE_CXX_STANDARD 17)
# 添加头文件路径 这样在不同目录下的cpp文件不用再去#include "../include/User.h"只需要include "User.h"
include_directories(include)

# 需要编译的可执行文件
#add_executable(myserver src/main.cpp include/User.h src/User.cpp)

# 只要在该目录下的源文件都编译
# 添加源文件目录
aux_source_directory(./src DIR_SRCS) #别名,只会遍历src目录下，无嵌套
add_executable(myserver ${DIR_SRCS} include/timer.h src/timer.cpp include/hightimer.h)
