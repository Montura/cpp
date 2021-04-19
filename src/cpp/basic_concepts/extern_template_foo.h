#pragma once

#include <iostream>

template<class A, class B>
struct Foo {
  void test(A a, B b) {
    std::cout << __PRETTY_FUNCTION__ << '\n';
  }
};

