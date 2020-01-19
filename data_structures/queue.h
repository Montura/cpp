#pragma once

#include "stack.h"

template <class KeyT>
class Queue {
  Stack<KeyT> left, right;
public:
  bool isEmpty() const {
    return left.isEmpty() && right.isEmpty();
  }

  int size() {
    return left.getSize() + right.getSize();
  }

  void enqueue(KeyT x) {
    left.push(x);
  }

  KeyT dequeue() {
    if (isEmpty()) {
      std::cout << "Queue is empty!";
      return 0;
    }

    if (right.isEmpty()) {
      while (!left.isEmpty()) {
        right.push(left.pop());
      }
    }

    return right.pop();
  }

  KeyT front() {
    if (right.isEmpty()) {
      while (!left.isEmpty()) {
        right.push(left.pop());
      }
    }
    return right.getTop();
  }

  void reverseQueueFirstKElements(int k) {
    if (isEmpty() || k > size() || k <= 0) {
      return;
    }

    Stack<int> s;
    for (int i = 0; i < k; i++) {
      s.push(dequeue());
    }

    while (!s.isEmpty()) {
      enqueue(s.pop());
    }

    /* Remove the remaining elements and enqueue them at the end of the Queue*/
    for (int i = 0; i < size() - k; i++) {
      enqueue(dequeue());
    }
  }

  friend std::ostream& operator<<(std::ostream& os, Queue& q){
    while (!q.isEmpty()) {
      os << q.dequeue() << " ";
    }
    return os;
  }
};
