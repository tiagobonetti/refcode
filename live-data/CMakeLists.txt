cmake_minimum_required(VERSION 2.8)

project(live-data)
add_definitions("-std=c++11")
add_executable(test
    src/main.cpp
    src/token.cpp
    )
