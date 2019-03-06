#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

struct Widget {
  bool operator<(Widget const& rhs) { return false; }
};

template <typename It>
void work(It begin, It end) {
  while (begin != end) {
    typename std::iterator_traits<It>::value_type currValue = *begin;
//    auto currValue = *begin;
  }
}

void initializing() {
  int x1; // potentially uninitialized
//  auto x2; // error! initializer required
  auto x3 = 0; // fine, x's value is well-defined

  // comparison function for Widgets pointed to by std::unique_ptrs
  auto derefUPLess_11 = [](const std::unique_ptr<Widget>& p1, const std::unique_ptr<Widget>& p2) { return *p1 < *p2; };

  // comparison function for values pointed to by anything pointer-like
  auto derefUPLess_14 = [](const auto& p1, const auto& p2) { return *p1 < *p2; };
}

void autoVsStdFunction() {
  // Perhaps we don’t really need auto to declare a variable that holds a closure, because we can use a std::function object.
  // std::function is a template in the C++11 Standard Library that generalizes the idea of a function pointer.
  // Whereas function pointers can point only to functions, however,
  // std::function objects can refer to any callable object, i.e., to anything that can be invoked like a function.

  std::function<bool(const std::unique_ptr<Widget>&, const std::unique_ptr<Widget>&)> func;
  // Because lambda expressions yield callable objects, closures can be stored in std::function objects.
  // That means we could declare the C++11 version of derefUPLess without using auto as follows:
  std::function<bool(const std::unique_ptr<Widget>&, const std::unique_ptr<Widget>&)> derefUPLess_function =
    [](const std::unique_ptr<Widget>& p1, const std::unique_ptr<Widget>& p2) { return *p1 < *p2; };

  // An auto-declared variable holding a closure has the same type as the closure, and as such it uses only
  // as much memory as the closure requires.

  // The type of a std::function-declared variable holding a closure is an instantiation of the std::function template,
  // and that has a fixed size for any given signature. This size may not be adequate for the closure it’s asked to store,
  // and when that’s the case, the std::function constructor will allocate HEAP MEMORY to store the closure.

  // The result is that the std::function object typically uses more memory than the auto-declared object.
}

void typeShortcuts() {
  std::vector<int> v;
  // The official return type of v.size() is std::vector<int>::size_type

  // On 32-bit Windows:
  //  - unsigned is 32 bits
  //  - std::vector<int>::size_type is 32 bits
  // On 64-bit Windows:
  //  - unsigned is 32 bits
  //  - std::vector<int>::size_type is 64 bits.
  // This means that code that works under 32-bit Windows may behave incorrectly under 64-bit Windows, and when porting
  // your application from 32 to 64 bits.

  // Using auto ensures that you don’t have to:
  auto size = v.size(); // sz's type is std::vector<int>::size_type


  std::unordered_map<std::string, int> m {};
  // the key part of a std::unordered_map is const,
  // so the type of std::pair in the hash table (which is what a std::unordered_map is)
  // isn’t std::pair<std::string, int>, it’s std::pair const std::string, int>.
  for (const std::pair<std::string, int>& pair : m) {
    // But that’s not the type declared for the variable p in the loop above.
    // As a result, compilers will strive to find a way to convert std::pair<const std::string, int> objects
    // (i.e., what’s in the hash table) to std::pair<std::string, int> objects (the declared type for p).
    // They’ll succeed by creating a temporary object of the type that p wants to bind to by copying each object
    // in m, then binding the reference p to that temporary object.
  }

  // Such unintentional type mismatches can be autoed away:
  for (auto& pair: m) {
    // do smth...
  }

  // The code using auto is very attractive characteristic that if you take p’s address, you’re sure to get a pointer
  // to an element within m.
  // In the code not using auto, you’d get a pointer to a temporary object—an object that would be destroyed at the end
  // of the loop iteration.
}

// Things to Remember
// - auto variables must be initialized, are generally immune to type mismatches that can lead to portability or
//   efficiency problems, can ease the process of refactoring, and typically require less typing than variables with
//   explicitly specified types.
// - auto-typed variables are subject to the pitfalls described in Items 2 and 6.