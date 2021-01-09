cmake_minimum_required(VERSION 3.12)
project(profiler)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} SHARED profiler.cpp)
