cmake_minimum_required(VERSION 3.12)
project(cpp)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)
include_directories("chapter1/item1_understand_template_type_deduction")
include_directories("chapter1/item2_undrestand_auto_type_deducing")
include_directories("chapter1/item3_understand_decltype")
include_directories("chapter2/")

add_executable(cpp main.cpp)