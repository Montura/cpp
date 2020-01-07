#pragma once
#include <cstdint>

template <class T>
struct Node {
  Node* next = nullptr;
  T const data;

  explicit Node(T data) : next(nullptr), data(data) {}

  Node<T>& operator=(const Node<T>* node) {}
};
