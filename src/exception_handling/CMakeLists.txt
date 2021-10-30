cmake_minimum_required(VERSION 3.12)
project(exception_handling)

set(CMAKE_C_STANDARD 11)

add_executable(exception_handling gcc_eh.cpp)