cmake_minimum_required(VERSION 3.12)
project(cpp)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

if (MSVC)
  add_definitions(-D__PRETTY_FUNCTION__=__FUNCSIG__)
  message(Compiler MSVC: ${MSVC_VERSION})
endif(MSVC)

include_directories(.)

add_subdirectory(miniprofiler)

add_executable(cpp main.cpp
    chapter1/item1.cpp
    chapter1/item2.cpp
    chapter1/item3.cpp
    chapter2/item5.cpp
    chapter2/item6.cpp
    chapter3/item7.cpp
    chapter3/item8.cpp
    data_structures/test_array.cpp
    data_structures/test_bin_heap.cpp
    data_structures/test_deque.cpp
    data_structures/test_dsu_size_stat.cpp
    data_structures/test_graph.cpp
    data_structures/test_queue.cpp
    data_structures/test_stack.cpp
    data_structures/list_test.cpp
    sorting/bin_search.cpp
    sorting/merge_sort.cpp
    sorting/quick_sort.cpp
    sorting/sorting.cpp
    templates/tag_dispatching.cpp
    c/libc.c
    c/extern_keyword.c
    c/data_types.c)

target_link_libraries(cpp profiler)
#add_executable(cpp interview/taskD.cpp)