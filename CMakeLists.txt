cmake_minimum_required(VERSION 3.12)
project(cpp)

set(CMAKE_CXX_STANDARD 17)


if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
set(WARNINGS "-Wno-unused-variable -Wno-unused-local-typedef -Wno-unused-private-field -Wno-format \
              -Wno-unused-function -Wno-unused-value -Wno-constant-conversion -Wno-constant-conversion \
              -Wno-uninitialized -Wno-vexing-parse")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${WARNINGS}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_definitions(-D__PRETTY_FUNCTION__=__FUNCSIG__)
#  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHa ")
  message(Compiler MSVC: ${MSVC_VERSION})
endif()

#add_definitions(-DDANGLING_REFERENCE)

include_directories(cpp include)

add_subdirectory(src/cpp_abi)

add_executable(cpp src/main.cpp
    src/chapter1/item1.cpp
    src/chapter1/item2.cpp
    src/chapter1/item3.cpp
    src/chapter2/item5.cpp
    src/chapter2/item6.cpp
    src/chapter3/item7.cpp
    src/chapter3/item8.cpp
    src/data_structures/test_array.cpp
    src/data_structures/test_bin_heap.cpp
    src/data_structures/test_deque.cpp
    src/data_structures/test_dsu_size_stat.cpp
    src/data_structures/test_graph.cpp
    src/data_structures/test_queue.cpp
    src/data_structures/test_stack.cpp
    src/data_structures/list_test.cpp
    src/sorting/bin_search.cpp
    src/sorting/merge_sort.cpp
    src/sorting/quick_sort.cpp
    src/sorting/sorting.cpp
    src/templates/tag_dispatching.cpp
    src/c/libc.c
    src/c/extern_keyword.c
    src/c/2-2_data_types.c
    src/c/2-3_constants_and_enums.c
    src/c/2-7_type_conversions.c
    src/c/2-8_incement_decrement.c
    src/c/2-9_bitwise_operations.c
    src/c/1-2_variables_and_arithmetic_expressions.c
    src/c/1-5_characters_input_output.c
    src/c/1-7-8-9_characters_array.c
    src/c/2-10_assignment_opertor.c
    src/c/3-3_if_else_bin_search.c
    src/cpp/cppref.cpp
    src/cpp/basic_concepts/extern_template.cpp
    src/templates/meta_functions.cpp
    src/templates/rvalues.cpp
    src/templates/variadic_templates.cpp
    )

#add_executable(cpp interview/taskD.cpp)