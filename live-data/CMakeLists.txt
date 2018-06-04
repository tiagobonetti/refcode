cmake_minimum_required(VERSION 2.8)

project(live-data)
add_definitions("-std=c++11")
include_directories(src)
add_executable(test
    src/observable/token.cpp
    src/observable/value.cpp
    src/main.cpp
    )
