#include <iostream>
#include "list.h"
#include "node.h"

template <class T>
void printList(Node<T>* node) {
  while(node != nullptr) {
    std::cout << "Node: " << node << ", data = " << node->data << std::endl;
    node = node->next;
  }
}

void testList() {
  List<int> list;

  list.pushFront(7);
  list.pushFront(8);
  list.pushFront(11);
  list.pushFront(13);

  list.pushBack(13);
  list.pushBack(11);
  list.pushBack(8);
  list.pushBack(7);

  auto* head = list.begin();
  list.insertAfter(head, 102);
  list.insertAfter(head, 106);
  list.insertAfter(head, 100);
  list.insertAfter(head, 101);
  list.insertAfter(head, 103);
  list.insertAfter(head, 104);

  std::cout << "Original list: ";
  list.print();
  list.reverseList();
  std::cout << "Reversed list: ";
  list.print();
  list.reverseList();
  std::cout << "Original list: ";
  list.print();

  list.print();
  list.popFront();
  list.print();
  list.popFront();
  list.print();

  list.popBack();
  list.print();
  list.popBack();
  list.print();
  list.popBack();
  list.print();
  list.popBack();
  list.print();
  list.popBack();
  list.print();
  list.popBack();
  list.print();

  head = list.begin();
  printList(head);
}