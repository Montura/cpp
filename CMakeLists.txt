cmake_minimum_required(VERSION 3.12)
project(cpp)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

include_directories(.)
include_directories("chapter1/item1_understand_template_type_deduction")
include_directories("chapter1/item2_undrestand_auto_type_deducing")
include_directories("chapter1/item3_understand_decltype")
include_directories("chapter1/item4_how_to_view_deduced_types")
include_directories("chapter2/item5_prefer_auto_to_explicit_type_declarations")
include_directories("chapter2/item6_use_the_explicitly_typed_initializer_idiom_when_auto_deduces_undesired_types")
include_directories("chapter3/item7_creating_objects")
include_directories("chapter3/item8_prefer_nullptr_to_zero_and_NULL")

add_executable(cpp main.cpp
               chapter3/item7_creating_objects/creatingObjects.cpp array_decay.cpp
               chapter3/item8_prefer_nullptr_to_zero_and_NULL/nullptr.cpp)