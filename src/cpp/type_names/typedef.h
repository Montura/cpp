#pragma once

// typedef - creates an alias that can be used anywhere in place of a (possibly complex) type name.

namespace Typedef {

  // The typedef specifier may not appear in a declaration that does not contain a declarator.
  // Since C++17
  typedef struct X {}; // ill-formed

  namespace Linkage {

  }
  // simple typedef
  typedef unsigned long ulong;

// the following two objects have the same type
  unsigned long l1;
  ulong l2;

// more complicated typedef
  typedef int int_t, *intp_t, (&fp)(int, ulong), arr_t[10];

// the following two objects have the same type
  int a1[10];
  arr_t a2;

// common C idiom to avoid having to write "struct S"
  typedef struct {int a; int b;} S, *pS;

// the following two objects have the same type
  pS ps1;
  S* ps2;

// error: storage-class-specifier cannot appear in a typedef declaration
// typedef static unsigned int uint;

// typedef can be used anywhere in the decl-specifier-seq
  long unsigned typedef int long ullong;
// more conventionally spelled "typedef unsigned long long int ullong;"

// std::add_const, like many other metafunctions, use member typedefs
  template< class T>
  struct add_const {
    typedef const T type;
  };

//  typedef struct Node {
//    struct listNode* next; // declares a new (incomplete) struct type named listNode
//  } listNode; // error: conflicts with the previously declared struct name

  void test() {
    X x;
  }
}