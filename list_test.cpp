#include <iostream>
#include "list.h"

void printList(Node* node) {
  while(node != nullptr) {
    std::cout << "Node: " << node << ", data = " << node->data << std::endl;
    node = node->next;
  }
}

void testList() {
  Node* head = new Node(1);
  Node* second = new Node(2);
  Node* third = new Node(3);

  head->next = second;
  second->next = third;

  List list(head);

  list.push_front(7);
  list.push_front(8);
  list.push_front(11);
  list.push_front(13);

  head->insertAfter(102);
  head->insertAfter(106);
  head->insertAfter(100);
  head->insertAfter(101);
  head->insertAfter(103);
  head->insertAfter(104);

  list.print();
  list.pop_front();
  list.print();
  list.pop_front();
  list.print();

  list.pop_back();
  list.print();
  list.pop_back();
  list.print();

  printList(head);
}