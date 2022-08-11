cmake_minimum_required(VERSION 3.20)
project(FastWebServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(FastWebServer main.cpp include/Thread.cpp include/Thread.h)
