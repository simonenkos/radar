cmake_minimum_required(VERSION 3.12)
project(radar)

set(CMAKE_CXX_STANDARD 14)

add_executable(radar main.cpp)
target_link_libraries(radar stdc++fs)