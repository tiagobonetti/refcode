cmake_minimum_required(VERSION 2.8)
project(test_observers)

add_definitions( "-std=c++11" )

include_directories(src)

add_executable(test_observers
    src/main.cpp
    src/value_token/subject.cpp
    src/value_token/test.cpp
    src/auto_token/token.cpp
    src/auto_token/test.cpp
    src/shared/test.cpp
    src/unique/test.cpp
    src/signals/test.cpp
    )

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
