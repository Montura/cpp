#pragma once
#include "Resume.h"

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