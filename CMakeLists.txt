cmake_minimum_required(VERSION 3.12)
project(cpp)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

if (MSVC)
  add_definitions(-D__PRETTY_FUNCTION__=__FUNCSIG__)
  message(Compiler MSVC: ${MSVC_VERSION})
endif(MSVC)

include_directories(.)

add_executable(cpp main.cpp array_decay.cpp
               chapter1/item1_understand_template_type_deduction/item1.cpp
               chapter1/item2_undrestand_auto_type_deducing/item2.cpp
               chapter1/item3_understand_decltype/item3.cpp
               chapter2/item5_prefer_auto_to_explicit_type_declarations/item5.cpp
               chapter2/item6_explicitly_typed_initializer_idiom/item6.cpp
               chapter3/item7_creating_objects/item7.cpp
               chapter3/item7_creating_objects/creatingObjects.cpp
               chapter3/item8_prefer_nullptr_to_zero_and_NULL/nullptr.cpp
               chapter3/item8_prefer_nullptr_to_zero_and_NULL/item8.cpp
        array_algos/array_algo.cpp
        data_structures/test_dsu_size_stat.cpp
        data_structures/test_bin_heap.cpp
        data_structures/list_test.cpp
        data_structures/node_test.cpp
        data_structures/test_graph.cpp
        sorting/tools.cpp
        sorting/merge_sort.cpp
        sorting/quick_sort.cpp
        sorting/sorting.cpp
        templates/tag_dispatching.cpp
        data_structures/heap_node.h data_structures/heap_comparators.h)
#add_executable(cpp interview/taskD.cpp)