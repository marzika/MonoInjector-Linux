cmake_minimum_required(VERSION 3.5.1)
project(MonoLoader)

set(CMAKE_CXX_STANDARD 11)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wno-maybe-uninitialized -Wno-unused-result -O3")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCE_FILES main.cpp)
add_library(MonoLoader SHARED ${SOURCE_FILES})
target_link_libraries(MonoLoader dl)