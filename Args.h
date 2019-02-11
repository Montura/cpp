#pragma ocne

#include <array>
#include "utilFunc.h"


// CASE 1. ParamType is a Reference or Pointer, but not a Universal Reference
// 1) If expr’s type is a reference, ignore the reference part.
// 2) Then pattern-match expr’s type against ParamType to determine T.
void deduceTemplatOne() {
  int x = 27;
  int const cx = x;
  int const& rcx = cx;

  fRef(x);   // T - int      , ParamType - int&
  fRef(cx);  // T - int const, ParamType - int const&
  fRef(rcx); // T - int const, ParamType - int const&

  fCRef(x);   // T - int, ParamType - int const&
  fCRef(cx);  // T - int, ParamType - int const&
  fCRef(rcx); // T - int, ParamType - int const&

  int const* px = &x;
  fPointer(&x); // T - int      , ParamType - int*
  fPointer(px); // T - int const, ParamType - int const*

}

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


