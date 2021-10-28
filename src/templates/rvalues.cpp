#include <utility>
#include <cassert>
#include "rvalues.h"

template <typename T>
struct Foo {
  int v = 0;
  explicit Foo(int x) : v(x) {
    std::cout << "Ctor" << std::endl;
  }

  Foo(Foo &&t) noexcept: v(t.v)  {
    t.v = 0;
    std::cout << "Move ctor" << std::endl;
  }

  Foo &operator=(const Foo &t) {
    if (this != &t) {
      Foo tmp(t);
      std::swap(*this, tmp);
    }
    std::cout << "Assignment" << std::endl;
    return *this;
  }

  Foo &operator=(Foo &&t) noexcept {
    std::swap(*this, t);
    std::cout << "Assignment" << std::endl;
    return *this;
  }
};

template<typename T>
struct Tricky {
  Tricky() = delete;

  explicit Tricky(int x) {};

  T foo() {};
};

/// --------------- std::declval ---------------
void test_declval::test_tricky() {
//    decltype(Foo<int>().foo()) t; // compile error: default CTOR is deleted
  decltype(((Tricky<int> *) nullptr)->foo()) t; // UB
  decltype(almost_declval<Tricky<int>>().foo()) x; // OK

  static_assert(
      std::is_same_v<
          decltype(almost_declval<Tricky<int>>()),
          decltype(std::declval<Tricky<int>>())
      >
  );
}

/// --------------- std::move ---------------
void test_move::testAlmostMove() {
  Foo<int> t1(1);
  auto t2 = std::move(t1);
  assert(t1.v == 0); // moved-from state
  assert(t2.v == 1);
  auto t3 = almostMove(t2);
  assert(t2.v == 0); // moved-from state
  assert(t3.v == 1);
  static_assert(std::is_same_v<decltype(t2), decltype(t3)>);
}

/// --------------- std::forward ---------------
test_forward::VALUE_CATEGORY test_forward::callme(int && t) {
  std::cout << "arg is rvalue\n";
  return VALUE_CATEGORY::L_VALUE;
}

test_forward::VALUE_CATEGORY test_forward::callme(int & t) {
  std::cout << "arg is lvalue\n";
  return VALUE_CATEGORY::R_VALUE;
}

void test_forward::testAlmostForward() {
  static_assert(std::is_same_v<decltype(std::forward<int&&>(0)), decltype(std::forward<int&&>(0))>);
  assert(trueForward(0) == myForward(0));

  int x = 0;
  static_assert(std::is_same_v<decltype(std::forward<int>(x)), decltype(std::forward<int>(x))>);
  assert(trueForward(x) == myForward(x));

  static_assert(std::is_same_v<decltype(std::forward<int&&>(x + 0)), decltype(std::forward<int&&>(x + 0))>);
  assert(trueForward(x + 0) == myForward(x + 0));
}