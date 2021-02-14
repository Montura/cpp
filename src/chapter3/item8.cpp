#include <iostream>
#include <memory>
#include <mutex>

#include "widget.h"

namespace Item8_NullptrVsZeroOrNULL {
  void f(int) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  };

  void f(bool) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  };

  void f(void*) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  };


  void test_zero() {
    f(0); // calls f(int), not f(void*)

//  f(NULL);
    // msvc: NULL is defined as 0 -> an example is compiled, void f(int) is called for NULL
    // gcc:  NULL is defined __ull -> an example isn't compiled because of ambiguous conversion:
    // (long -> int), (long -> bool), (long -> void*) are considered equally good.

    // There is a contradiction between the meaning of the source code:
    //  - apparent // (“I’m calling f with NULL -  the null pointer”)
    //  - actual   // (“I’m calling f with some kind of integer - not the null pointer”).
    // So, we must avoid overloading on pointer and integral types!
  }

  void test_nullptr() {
    // nullptr’s advantage is that it doesn’t have an integral type.
    // nullptr’s actual type is std::nullptr_t, and, in a wonderfully circular definition,
    // std::nullptr_t is defined to be the type of nullptr

    // The type std::nullptr_t implicitly converts to all raw pointer types, and
    // that’s what makes nullptr act as if it were a pointer of all types.

    // nullptr
    auto x = nullptr;
    f(nullptr);
  }

/// nullptr shines especially brightly when templates enter the picture.
  using MuxGuard = std::lock_guard<std::mutex>;
  std::mutex f1m, f2m, f3m; // mutexes for f1, f2, and f3

  int f1(std::shared_ptr<Widget> spw) { return 0; }; // call these only when
  double f2(std::unique_ptr<Widget> upw) { return  0; }; // the appropriate
  bool f3(Widget* pw) { return false; }; // mutex is locked

  void nullptr_and_templates() {
    {
      MuxGuard g(f1m); // lock mutex for f1
      auto result = f1(0); // pass 0 as null ptr to f1
    } // unlock mutex

    {
      MuxGuard g(f2m); // lock mutex for f2
      auto result = f2(NULL); // pass NULL as null ptr to f2
    } // unlock mutex

    {
      MuxGuard g(f3m); // lock mutex for f3
      auto result = f3(nullptr); // pass nullptr as null ptr to f3
    } //
  }


  template<typename FuncType, typename MutexType, typename PtrType>
  auto lockAndCall_11(FuncType func, MutexType& mutex, PtrType ptr) -> decltype(func(ptr)) {
    std::cout << __PRETTY_FUNCTION__ << std::endl << std::endl;
    MuxGuard g(mutex);
    return func(ptr);
  }

  template <typename FuncType, typename MutexType, typename PtrType>
  auto lockAndCall_14_trailing_type(FuncType func, MutexType& mutex, PtrType ptr) -> decltype(auto) {
    std::cout << __PRETTY_FUNCTION__ << std::endl << std::endl;
    MuxGuard g(mutex);
    return func(ptr);
  }

  template <typename FuncType, typename MutexType, typename PtrType>
  decltype(auto) lockAndCall_14(FuncType func, MutexType& mutex, PtrType ptr) {
    std::cout << __PRETTY_FUNCTION__ << std::endl << std::endl;
    MuxGuard g(mutex);
    return func(ptr);
  }

  void nullptr_as_template_arg() {
    // c++ 11
//  auto result1 = lockAndCall_11(f1, f1m, 0); // error!
    auto result1 = lockAndCall_11(f1, f1m, nullptr); // fine
//  auto result2 = lockAndCall_11(f2, f2m, NULL); // error!
    auto result2 = lockAndCall_11(f2, f2m, nullptr); // fine
    auto result3 = lockAndCall_11(f3, f3m, nullptr); // fine


    // c++ 14 and trailing type
    auto result4 = lockAndCall_14_trailing_type(f1, f1m, nullptr); // error!
    auto result5 = lockAndCall_14_trailing_type(f2, f2m, nullptr); // error!
    auto result6 = lockAndCall_14_trailing_type(f3, f3m, nullptr); // fine

    // c++14 decltype(auto)
    auto result7 = lockAndCall_14(f1, f1m, nullptr); // fine
    auto result8 = lockAndCall_14(f2, f2m, nullptr); // fine
    auto result9 = lockAndCall_14(f3, f3m, nullptr); // fine
  }

  void test() {
    test_zero();
    test_nullptr();
    nullptr_and_templates();
    nullptr_as_template_arg();
  }

  /// Things to Remember:
  ///   - Prefer nullptr to 0 and NULL.
  ///   - Avoid overloading on integral and pointer types.
}