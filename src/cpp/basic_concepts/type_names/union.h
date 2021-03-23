#pragma once

// UNION - a special class type that can hold only one of its non-static data members at a time.

// - A user-defined type in which all members share the same memory location.
// - At any given time, a union can contain no more than one object from its list of members.
// - No matter how many members a union has, it always uses only enough memory to store the largest member.
// - The other data members are allocated in the same bytes as part of that largest member.
// - UB: to read from the member of the union that wasn't most recently written.
// Since C++14
// - The details of that allocation are implementation-defined but all non-static data members will have the same address.

// Since C++11/C++14
// Special member function - copy/move constructor, copy/move assignment, or destructor

// - Can have member functions (including constructors and destructors), but not virtual functions.
// - Cannot have base classes and cannot be used as a base class.
// - Cannot have non-static data members of reference types.


#include <iostream>
#include <cstdint>
#include <variant>

#include "../std_stuff.h"

namespace Union {
  // Until C++11 (for non-static data member)
  // - Cannot contain a data member with a non-trivial special member function

  // Since C++11 (for non-static data member)

  // - If contains a data member with a non-trivial special member function,
  // that function is deleted by default in the union and needs to be defined explicitly by the programmer.

  // - If a union contains a data member with a non-trivial default constructor,
  // the default constructor of the union is deleted by default unless a variant member of the union has a default member initializer.

  namespace T1 {
    union S {
      std::int32_t n;     // occupies 4 bytes
      std::uint16_t s[2]; // occupies 4 bytes
      std::uint8_t c;     // occupies 1 byte
    };                    // the whole union occupies 4 bytes

    void test() {
      assert(sizeof(S) == sizeof(std::uint32_t));
      S s = {0x12345678}; // initializes the first member, s.n is now the active member
      // at this point, reading from s.s or s.c is undefined behavior
      std::cout << std::hex << "s.n = " << s.n << std::dec << '\n';
      std::cout << "!!! UB: reading inactive member of union: \n\ts.s[0] = " << s.s[0] << "\n\ts.s[1] = " << s.s[1]
                << '\n';

      s.s[0] = 0x0011; // s.s is now the active member
      // at this point, reading from n or c is UB but most compilers define it
      std::cout << "s.c is now " << +s.c << '\n' // 11 or 00, depending on platform
                << "s.n is now " << s.n << '\n'; // 12340011 or 00115678
      std::cout << "!!! UB: reading inactive member of union: \n\ts.s[0] = " << s.s[0] << "\n\ts.s[1] = " << s.s[1]
                << '\n';
    }
  }

  namespace T2 {
    //    If members of a union are classes with user-defined constructors and destructors, to switch the active member,
    // explicit destructor and placement new are generally needed:

    union T {
      std::string str;
      std::vector<int> vec;

      void foo() {}

      ~T() {} // needs to know which member is active, only possible in union-like class
    };          // the whole union occupies max(sizeof(string), sizeof(vector<int>))

    void test() {
      T t = {"Hello, world"};
      std::cout << "t.str = " << t.str << '\n';     // at this point, reading from t.vec is undefined behavior
      t.str.~basic_string();

      new(&t.vec) std::vector<int>; // now, t.vec is the active member of the union
      t.vec.push_back(10);
      std::cout << t.vec.size() << '\n';

      t.vec.~vector();
    }
  }

  namespace MemberLifetime {
    // The lifetime of a union member begins when the member is made active.
    // If another member was active previously, its lifetime ends.

    union A {
      int x;
      int y[4];
    };

    struct B {
      A a;
    };

    union C {
      B b;
      int k;
    };

    int f() {
      C c;               // does not start lifetime of any union member
      c.b.a.y[3] = 4;    // OK: "c.b.a.y[3]", names union members c.b and c.b.a.y;
      // This creates objects to hold union members c.b and c.b.a.y
      return c.b.a.y[3]; // OK: c.b.a.y refers to newly created object
    }

    struct X {
      const int a;
      int b;
    };

    union Y {
      X x;
      float k;
    };

    void g() {
      Y y = { { 1, 2 } }; // OK, y.x is active union member (9.2)
      assert((y.x.a == 1) && (y.x.b == 2));

      int n = y.x.a;
      y.k = 4;   // OK: ends lifetime of y.x, y.k is active member of union
      assert(y.k == 4);
      y.x.b = n; // undefined behavior: y.x.b modified outside its lifetime,
      // "y.x.b" names y.x, but X's default constructor is deleted, so union member y.x's lifetime does not implicitly start
    }

    void test() {
      f();
      g();
    }
  }

  namespace AnonymousUnion {
    // An anonymous union is an unnamed union definition that does not simultaneously define any variables
    // (including objects of the union type, references, or pointers to the union).

    // Restrictions:
    // - Cannot have member functions
    // - Cannot have static data members
    // - All their data members must be public.
    // Since C++11
    // - The only declarations allowed are non-static data members and static_assert declarations.

    void test() {
      union
      {
        int a;
        const char* p;
//        static int x = 2; // error: static data member 'x' not allowed in local class ''
//        void foo() {} // error:  error: functions cannot be declared in an anonymous union
      };
      a = 1;
      p = "Jennifer";
    }
  }

  namespace UnionLikeClasses {
  // S has 2 members:
  //  1. one non-static data member (tag) ( one of three enumerator members (CHAR, INT, DOUBLE))
  //  3. three variant members (c, i, d)
    struct S {
      enum {
        CHAR, INT, DOUBLE
      } tag;
      union {
        char c;
        int i;
        double d;
      };
    };

    void print_s(const S& s) {
      switch (s.tag) {
        case S::CHAR:
          std::cout << s.c << '\n';
          break;
        case S::INT:
          std::cout << s.i << '\n';
          break;
        case S::DOUBLE:
          std::cout << s.d << '\n';
          break;
      }
    }

    void f() {
      S s = {S::CHAR, 'a'};
      print_s(s);
      s.tag = S::INT;
      s.i = 123;
      print_s(s);
    }

    void test() {
      f();
    }
  }

  namespace StdVariant {
    // The class template std::variant represents a type-safe union.

    //  An instance of std::variant at any given time either holds a value of one of its alternative types,
    // or in the case of error - no value (this state is hard to achieve, see valueless_by_exception).

    // Restrictions:
    // - Is not allowed to allocate additional (dynamic) memory.
    // - Is not permitted to hold references, arrays, or the type void.
    // - Is permitted to hold the same type more than once, and to hold differently cv-qualified versions of the same type.
    void test() {
      StdStuff::test_hold_alternative();
      StdStuff::test_string_literals();

      std::variant<int, float> v, w;
      v = 12; // v contains int
      int i = std::get<int>(v);
      assert(i == 12);

      w = std::get<int>(v);
      w = std::get<0>(v); // same effect as the previous line
      w = v; // same effect as the previous line
      assert(std::get<0>(w) == 12);
      assert(std::get<int>(w) == 12);

//  std::get<double>(v); // error: no double in [int, float]
//  std::get<3>(v);      // error: valid index values are 0 and 1

      try {
        std::get<float>(w); // w contains int, not float: will throw
      } catch (const std::bad_variant_access&) {
        std::cerr << "A provocative error with bad_variant_access!\n";
      }

      using namespace std::literals;

      std::variant<std::string> x("abc");
      // converting constructors work when unambiguous
      x = "def"; // converting assignment also works when unambiguous

      std::variant<std::string, void const*> y("abc");

      // casts to void const * when passed a char const *
      assert(std::holds_alternative<void const*>(y)); // succeeds
      y = "xyz"s;
      assert(std::holds_alternative<std::string>(y)); // succeeds
    }
  }

  void test() {
    T1::test();
    T2::test();
    MemberLifetime::test();
    AnonymousUnion::test();
    UnionLikeClasses::test();
    StdVariant::test();
  }
}