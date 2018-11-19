cmake_minimum_required(VERSION 3.10)
project(empty)
add_executable(empty main.cpp)
target_compile_features(empty PRIVATE cxx_std_17)
