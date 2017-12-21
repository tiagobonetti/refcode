cmake_minimum_required(VERSION 2.8)
project(boost-po)

set(CMAKE_VERBOSE_MAKEFILE ON)

set (Boost_NO_BOOST_CMAKE ON)
find_package(Boost COMPONENTS REQUIRED program_options)

add_definitions("-std=c++11")
add_executable(boost-po main.cpp)

target_link_libraries(boost-po ${Boost_LIBRARIES})

