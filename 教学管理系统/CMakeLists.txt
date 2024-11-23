cmake_minimum_required(VERSION 3.24)
project(4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(
        LinkedList.cpp
        LinkedList.h
        main.cpp
        Pair.h main1.cpp)
