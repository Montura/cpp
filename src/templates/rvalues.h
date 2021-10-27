#pragma once
#include <iostream>

/// --------------- std::declval ---------------
namespace test_declval {
// like std::declval
    template<typename T>
    std::add_rvalue_reference_t<T> almost_declval();

    void test_tricky();
}

/// --------------- std::move ---------------
namespace test_move {
    template<typename T>
    decltype(auto) almostMove(T && t) {
        std::cout << "call almost_move\n";
        using rv_ref_t = std::remove_reference_t<T>;
        return static_cast<rv_ref_t&&>(t);
    }

    void testAlmostMove();
}

/// --------------- std::forward ---------------
namespace test_forward {
  template <typename T>
  decltype(auto) almostForward(std::remove_reference_t<T> & arg) {
    return static_cast<T&&>(arg);
  }

  enum VALUE_CATEGORY: int { R_VALUE = 0, L_VALUE = 1 };

  VALUE_CATEGORY callme(int && t);
  VALUE_CATEGORY callme(int & t);

  template <typename T>
  VALUE_CATEGORY trueForward(T && t) {
    return callme(std::forward<T>(t));
  }

  template <typename T>
  VALUE_CATEGORY myForward(T && t) {
    return callme(almostForward<T>(t));
  }

  void testAlmostForward();
}