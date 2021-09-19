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

  std::cout << "Original list: " << list;
  list.reverse_list();
  std::cout << "Reversed list: " << list;
  list.reverse_list();
  std::cout << "Original list: " << list;

  std::cout << list;
  list.pop_front();
  std::cout << list;
  list.pop_front();
  std::cout << list;

  list.pop_back();
  std::cout << list;
  list.pop_back();
  std::cout << list;
  list.pop_back();
  std::cout << list;
  list.pop_back();
  std::cout << list;
  list.pop_back();
  std::cout << list;
  list.pop_back();
  std::cout << list;

}