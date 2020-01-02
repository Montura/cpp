//
// Created by user on 02.01.2020.
//

#ifndef CPP_NODE_H
#define CPP_NODE_H
#include <cstdint>

template <class T>
struct Node {
  Node* next = nullptr;
  T const data;

  explicit Node(T data) : next(nullptr), data(data) {}

  Node<T>& operator=(const Node<T>* node) {}
};

#include <cstdint>

#endif //CPP_NODE_H
