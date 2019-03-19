#pragma once
#include "Resume.h"

template <typename T>
void fVal(T param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

// CASE 3. ParamType is Neither a Pointer nor a Reference
// 1) As before, if expr’s type is a reference, ignore the reference part.
// 2) If, after ignoring expr’s reference-ness, expr is const, ignore that, too. If it’s volatile, also ignore that.

void deduceTemplateThree() {
  int x = 27;
  int const cx = x;
  int const& rcx = cx;

  fVal(x);   // T - int, ParamType - int
  fVal(cx);  // T - int, ParamType - int
  fVal(rcx); // T - int, ParamType - int

  // VOLATILE and CONST are ignored only for by-value parameters.
  const char* const ptr = "Fun with pointers";
  fVal(ptr); // T - const char*, ParamType - const char
}