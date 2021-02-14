#include <iostream>
#include "data_structures/list.h"

void testList() {
  List<int> list;

  list.push_front(7);
  list.push_front(8);
  list.push_front(11);
  list.push_front(13);

  list.push_back(13);
  list.push_back(11);
  list.push_back(8);
  list.push_back(7);

  list.push_front(102);
  list.push_front(106);
  list.push_front(100);
  list.push_front(101);
  list.push_front(103);
  list.push_front(104);

  std::cout << "Original list: ";
  list.print();
  list.reverse_list();
  std::cout << "Reversed list: ";
  list.print();
  list.reverse_list();
  std::cout << "Original list: ";
  list.print();

  list.print();
  list.pop_front();
  list.print();
  list.pop_front();
  list.print();

  list.pop_back();
  list.print();
  list.pop_back();
  list.print();
  list.pop_back();
  list.print();
  list.pop_back();
  list.print();
  list.pop_back();
  list.print();
  list.pop_back();
  list.print();

}