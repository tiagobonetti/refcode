cmake_minimum_required(VERSION 2.8)
project(test_observers)

add_definitions( "-std=c++11" )
include_directories(src)

add_executable(test_observers
    src/main.cpp
    src/token/subject.cpp
    src/auto_token/subject.cpp
    src/auto_token/test.cpp
    )

# target_link_libraries(test_observers boost_system)
