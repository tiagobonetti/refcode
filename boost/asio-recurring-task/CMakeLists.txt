cmake_minimum_required(VERSION 2.8)
project(recurring-task)

set(CMAKE_VERBOSE_MAKEFILE ON)

set (Boost_NO_BOOST_CMAKE ON)
find_package(Boost COMPONENTS REQUIRED system)

add_definitions("-std=c++11")
add_executable(recurring-task main.cpp)
target_link_libraries(recurring-task ${Boost_LIBRARIES})
