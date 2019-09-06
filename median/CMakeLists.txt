cmake_minimum_required(VERSION 3.10)
project(median)
add_executable(median main.cpp)
target_compile_features(median PRIVATE cxx_std_17)
