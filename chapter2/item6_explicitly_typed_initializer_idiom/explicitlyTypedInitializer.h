#pragma once

#include <bits/unique_ptr.h>
#include <unordered_map>
#include <vector>
#include <functional>
#include <iostream>
#include "Widget.h"

std::vector<bool> features(Widget const& w) {
  return std::vector<bool> (10);
};

void processWidget(Widget const& w, bool isHighPriority) {};

void autoDeduceUndesiredTypes() {
  std::vector<bool> v;
  // std::vector<bool> is specified to represent its bools in packed form, one bit per bool, but:
  // -- std::vector<T>::operator[] returns a T& for every type except bool (C++ forbids references to bits).
  // -- std::vector<bool>::operator[] doesn’t return a reference to an element of the container.
  //    Instead, it returns an std::vector<bool>::reference object (proxy type) that acts like a bool&.

  Widget w;
  bool highPriority = features(w)[5]; // bool (implicit conversion std::vector<bool>::reference to bool)
  processWidget(w, highPriority); // ok

  auto isHighPriority = features(w)[5]; // std::vector<bool>::reference (no implicit conversion)
  processWidget(w, isHighPriority); // undefined behaviour: contains a dangling pointer

  // - features(w) returns a temporary std::vector<bool> object - TEMP.
  // - Operator[] is invoked on TEMP returns the std::vector<bool>::reference object.
  // - It contains a pointer to a word in the data structure holding the bits that are managed by TEMP,
  //   plus the offset into that word corresponding to bit 5.
  // - So IS_HIGH_PRIORITY is a COPY of this std::vector<bool>::reference object!
  // - So, IS_HIGH_PRIORITY, too, contains a pointer to a word in temp, plus the offset corresponding to bit 5.
  // - At the end of the statement, temp is destroyed, because it’s a temporary object. Therefore, IS_HIGH_PRIORITY
  //   contains a dangling pointer, and that’s the cause of the undefined behavior in the call to processWidget:

  // To AVOID:
  // auto someVar = expression of "invisible" proxy class type;
}

// The solution is to force a different type deduction: Explicitly Typed Initializer Idiom.

double calcEpsilon() {}; // return tolerance value

void explicitlyTypedInitializerIdiom() {
  Widget w;
  auto highPriority = static_cast<bool>(features(w)[5]);

  // Applications of the idiom aren’t limited to initializers yielding proxy class types.
  auto ep = static_cast<float>(calcEpsilon()); // impliclitly convert double → float

  // Suppose you need to calculate the index of an element in a container with random access iterators
  // (e.g., a std::vector, std::deque, or std::array), and you’re given a double between 0.0 and 1.0 indicating how far
  // from the beginning of the container the desired element is located. (0.5 would indicate the middle of the container.)
  double pos = 0.7;
  std::vector<int> c;
  int index = static_cast<int>(pos * c.size());
}


// Things to Remember
// - “Invisible” proxy types can cause auto to deduce the “wrong” type for an initializing expression.
// - The explicitly typed initializer idiom forces auto to deduce the type you want it to have.