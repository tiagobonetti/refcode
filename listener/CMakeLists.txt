cmake_minimum_required(VERSION 2.8)
project(test_observers)

add_definitions( "-std=c++11" )
include_directories(src)

add_executable(test_observers
    src/main.cpp
    src/value_token/subject.cpp
    src/value_token/test.cpp
    src/auto_token/subject.cpp
    src/auto_token/test.cpp
    src/shared/test.cpp
    src/unique/test.cpp
    )

# target_link_libraries(test_observers boost_system)
