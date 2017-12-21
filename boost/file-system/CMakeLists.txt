cmake_minimum_required(VERSION 2.8)
project(boost-fs)

set(CMAKE_VERBOSE_MAKEFILE ON)

set (Boost_NO_BOOST_CMAKE ON)
find_package(Boost COMPONENTS REQUIRED regex system filesystem)

add_definitions("-std=c++11")
add_executable(boost-fs main.cpp)

target_link_libraries(boost-fs
    ${Boost_LIBRARIES}
#    ${Boost_FILESYSTEM_LIBRARY}
#    ${Boost_SYSTEM_LIBRARY}
#    ${Boost_REGEX_LIBRARY}
    )


MESSAGE( STATUS ${Boost_FILESYSTEM_LIBRARY} )
MESSAGE( STATUS ${Boost_SYSTEM_LIBRARY} )
MESSAGE( STATUS ${Boost_REGEX_LIBRARY} )




