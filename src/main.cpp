#include "items_common.h"

extern "C" void test_libc();
void cppref();

void testList();
void array_rotation();
void test_tag_dispatching();
void test_quick_sort();
void test_merge_sort();
void test_graph_impl();
void test_dsu_size_stat();
void test_binary_heap();
void test_stack();
void test_queue();
void test_deque();

int main() {
  // tests
  Item1_TemplateDeduction::test();
  Item1_ArrayArguments::test();
  Item2_AutoTypeDeducing::test();
  Item3_Decltype::test();
  Item5_AutoVsExplicitTypes::test();

  Item6_ExplicitlyTypedIdiom::test();
  Item7_InitializerList::test();
  Item8_NullptrVsZeroOrNULL::test();

//  testList();
//
//  array_rotation();
//  test_tag_dispatching();
//  test_quick_sort();
//  test_merge_sort();
//
//  test_graph_impl();
//  test_binary_heap();
//  test_stack();
//  test_queue();
//  test_deque();

  // test_libc
//  test_libc();
  cppref();
  return 0;
}