cmake_minimum_required(VERSION 3.10)
project(dbg-macro)

add_subdirectory(third-party/dbg-macro EXCLUDE_FROM_ALL)

add_executable(dbg-macro main.cpp)
target_compile_features(dbg-macro PRIVATE cxx_std_17)
target_link_libraries(dbg-macro PRIVATE dbg_macro)

