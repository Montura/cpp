#pragma once

#include <limits>
#include <iostream>

template <class Key>
struct Min {
  static bool compare(Key& a, Key& b) {
    return a < b;
  }
};

template <class Key>
struct Max {
  static bool compare(Key& a, Key& b) {
    return a > b;
  }
};


template <class Key>
struct std::numeric_limits<Min<Key>> {
  static int max() {
    std::cout << __PRETTY_FUNCTION__  << std::endl;
    return std::numeric_limits<Key>::min();
  }
};

template <class Key>
struct std::numeric_limits<Max<Key>> {
  static int max() {
    std::cout << __PRETTY_FUNCTION__  << std::endl;
    return std::numeric_limits<Key>::max();
  }
};