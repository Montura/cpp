cmake_minimum_required(VERSION 3.12)
project(exception_handling)

set(CMAKE_C_STANDARD 11)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # /DCPPEX (so that a C++ exception is thrown)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHsc ")
  set(SOURCES seh.cpp)
else()
  set(SOURCES gcc_eh.cpp)
endif()

add_executable(exception_handling main.cpp setjmp_longjmp.cpp ${SOURCES})