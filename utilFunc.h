#pragma once

#include <iostream>

//template<typename T>
//void f(ParamType param);

// f(expr) // deduce T and ParamType from expr

template<typename T>
void fRef(T & param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

template<typename T>
void fCRef(T const& param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

template<typename T>
void fPointer(T * param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

template <typename T>
void fUniRef(T && param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

template <typename T>
void fVal(T param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

void someFunc(int, double) {};

template<typename T>
class TypeChecher;

template <typename T>
constexpr void checkTypeName() {
  TypeChecher<T> checker;
};

// Things to Remember
// - During template type deduction, arguments that are references are treated as non-references, i.e., their reference-ness is ignored.
// - When deducing types for universal reference parameters, lvalue arguments get special treatment.
// - When deducing types for by-value parameters, const and/or volatile arguments are treated as non-const and non-volatile.
// - During template type deduction, arguments that are array or function names decay to pointers, unless they’re used to initialize references.