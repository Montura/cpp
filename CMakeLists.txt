cmake_minimum_required(VERSION 3.12)
project(cpp)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

if (MSVC)
  add_definitions(-D__PRETTY_FUNCTION__=__FUNCSIG__)
  message(Compiler MSVC: ${MSVC_VERSION})
endif(MSVC)

include_directories(cpp include)

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
    )

#add_executable(cpp interview/taskD.cpp)