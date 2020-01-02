//
// Created by user on 02.01.2020.
//

#include "node.h"

void testNode() {
  auto* head = new Node<int>(1);
  auto* second = new Node<int>(2);
  auto* third = new Node<int>(3);

  head->next = second;
  second->next = third;
}