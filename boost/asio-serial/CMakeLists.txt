cmake_minimum_required(VERSION 2.8)
project(serial)

set(CMAKE_VERBOSE_MAKEFILE ON)

set (Boost_NO_BOOST_CMAKE ON)
find_package(Boost COMPONENTS REQUIRED thread date_time system)

find_package(Threads REQUIRED)

add_definitions("-std=c++11")
add_executable(serial main.cpp async_serial.cpp buffered_async_serial.cpp)

target_link_libraries(serial ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
