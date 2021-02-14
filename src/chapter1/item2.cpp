#include <iostream>
#include <vector>

#include "type_checker.h"

namespace Item2_AutoTypeDeducing {

  // !!! Only one curious exception, auto type deduction is template type deduction.

  // In Item 1, template type deduction is explained using this general function template

  //template<typename T>
  //void f(ParamType param);
  // and this general call:
  // f(expr); // call f with some expression

  //In the call to f, compilers use expr to deduce types for T and ParamType.

  // When a variable is declared using auto, auto plays the role of T in the template, and
  // the type specifier for the variable acts as ParamType. This is easier to show than to
  // describe, so consider this example:

  template <typename T>      // conceptual template for
  void func_for_x(T param) { // deducing x's type
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  };

  template <typename T>              // conceptual template for
  void func_for_cx(const T param) {  // deducing cx's type
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  };

  template <typename T>               // conceptual template for
  void func_for_rx(const T& param) {  // deducing cx's type
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  };

// To deduce types for x, cx, and rx in these examples,  compilers act as if there were a template for each declaration
// as well as a call to that template with the corresponding initializing expression:
  void conceptualCalls() {
    auto x = 27;        // type specifier - auto
    const auto cx = x;  // type specifier - const auto
    const auto& rx = x; // type specifier - const auto&

    func_for_x(27);  // conceptual call: param's deduced type is x's type
    func_for_cx(cx); // conceptual call" param's deduced type is cx's type
    func_for_rx(rx); // conceptual call" param's deduced type is rx's type
  }

  static void someFunc() {};

  void autoDeducing() {
    auto        x = 27; // CASE 3 (Is neither ref nor ptr), x's type - int
    const auto  cx = x; // CASE 3 (Is neither ref nor ptr), x's type - int
    const auto& rx = x; // CASE 1 (Is a non-universal ref), x's type - int

    auto&& uref1 = x;   // x  - int       and lvalue, uref1's - int&
    auto&& uref2 = cx;  // cx - const int and lvalue, uref2's - const int&
    auto&& uref3 = 27;  // 27 - int       and rvalue, uref3's - int&&

//  checkTypeName<decltype(uref1)>();
//  checkTypeName<decltype(uref2)>();
//  checkTypeName<decltype(uref3)>();

    const char name[] = "R. N. Briggs"; // name's type is const char[13]

    auto  arr1 = name; // arr1 - const char*
    auto& arr2 = name; // arr2 - const char (&)[13]

    auto  func1 = someFunc; // func - void (*)(int, float)
    auto& func2 = someFunc; // func - void (&)(int, float)
  }

// So, auto type deduction works like template type deduction. They’re essentially two sides of the same coin.

  void autoSpecialDeducingRule() {
    //C++ 98
    int x1 = 27;
    int x2(27);
    //C++ 11
    int x3 = {27};
    int x4{27};

    auto ax1 = 27;    // int
    auto ax2(27);     // int
    auto ax3 = {27};  // std::initializer_list<int>
    auto ax4{27};     // int

//  checkTypeName<decltype(ax1)>();
//  checkTypeName<decltype(ax2)>();
//  checkTypeName<decltype(ax3)>();
//  checkTypeName<decltype(ax4)>();
  }

  template <typename T>
  void f(T param) {};

  template <typename T>
  void g(std::initializer_list<T> param) {};

  void autoInitializerListAndFunction() {
    //  auto x5 = {3, 7, 5.0}; // error -> can't deduce type
    auto x = {1, 2, 3}; // std::initializer_list<int>
    f(x); // ok
    //  f({1, 2, 3}); // error -> no matching function for call to 'f(<brace-enclosed initializer list>)'
    g(x); // ok
    g({1, 2, 3}); // ok, T -> int
  };


// C++ 14
// C++14 permits auto to indicate that a function’s return type should be deduced (see Item 3), and
// C++14 lambdas may use auto in parameter declarations. However, these uses of auto employ template type deduction,
// not auto type deduction.

  auto createInitializerList() -> void {
//  return {1, 2, 3}; // er ror: can't deduce type from initializer_list
  }

  void lambda() {
    std::vector<int> v;
//  auto resetV = [&v](const auto& newValue) { v = newValue; };
//  resetV({1, 2, 3}); // error -> can't deduce type for {1, 2, 3}
  }

  void test() {
    autoInitializerListAndFunction();
    conceptualCalls();
    autoDeducing();
    autoSpecialDeducingRule();
  }

  ///  Things to Remember:
  ///  - auto type deduction is usually the same as template type deduction, but auto type deduction assumes that
  ///    a braced initializer represents a std::initializer_list, and template type deduction doesn’t.
  ///  - auto in a function return type or a lambda parameter implies template type deduction, not auto type deduction.
}
