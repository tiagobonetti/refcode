cmake_minimum_required(VERSION 3.10)
project(graciously-shutdown)

add_executable(test main.cpp)
target_compile_features(test PRIVATE cxx_std_17)

find_package (Threads)
target_link_libraries(test ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(test PRIVATE third-party/asio/asio/include)
target_compile_definitions(test PRIVATE ASIO_STANDALONE=1)
