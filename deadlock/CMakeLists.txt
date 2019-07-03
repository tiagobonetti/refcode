cmake_minimum_required(VERSION 3.10)
project(deadlock)
add_executable(deadlock main.cpp)
target_compile_features(deadlock PRIVATE cxx_std_17)

find_package (Threads)
target_link_libraries(deadlock ${CMAKE_THREAD_LIBS_INIT})

