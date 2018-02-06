cmake_minimum_required(VERSION 2.8)
project(ssl)

set(CMAKE_VERBOSE_MAKEFILE ON)

set (Boost_NO_BOOST_CMAKE ON)
find_package(Boost COMPONENTS REQUIRED thread date_time system)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

add_definitions("-std=c++11")
add_executable(ssl main.cpp)

target_link_libraries(ssl ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
