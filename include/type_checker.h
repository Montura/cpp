#pragma once

template<typename T>
class TypeChecker;

template <typename T>
void checkTypeName() {
  TypeChecker<T> checker;
};

// - Attempts to instantiate this template will elicit an error message, because there’s no
//   template definition to instantiate. To see the types for x and y, just try to instantiate
// - TypeChecker with their types:
//   TypeChecker<decltype(x)> xType; // elicit errors containing
//   TypeChecker<decltype(y)> yType; // x's and y's types