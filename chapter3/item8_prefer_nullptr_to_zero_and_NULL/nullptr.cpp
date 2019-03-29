#include <iostream>
#include <Widget.h>
#include <memory>
#include <mutex>

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
template <typename T>
using MuxGuard = std::lock_guard<std::unique_lock<T>>;

int f1(std::shared_ptr<Widget> spw); // call these only when
double f2(std::unique_ptr<Widget> upw); // the appropriate
bool f3(Widget* pw); // mutex is locked

template <typename T>
void nullptr_and_templates();

template <>
void nullptr_and_templates<Widget>() {
  std::unique_lock<Widget> f1m, f2m, f3m; // mutexes for f1, f2, and f3
  {
    MuxGuard<Widget> g(f1m); // lock mutex for f1
    auto result = f1(0); // pass 0 as null ptr to f1
  } // unlock mutex

  {
    MuxGuard<Widget> g(f2m); // lock mutex for f2
    auto result = f2(NULL); // pass NULL as null ptr to f2
  } // unlock mutex

  {
    MuxGuard<Widget> g(f3m); // lock mutex for f3
    auto result = f3(nullptr); // pass nullptr as null ptr to f3
  } //


}