#pragma once

#include <cassert>

namespace FundamentalTypes {
  namespace VoidType {
    // Void
    //  A type with an empty set of values.
    //  - Is an incomplete type that cannot be completed (consequently, objects of type void are disallowed).
    //  - There are no arrays of void, nor references to void.
    //  - However, pointers to void and functions returning type void are permitted.

    // std::nullptr_t (defined in header <cstddef>, since c++11; typedef decltype(nullptr) nullptr_t;)
    // - Is the type of the null pointer literal, nullptr.
    // - Is a distinct type that is not itself a pointer type or a pointer to member type.
    // - Its values are null pointer constant (see NULL)
    // - May be implicitly converted to any pointer and pointer to member type.

    void test() {
      assert(sizeof(void*) == sizeof(std::nullptr_t));
    }
  }

 namespace DataModels {
    // 32-bit OS:
    //  1. LP32 or 2/4/4 (int is 16-bit, long and pointer are 32-bit)   - Win16 API
    //  2. ILP32 or 4/4/4 (int, long, and pointer are 32-bit);          - Win32 API Unix and Unix-like systems (Linux, macOS)

    // 64-bit OS:
    //  1. LLP64 or 4/4/8 (int and long are 32-bit, pointer is 64-bit)  - Win64 API
    //  2. LP64 or 4/8/8 (int is 32-bit, long and pointer are 64-bit)   - Unix and Unix-like systems (Linux, macOS)

    void test() {
      assert(sizeof(int) == 4);
      assert(sizeof(long) == 8);
      assert(sizeof(void*) == 8);
    }
  }

  namespace IntegerTypes {
// Modifiers:
//  1. Signedness:
//    - signed - target type will have signed representation (this is the default if omitted)
//    - unsigned - target type will have unsigned representation
//  2. Size:
//    - short - target type will be optimized for space and will have width of at least 16 bits.
//    - long - target type will have width of at least 32 bits.
//    - long long - target type will have width of at least 64 bits (since C++11).

//  NOTE: integer arithmetic is defined differently for the signed and unsigned integer types.
//  See arithmetic operators, in particular integer overflows.

  std::size_t x;
//  Is the unsigned integer type of the result of:
//  - the sizeof operator
//  - the sizeof... operator
//  - the alignof operator (since C++11)

//    See also Fixed width integer types     (since C++11)

    void print_sizeof_s() {
      // todo: add asserts for DataModels for __WIN32__, __APPLE__, __LINUX__

      printf("sizeof(char)                = %lld\n", sizeof(char));
      printf("sizeof(signed char)         = %lld\n", sizeof(signed char));
      printf("sizeof(unsigned char)       = %lld\n", sizeof(unsigned char));
      printf("sizeof(short)               = %lld\n", sizeof(short));
      printf("sizeof(unsigned short)      = %lld\n", sizeof(unsigned short));
      printf("sizeof(int)                 = %lld\n", sizeof(int));
      printf("sizeof(unsigned int)        = %lld\n", sizeof(unsigned int));
      printf("sizeof(long)                = %lld\n", sizeof(long));
      printf("sizeof(unsigned long)       = %lld\n", sizeof(unsigned long));
      printf("sizeof(long long)           = %lld\n", sizeof(long long));
      printf("sizeof(unsigned long long)  = %lld\n", sizeof(unsigned long long));
      printf("sizeof(float)               = %lld\n", sizeof(float));
      printf("sizeof(double)              = %lld\n", sizeof(double));
      printf("sizeof(long double)         = %lld\n", sizeof(long double));
      printf("sizeof(std::size_t)         = %lld\n", sizeof(std::size_t));
    }

    void print_limits() {
      printf("The number of bits in a byte is accessible as %d\n", CHAR_BIT);
      printf("char                 limits: [%d, %d]\n", CHAR_MIN, CHAR_MAX);
      printf("unsigned char        limits: [%d, %d]\n", 0, UCHAR_MAX);
      printf("short                limits: [%d, %d]\n", SHRT_MIN, SCHAR_MAX);
      printf("unsigned short       limits: [%d, %d]\n", 0, USHRT_MAX);
      printf("int                  limits: [%d, %d]\n", INT_MIN, INT_MAX);
      printf("unsigned int         limits: [%d, %u]\n", 0, UINT_MAX);
      printf("long                 limits: [%ld, %ld]\n", LONG_MIN, LONG_MAX);
      printf("unsigned long        limits: [%d, %lu]\n", 0, ULONG_MAX);
      printf("long long            limits: [%lld, %lld]\n", LLONG_MIN, LLONG_MAX);
      printf("unsigned long long   limits: [%d, %llu]\n", 0, ULLONG_MAX);
      printf("float                limits: [%E, %E]\n", -FLT_MAX, FLT_MAX);
      printf("double               limits: [%E, %E]\n", -DBL_MAX, DBL_MAX);
      printf("long double          limits: [%E, %E]\n", -DBL_MAX, DBL_MAX);
    }

    void test() {
      print_sizeof_s();
      print_limits();
    }

  }

  namespace BooleanType {
    // Type, capable of holding one of the two values:
    // - true
    // - false

    void test() {
      // The value of sizeof(bool) is implementation defined and might differ from 1.
      std::size_t bool_size = sizeof(bool);
      std::cout << "Bool size is " << bool_size << '\n';
      assert(bool_size == 1); // !!!!: might differ from 1
    }
  }

  namespace CharacterTypes {

  }

  void test() {
    VoidType::test();
    IntegerTypes::test();
    BooleanType::test();
  }
}