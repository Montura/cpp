#pragma once

#include <cassert>

namespace Sizeof {
  // Yields the size in bytes of the object representation of:
  // 1. Type
  // 2. The type of expression, if that expression is evaluated

  // - The result of sizeof is always nonzero, even if applied to an empty class type!
  // - Lvalue-to-rvalue, array-to-pointer, or function-to-pointer conversions are not performed.

  // Since c++17
  // - Temporary materialization, however, is (formally) performed for prvalue arguments:
  //    sizeof determines the size of the result object.
  // todo: see Temporary materialization

  // Cannot be used with:
  // - function types
  // - incomplete types
  // - bit-field glvalues
  //
  //  When applied:
  //  - To a reference type, the result is the size of the referenced type.
  //  - To a class type, the result is the size of an object of that class plus any additional padding required to place such object in an array.
  //  - To an expression, sizeof does not evaluate the expression, and even if the expression designates a polymorphic object,
  //  the result is the size of the static type of the expression.

  struct S;

  void test() {
    // Depending on the computer architecture, a byte may consist of 8 or more bits, the
    // exact number being recorded in CHAR_BIT.
    int charBit = CHAR_BIT;
    assert(CHAR_BIT == 8); // for x64, x86

    // // The following sizeof expressions always evaluate to 1:
    assert(sizeof(char) == 1);
    assert(sizeof(signed char) == 1);
    assert(sizeof(unsigned char) == 1);
    assert(sizeof(std::byte) == 1); // since C++17
//    assert(sizeof(char8_t) == 1); // since C++20

//    int x = sizeof(S); // error: incomplete type

    struct S {} s;
    assert(sizeof(S) == 1); // ok
  }
}