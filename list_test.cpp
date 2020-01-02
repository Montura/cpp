#include <iostream>
#include "list.h"

void printList(Node* node) {
  while(node != nullptr) {
    std::cout << "Node: " << node << ", data = " << node->data << std::endl;
    node = node->next;
  }
}

void testNode() {
  Node* head = new Node(1);
  Node* second = new Node(2);
  Node* third = new Node(3);

  head->next = second;
  second->next = third;
}

void testList() {
  List list;

  list.pushFront(7);
  list.pushFront(8);
  list.pushFront(11);
  list.pushFront(13);

  list.pushBack(13);
  list.pushBack(11);
  list.pushBack(8);
  list.pushBack(7);

  Node* head = list.begin();
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