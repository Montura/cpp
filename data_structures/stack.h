#pragma once

#include <array>
#include <iostream>
#include "binary_heap.h"


template <class T>
struct Stat {
  bool operator()(const T& a, const T& b) {
    return a < b;
  }
};

template<class KeyT>
class Stack {
protected:
  KeyT* data;
  int size;
  int top = -1;

public:
  explicit Stack(int size) : size(size) {
    data = new KeyT [size];
  };

  ~Stack() {
    delete[] data;
  }

  bool isEmpty() {
    return top == -1;
  }

  bool isFull() {
    return top == size - 1;
  }

  KeyT getTop() {
    return data[top];
  }

  virtual KeyT pop() {
    if (isEmpty()) {
      std::cout << "Stack is empty!" << std::endl;
      return top;
    }
    return data[top--];
  }

  virtual void push(const KeyT& x) {
    if (isFull()) {
      std::cout << "Stack is full!" << std::endl;
    } else {
      data[++top] = x;
    }
  }
};

template<class KeyT, template <class> class Comparator>
class StackM : public Stack<KeyT> {
  typedef Stack<KeyT> BaseStack;
  typedef Comparator<KeyT> Comp;

  BaseStack stat;

public:
  explicit StackM(int size) : BaseStack(size), stat(size) {}


  KeyT pop() override {
    int x = BaseStack::pop();
    stat.pop();
    return x;
  }

  void push(const KeyT& key) override {
    if (BaseStack::isEmpty()) {
      BaseStack::push(key);
      stat.push(key);
    } else {
      BaseStack::push(key);
      int curr = stat.getTop();
      if (Comp()(key, curr)) {
        stat.push(key);
      } else {
        stat.push(curr);
      }
    }
  }

  KeyT getStat() {
    return stat.getTop();
  }
};