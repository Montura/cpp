cmake_minimum_required(VERSION 3.12)
project(cpp_abi)

set(CMAKE_C_STANDARD 11)

SET(CMAKE_CXX_COMPILER "/usr/bin/gcc")

add_executable(cpp_abi throw.cpp cpp_abi.cpp exception.c)

# Emulating
#  > g++ -c -o throw.o -O0 -ggdb throw.cpp
#  > gcc -c -o main.o -O0 -ggdb main.c
#  > gcc main.o throw.o -o app
#       Undefined symbols for architecture x86_64:
#        "vtable for __cxxabiv1::__class_type_info", referenced from:
#            typeinfo for Exception in throw.cpp.o
#        NOTE: a missing vtable usually means the first non-inline virtual member function has no definition.
#        "___cxa_allocate_exception", referenced from:
#            _seppuku in throw.cpp.o
#        "___cxa_throw", referenced from:
#            _seppuku in throw.cpp.o