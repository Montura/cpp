#pragma once
#include <string>
#include <cassert>
#include <cmath>

// Identifiers
namespace Identifiers {
  namespace Declarations {
    // 1. Keywords cannot be used for other purposes;
    // 2. Non-keywords is in an attribute-token.
    //    (e.g. [[private]] is a valid attribute) (since C++11)
    // 3. Alternative representations for certain operators cannot be used for other purposes;
    void alternative_operator_representation() {
#if defined(__APPLE__) || defined(__linux__)
      int x = 3, y = 2, z = 0;

      z = x and y;      // x && y
      z and_eq (x and y); // z &= x && y
      z = x or y;       // z = x | y
      z or_eq (x or y); // z |= (x | y);
      z = x xor y;       // z = x ^ y
      z xor_eq (x xor y); // z ^= (x ^ y);

      z = x bitand y;   // x & y;
      z = x bitor y;    // x | y;

      z = compl x;      // z = ~z
      z = not y;        // z = !y;

      z = (x not_eq y); // z = (x != y)
#endif
    }
    // 4. The identifiers with special meaning (final, import and override)
    // 5. The identifiers with a double underscore anywhere are reserved
    // 6, The identifiers that begin with an underscore followed by an uppercase letter are reserved;
    // 7. The identifiers that begin with an underscore are reserved in the global namespace.

  }

  namespace Zombie {
    // Some identifiers only exists in previous standards. In other words, they have been removed.
    // However, they are still reserved for previous standardization in a certain context.

    // #include <memory> (Smart pointer)
    //    std::auto_ptr<int> a;

    // #include <functional> (Function object utility)
    //    std::binary_function<int, int, bool> test;

    // #include <csdtio> (C style I/O(
    //    std::gets();

    // #include <algorithm>
    //    std::random_shuffle, std::shuffle
  }

  namespace InExpressions {
    // An identifier that names:
    //  - variable
    //  - function
    //  - specialization of a concept (since C++20)
    //  - enumerator
    //  can be used as an expression.
    //  "Identifier-expression" -> is the entity named by the identifier.
    void f() {
      float x, &r = x;
      auto f = [=] {
        decltype(x) y1;             // y1 has type float
        decltype((x)) y2 = y1;      // y2 has type float const& because this lambda is not mutable and x is an lvalue
        decltype(r) r1 = y1;        // r1 has type float&
//        decltype((r)) r2 = y2;      // r2 has type float const&
      };
      f();
    }
  }

  namespace Unqualified {
    // The following can be used in expressions in the same role:
    struct A {
      //  1. An overloaded operator name in function notation
      A& operator +(const A& a) { return *this; };
      void* operator new(size_t size) { return new int(size); };

      //  2. A user-defined conversion function name, such as operator bool;
      operator bool() const { // implicit conversion
        return false;
      }
      explicit operator void*() const { // explicit conversion
        return nullptr;
      }

      //  3. The character ~ followed by a class name
      ~A() = default;
    };

    //  4. A user-defined literal operator name, such as operator "" _km;
    // https://en.cppreference.com/w/cpp/language/user_literal
    long double operator "" _w(long double x ) {
      std::cout << "long double operator \"\" _w(long double), arg = " << x << '\n';
      return x;
    }

    std::u16string operator "" _w(const char16_t* s, size_t n) {
      std::cout <<
                "std::string operator \"\" _w(const char16_t*, size_t), args = " << s << ", " << n << '\n';
      return std::u16string(s);
    };

    uint64_t operator "" _w(const char* ptr) {
      std::cout << "unsigned operator \"\" _w(const char* ptr), arg = " << ptr << '\n';
      return reinterpret_cast<uint64_t>(ptr);
    };

    void user_defined_literal_operator() {
      1.2_w;    // calls operator "" _w(1.2L)
      u"one"_w; // calls operator "" _w(u"one", 3)
      12_w;     // calls operator "" _w("12")
//      "two"_w;  // error: no applicable literal operator
    }

    //  5. A template name followed by its argument list
    template <typename T>
    struct MyTemplate;

    template <>
    struct MyTemplate<int> {};

    //  6. The character ~ followed by a decltype specifier, such as ~decltype(str).
    template <typename T>
    void dispose(T t) {
      ~decltype(t)();
    }

    //  7. Together with identifiers they are known as unqualified id-expressions.
  }

  namespace Qualified {
    //  A qualified id-expression is an unqualified id-expression prepended by:
    //  - A scope resolution operator ::
    void f() {
      ::ceil(1);
    }
    //  - A sequence of enumeration (since C++11)
    std::string::const_pointer a;

    //  - Class or namespace names or decltype expressions (since C++11) separated by scope resolution operators.
    template<typename T>
    struct B;

    template<typename T>
    struct X : B<T> { // "B<T>" is dependent on T
      // todo: learn
      typename T::A* pa; // "T::A" is dependent on T // (see below for the meaning of this use of "typename")

      void f(B<T>* pb) {
        static int i = B<T>::i; // "B<T>::i" is dependent on T
        pb->j++; // "pb->j" is dependent on T
      }
    };
  }

  void test_identifiers() {
    Declarations::alternative_operator_representation();
    InExpressions::f();
    Unqualified::user_defined_literal_operator();
  }
}