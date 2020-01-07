#include <data_structures/graph.h>

void item1();
void item2();
void item3();
void item5();
void item6();
void item7();
void item8();

void array_decay_example();
void testList();
void array_rotation();
void test_tag_dispatching();
void test_quick_sort();
void test_merge_sort();

int main() {
  array_decay_example();
  item1();
  item2();
  item3();
  item5();
  item6();
  item7();
  item8();

  testList();

  array_rotation();
  test_tag_dispatching();
  test_quick_sort();
  test_merge_sort();

  test_graph_impl();

  return 0;
}