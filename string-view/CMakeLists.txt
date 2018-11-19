cmake_minimum_required(VERSION 3.10)
project(string-view)
add_executable(string-view main.cpp)
target_compile_features(string-view PRIVATE cxx_std_17)
