#pragma once
#include "utilFunc.h"

// CASE 2. ParamType is a Universal Reference
// 1) If expr is an lvalue, both T and ParamType are deduced to be lvalue references.
//    That’s doubly unusual. First, it’s the only situation in template type deduction
//    where T is deduced to be a reference. Second, although ParamType is declared
//    using the syntax for an rvalue reference, its deduced type is an lvalue reference.
// 2) If expr is an rvalue, the “normal” (i.e., Case 1) rules apply.

void deduceTemplateTwo() {
  int x = 27;
  int const cx = x;
  int const& rcx = cx;

  fUniRef(x);   // х - lvalue,    T - int&      , ParamType - int&
  fUniRef(cx);  // cх - lvalue,   T - int const&, ParamType - int const&
  fUniRef(rcx); // rcx - lvalue,  T - int const&, ParamType - int const&
  fUniRef(27);  // 27 - rvalue,   T - int       , ParamType - int &&
}