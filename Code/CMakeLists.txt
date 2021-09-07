cmake_minimum_required(VERSION 2.8)
project(Learn)

set(CMAKE_CXX_STANDARD 11)
# 为了在运行是能正常使用c++11
# 并且让源文件中的c++11特性内容不显示红色
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
# -ptread：链接pthread.h中的函数
# -D_REENTRANT：定义宏_REENTRANT，使得线程调用的标准库函数为线程安全函数版本
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -D_REENTRANT")

add_executable(main news_receiver_brd.cpp)

