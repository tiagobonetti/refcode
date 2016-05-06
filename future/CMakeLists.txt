set(CMAKE_C_COMPILER "clang-3.6")
set(CMAKE_CXX_COMPILER "clang++-3.6")
cmake_minimum_required(VERSION 2.8)

project(test)

add_definitions( "-std=c++11 -pthread" )
add_executable(test future.cpp)


