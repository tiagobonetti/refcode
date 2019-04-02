cmake_minimum_required(VERSION 3.10)
project(math)

add_subdirectory(third-party/fmt EXCLUDE_FROM_ALL)

add_executable(math main.cpp)
target_compile_features(math PRIVATE cxx_std_17)
target_link_libraries(math PRIVATE fmt::fmt-header-only)
