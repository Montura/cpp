#pragma once

#include "func.h"

void someFunc(int, double) {};

template<typename T>
void f1(T param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template<typename T>
void f2(T& param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void checkTypes() {
  f1(someFunc); // T - void(*) (int, double) as PTR to func
  f2(someFunc); // T - void(&) (int, double) as REF to func
}