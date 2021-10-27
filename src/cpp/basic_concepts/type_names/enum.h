#pragma once

// Enumeration
//    Is a distinct type whose value is restricted to a range of values (see below for details), which may include several
//  explicitly named constants ("enumerators").
//    The values of the constants are values of an integral type known as the underlying type of the enumeration.

namespace Enumeration {
  //  Each enumerator becomes a named constant of the enumeration's type, visible in the enclosing scope,
  // and can be used whenever constants are required.

namespace Unscoped {
//  enum Unscoped; // error
//  1) Declares an unscoped enumeration type whose underlying type is not fixed
//  (in this case, the underlying type is an implementation-defined integral type that can represent all enumerator values;
//  this type is not larger than int unless the value of an enumerator cannot fit in an int or unsigned int.
  enum Unscoped {
  };
//  2) Declares an unscoped enumeration type whose underlying type is fixed.
  enum UnscopedI : int {
  };
//  3) Opaque enum declaration for an unscoped enumeration must specify the name and the underlying type.
// It defines the enumeration type but not its enumerators: after this declaration, the type is a complete type and its size is known.
  enum UnscopedI : int;

  void test_1() {
    enum Color {
      red, green, blue
    };

    Color r = red;
    switch (r) {
      case red  :
        std::cout << "red\n";
        break;
      case green:
        std::cout << "green\n";
        break;
      case blue :
        std::cout << "blue\n";
        break;
    }

    // Values of unscoped enumeration type are implicitly-convertible to integral types
    Color col = red;
    int n = blue; // n == 21
    assert(n == 2);
  }

  void test_2() {
    // 1. Each enumerator is associated with a value of the underlying type
    // 2. When initializers are provided in the enumerator-list, the values of enumerators are defined by those initializers.
    // 3. If the first enumerator does not have an initializer, the associated value is zero
    // a = 0, b = 1, c = 10, d = 11, e = 1, f = 2, g = 12

    enum Foo {
      a, b, c = 10, d, e = 1, f, g = f + c
    };
    assert(a == 0);
    assert(b == 1);
    assert(c == 10);
    assert(d == 11);
    assert(e == 1);
    assert(f == 2);
    assert(g == 12);
  }

  void test_3() {
// 1.Values of integer, floating-point, and enumeration types can be converted by static_cast or explicit cast,
//   to any enumeration type.
// 2. If the underlying type is not fixed and the source value is out of range,
//   the result is unspecified (until C++17) undefined (since C++17).
// 3. The value after such conversion may not necessarily equal any of the named enumerators defined for the enumeration.

    enum access_t {
      read = 1, write = 2, exec = 4
    }; // enumerators: 1, 2, 4; range: 0..7
    auto rwe = static_cast<access_t>(7);
    assert((rwe & read) && (rwe & write) && (rwe & exec));

    auto x = static_cast<access_t>(8.0); // undefined behavior since C++17
    auto y = static_cast<access_t>(8); // undefined behavior since C++17

    enum foo {
      a = 0, b = UINT_MAX
    }; // range: [0, UINT_MAX]
    foo f = foo(-1); // undefined behavior since C++17, even if foo's underlying type is unsigned int
  }

  void test_4() {
    // The name of an unscoped enumeration may be omitted:
    //  such declaration only introduces the enumerators into the enclosing scope:
    enum { a, b, c = 0, d = a + 2 }; // defines a = 0, b = 1, c = 0, d = 2
  }

  void test_5() {
    // If an unscoped enumeration is a class member, its enumerators may be accessed using class member access operators . and ->:
    struct X {
      enum direction {
        left = 'l', right = 'r'
      };
    };
    X x;
    X* p = &x;

    int a = X::direction::left; // allowed only in C++11 and later
    int b = X::left;
    int c = x.left;
    int d = p->left;
    assert((a == b) && (b == c) && (c == d));
  }
}


namespace Scoped {
// 1. Declares a scoped enumeration type whose underlying type is int (the keywords class and struct are exactly equivalent)
  enum class ScopedClass;
// 2. Declares a scoped enumeration type whose underlying type is type
  enum class ScopedClass : int;
// 3. Opaque enum declaration for a scoped enumeration whose underlying type is int
  enum class ScopedClass {
    a, b, c, d
  };
// 4. Opaque enum declaration for a scoped enumeration whose underlying type is type
  enum class ScopedClassInt : int {
    a, b, c, d
  };

//  1. Each enumerator becomes a named constant of the enumeration's type, which is contained within the scope of the enumeration,
// and can be accessed using scope resolution operator.
//  2. There are no implicit conversions from the values of a scoped enumerator to integral types,
//  3. Although static_cast may be used to obtain the numeric value of the enumerator

  void test_1() {
    enum class Color {
      red, green = 20, blue
    };
    Color r = Color::blue;
    switch (r) {
      case Color::red  :
        std::cout << "red\n";
        break;
      case Color::green:
        std::cout << "green\n";
        break;
      case Color::blue :
        std::cout << "blue\n";
        break;
    }
//    int n = r; // error: no scoped enum to int conversion
    int n = static_cast<int>(r); // OK, n = 21
  }

//  SCOPED and UNSCOPED enumeration types whose underlying type is fixed can be initialized from an integer without a cast,
// using list initialization, if all of the following is true:
//  1. the initialization is direct-list-initialization
//  2. the initializer list has only a single element
//  3. the enumeration is either scoped or unscoped with underlying type fixed
//  4. the conversion is non-narrowing

  void test_2() {
    enum byte : unsigned char {}; // byte is a new integer type

    byte b { 42 }; // OK as of C++17 (direct-list-initialization)
    byte d = byte { 42 }; // OK as of C++17; same value as b

//    byte b { 1000 }; // error: is narrowing conversion
//    byte e { -1 }; // error
//    byte c = { 42 }; // error: copy-list-initialization

    struct A {
      byte b;
    };
//    A a1 = { { 42 } }; // error (copy-list-initialization of a constructor parameter)
    A a2 = {byte{42}}; // OK as of C++17

    void f(byte);
//    f({42}); // error (copy-list-initialization of a function parameter)

    enum class Handle : std::uint32_t {
      Invalid = 0
    };
    Handle h { 42 }; // OK as of C++17
  }
}

namespace UsingEnumDeclarationCxx20 {
  // using enum nested-name-specifier(optional) name ;		(since C++20)

  enum class fruit { orange, apple };

#if defined(__cpp_using_enum)
  struct S {
    using enum fruit; // OK: introduces orange and apple into S
  };

    void test() {
    S s;
    s.orange;  // OK: names fruit::orange
    S::orange; // OK: names fruit::orange
  }
#endif
}

  void test() {
    Unscoped::test_1();
    Unscoped::test_2();
    Unscoped::test_3();
    Unscoped::test_4();
    Unscoped::test_5();
    Scoped::test_1();
    Scoped::test_2();
  }
}