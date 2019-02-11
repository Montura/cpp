#pragma once

#include "utilFunc.h"
#include <vector>
// Primary use for decltype is declaring function templates where the function’s return type depends on its parameter types.

// !!!! Auto is used not for type deducing
// Auto is used for indicating TRAILING RETURN TYPE: auto f(params) -> ... {} (return type can use params)

// C++ 11
template <typename Container, typename Idx>
auto updateValue_11(Container& c, Idx i) -> decltype(c[i]) {
  // to do something
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return c[i];
}

// C++ 14 without trailing type -> not quite correct
template <typename Container, typename Idx>
auto updateValue_14(Container& c, Idx i) {
  // to do something
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return c[i];
}

// C++ 14 with trailing type -> as in C++ 11
template <typename Container, typename Idx>
auto updateValueTrailingType_11(Container& c, Idx i) -> decltype(c[i]) {
  // to do something
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return c[i];
}

// C++ 14 with trailing type -> auto specifies that the type is to be deduced,
// and decltype says that decltype rules should be used during the deduction.
template <typename Container, typename Idx>
auto updateValueTrailingType_14(Container& c, Idx i) -> decltype(auto) {
  // to do something
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return c[i];
}

struct Element {};

void decltypeAuto() {
  std::vector<int> v;
//  updateValue(v, 5) = 10; // error for c++14 declaration without trailing type!!!!
  // v[5] returns int&, but auto return type will strip off the reference, thus yielding a return type of int.
  // So we got the assigning 10 to an r-value int

  // It can be fixed by using:
  // -- decltype(c[i]) as in C++ 11
  // -- decltype(auto) only for C++ 14: says that decltype rules should be used during the deduction.
  updateValueTrailingType_11(v, 3) = 7;
  updateValueTrailingType_14(v, 3) = 7;

  Element e;
  Element const& eCRef = e;

  auto myElem1 = eCRef;           // auto type deduction -> myElem1's type is Element
  decltype(auto) myElem2 = eCRef; // decltype type deduction -> myElem2's type is Element const&
//  checkTypeName<decltype(myElem1)>();
//  checkTypeName<decltype(myElem2)>();
}



// So updateValue should accept lvalue and rvalue params by Universal Reference.
// But we don’t know what type of container we’re operating on, and that means we’re equally ignorant of the type of index objects it uses.
// We we need to update the template’s implementation to bring it into accord
// with Item 25’s admonition to apply std::forward to universal references

// C++11
template <typename Container, typename Idx>
auto updateValueRValue_11(Container&& c, Idx i) -> decltype(std::forward<Container>(c)[i]) {
  // to do something
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return c[i];
}

// C++14
template <typename Container, typename Idx>
decltype(auto) updateValueRValue_14(Container&& c, Idx i) {
  // to do something
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  return std::forward<Container>(c)[i];
}

template <typename T>
std::vector<T> make_vector() { return std::vector<T>(10); };

void decltypeForRValueParams() {
  auto vec = make_vector<int>();

  // save the copy of 5th element of vector returned from make_vector()
  auto elemCopy = updateValueRValue_11(make_vector<int>(), 5);
  auto elemCopy1 = updateValueRValue_14(make_vector<int>(), 5);

  // update 6th element of vector x
  updateValueRValue_11(vec, 5) = 6;
  updateValueRValue_14(vec, 5) = 6;
}

// C++ 14
decltype(auto) decltypeValue() {
  int x = 0;
  return x; // decltype (x) is int, func returns int
}

// C++ 14 (UB)
decltype(auto) decltypeRefToLocalVariable() {
  int x = 0;
  return (x); // decltype ((x)) is int&, func returns int&
}

// Things to Remember:
// - decltype almost always yields the type of a variable or expression without any modifications.
// - For lvalue expressions of type T other than names, decltype always reports a type of T&.
// - C++14 supports decltype(auto), which, like auto, deduces a type from its initializer,
//   but it performs the type deduction using the decltype rules.