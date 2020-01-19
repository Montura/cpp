#pragma once

#include <vector>
#include <iostream>

template <class KeyT>
class Deque {
  std::vector<KeyT> data;
  int n = 0, head = 0, tail = 0;

public:
  explicit Deque(int n) : data(n), n(n) {}

  int size() {
    return n;
  }

  bool isFull() const { return head == ((tail + 1) % n); }

  void push_back(const KeyT& k) {
    if (isFull()) {
      std::cout << "Deque is full, overflow!" << std::endl;
      return;
    }
    data[tail] = k;
    tail = (tail + 1) % n;
  }

  KeyT pop_back() {
    if (isEmpty()) {
      std::cout << "Deque is empty, underflow!" << std::endl;
      return -1;
    }
    tail = (tail - 1 + n) % n;
    return data[tail];
  }

  void push_front(const KeyT& k) {
    if (isFull()) {
      std::cout << "Deque is full, overflow!" << std::endl;
      return;
    }

    head = (head - 1 + n) % n;
    data[head] = k;
  }

  KeyT pop_front() {
    if (isEmpty()) {
      std::cout << "Deque is empty, underflow!" << std::endl;
      return -1;
    }
    KeyT value = data[head];
    head = (head + 1) % n;
    return value;
  }

  bool isEmpty() const {
    return (head == tail);
  }

  const KeyT& last() const {
    return data[tail - 1];
  }

  const KeyT& front() const {
    return data[head];
  }
};