cmake_minimum_required(VERSION 3.12)
project(exception_handling)

set(CMAKE_C_STANDARD 11)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${WARNINGS}")
  # /DCPPEX (so that a C++ exception is thrown)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHsc")
  add_link_options(/SAFESEH:NO)
  set(SOURCES msvc/seh.cpp msvc/seh_under_the_hood.cpp)
else()
  set(SOURCES gcc/gcc_stack_unwinding.cpp)
endif()

add_executable(exception_handling main.cpp setjmp_longjmp.cpp ${SOURCES})