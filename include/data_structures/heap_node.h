#pragma once

#include <string>
#include <iostream>
#include <limits>
#include <climits>

class HeapNode {
  int key;
  std::string value;

public:
  explicit HeapNode(int key = 0, const std::string& val = "") {
    this->key = key;
    value = val;
  }

  HeapNode& operator=(const HeapNode& node) {
    if (this != &node) {
      key = node.key;
      value = node.value;
    }
    return *this;
  }

  HeapNode& operator=(int newKey) {
    key = newKey;
    return *this;
  }

  bool operator<(HeapNode const& node) {
    return key < node.key;
  }

  bool operator>(HeapNode const& node) {
    return key > node.key;
  }

  bool operator==(HeapNode const& node) {
    return key == node.key && value == node.value;
  }

  friend std::ostream& operator<<(std::ostream& os, const HeapNode&);
};


std::ostream& operator<<(std::ostream& os, const HeapNode& str) {
  os << "key: " << str.key << ", value: " << str.value;
  return os;
}


template <> struct std::numeric_limits<HeapNode> {
  static int max() {
    std::cout << __PRETTY_FUNCTION__  << std::endl;
    return INT_MAX;
  }

  static int min() {
    std::cout << __PRETTY_FUNCTION__  << std::endl;
    return INT_MIN;
  }
};


