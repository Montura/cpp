#pragma once

#include "Resume.h"

template<typename T>
void f1(T param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template<typename T>
void f2(T& param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void checkTypes() {
  f1(someFunc); // T - void (*)(int, double) as PTR to func
  f2(someFunc); // T - void (&)(int, double) as REF to func
}


// Things to Remember
// - During template type deduction, arguments that are references are treated as non-references,
//   i.e., their reference-ness is ignored.
// - When deducing types for universal reference parameters, lvalue arguments get special treatment.
// - When deducing types for by-value parameters, const and/or volatile arguments are treated as non-const and non-volatile.
// - During template type deduction, arguments that are array or function names decay to pointers,
//   unless they’re used to initialize references.