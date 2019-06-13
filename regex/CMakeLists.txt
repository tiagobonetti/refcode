cmake_minimum_required(VERSION 3.10)
project(regex)

add_subdirectory(third-party/fmt EXCLUDE_FROM_ALL)

add_executable(regex main.cpp)
target_compile_features(regex PRIVATE cxx_std_17)
target_link_libraries(regex PRIVATE fmt::fmt-header-only)

