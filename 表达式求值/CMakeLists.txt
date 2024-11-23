cmake_minimum_required(VERSION 3.24)
project(EX2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(EX2
        LinkedList.cpp
        LinkedList.h
        main.cpp
        Stack.cpp
        Stack.h)
