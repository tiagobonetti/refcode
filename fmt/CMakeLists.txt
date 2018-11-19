cmake_minimum_required(VERSION 3.10)
project(fmt-test)

add_subdirectory(third-party/fmt EXCLUDE_FROM_ALL)

add_executable(fmt-test main.cpp)
target_compile_features(fmt-test PRIVATE cxx_std_17)
target_link_libraries(fmt-test PRIVATE fmt::fmt-header-only)
