#pragma once

#include <string>
#include <vector>
#include <array>
#include "helper.h"

namespace ReferenceDeclaration {
  namespace ReferenceCollapsing {

    typedef int&  lref;
    typedef int&& rref;

    void test() {
      int n;
      lref&  r1 = n; // type of r1 is int&
      lref&& r2 = n; // type of r2 is int&
      rref&  r3 = n; // type of r3 is int&
      rref&& r4 = 1; // type of r4 is int&&

      std::cout << "type of n is: " << type_name<decltype(n)>() << '\n';
      std::cout << "type of r1 is: " << type_name<decltype(r1)>() << '\n';
      std::cout << "type of r2 is: " << type_name<decltype(r2)>() << '\n';
      std::cout << "type of r3 is: " << type_name<decltype(r3)>() << '\n';
      std::cout << "type of r4 is: " << type_name<decltype(r4)>() << '\n';
    }
  }

  namespace LValueReference {
    // 1. Lvalue references can be used to alias an existing object (optionally with different cv-qualification):
    void t1() {
      std::string s = "Ex";
      std::string& r1 = s;
      const std::string& r2 = s;

      r1 += "ample";            // modifies s
//    r2 += "!";                  // error: cannot modify through reference to const
      assert("Example" == s);
      assert("Example" == r1);
      assert("Example" == r2);
    }

    // 2. They can also be used to implement pass-by-reference semantics in function calls:
    void double_string(std::string& s) {
      s += s; // 's' is the same object as main()'s 'str'
    }

    void t2() {
      std::string str = "Test";
      double_string(str);
      assert("TestTest" == str);
    }


    // 3. When a function's return type is lvalue reference, the function call expression becomes an lvalue expression:
    char& char_number(std::string& s, std::size_t n) {
      return s.at(n); // string::at() returns a reference to char
    }

    void t3() {
      std::string str = "Test";
      char_number(str, 1) = 'a'; // the function call is lvalue, can be assigned to
      assert("Tast" == str);
    }

    void test() {
      t1();
      t2();
      t3();
    }
  }

  namespace RValueReference {
    void t1() {
      std::string s1 = "Test";
//      std::string&& r1 = s1;           // error: can't bind to lvalue

      const std::string& r2 = s1 + s1; // okay: lvalue reference to const extends lifetime
//      r2 += "Test";                    // error: can't modify through reference to const

      std::string&& r3 = s1 + s1;      // okay: rvalue reference extends lifetime
      r3 += "Test";                    // okay: can modify through reference to non-const
      assert("Test" == s1);
      assert("TestTest" == r2);
      assert("TestTestTest" == r3);
    }

    void f(int& x) {
      std::cout << "lvalue reference overload: " <<  __PRETTY_FUNCTION__ << '\n';
    }

    void f(const int& x) {
      std::cout << "lvalue reference to const overload: " <<  __PRETTY_FUNCTION__ << '\n';
    }

    void f(int&& x) {
      std::cout << "rvalue reference overload: " <<  __PRETTY_FUNCTION__ << '\n';
    }

    void t2() {
      int i = 1;
      const int ci = 2;
      f(i);  // calls t1(int&)
      f(ci); // calls t1(const int&)
      f(3);  // calls t1(int&&)
      // would call f(const int&) if t1(int&&) overload wasn't provided
      f(std::move(i)); // calls t1(int&&)

      // rvalue reference variables are lvalues when used in expressions
      int&& x = 1;
      f(x);            // calls t1(int& x)
      f(std::move(x)); // calls t1(int&& x)
    }

    void t3() {
      int i2 = 42;
      int&& rri = std::move(i2); // binds directly to i2

      assert(&i2 == &rri);
      assert(rri == 42);
    }

    void t4() {
      std::vector<int> v { 1, 2, 3, 4, 5 };
      std::vector<int> v2(std::move(v)); // binds an rvalue reference to v
      assert(v.empty());
    }

    void test() {
      t1();
      t2();
      t3();
      t4();
    }
  }

  namespace ForwardingReferences {
    // see "std_forward.h"

    //  1. Function parameter of a function template declared as rvalue reference to cv-unqualified type template parameter
    // of that same function template:
    namespace Test1 {
      template<class T>
      int g(const T&& x); // x is not a forwarding reference: const T is not cv-unqualified

      template<class T>
      int g(T&& x) { return x; }

      template<class T>
      struct A {
        template<class U>
        A(T&& x, U&& y, int* p) {};
        // x is not a forwarding reference: T is not a type template parameter of the constructor,
        // but y is a forwarding reference
      };

      template<class T>
      int t1(T&& x) {                  // x is a forwarding reference
        return g(std::forward<T>(x)); // and so can be forwarded
      }
    }

    // 2. Auto&& except when deduced from a brace-enclosed initializer list:
    namespace Test2 {
      std::array<int, 3> foo() {
        return std::array<int, 3> { 1, 2, 3 };
      }

      template<class T>
      void g(T && vec) {
        std::cout << __PRETTY_FUNCTION__ << '\n';
      }

      void t2() {
        auto&& vec = foo();           // foo() may be lvalue or rvalue, vec is a forwarding reference
        auto i = std::begin(vec); // works either way
        (*i)++;                      // works either way
        g(std::forward<decltype(vec)>(vec)); // forwards, preserving value category

        for (auto&& x: foo()) {
          // x is a forwarding reference; this is the safest way to use range for loops
        }

        auto&& z = { 1, 2, 3 }; // *not* a forwarding reference (special case for initializer lists)
      }
    }

    void test() {
      int i;
      Test1::t1(i); // argument is lvalue, calls t1<int&>(int&), std::forward<int&>(x) is lvalue
      Test1::t1(0); // argument is rvalue, calls t1<int>(int&&), std::forward<int>(x) is rvalue
      Test2::t2();
    }
  }

  namespace DanglingReferences {
    std::string& f() {
      std::string s = "Example";
      return s; // exits the scope of s:
      // its destructor is called and its storage deallocated
    }

    void test() {
      std::string& r = f(); // dangling reference
      std::cout << r;       // undefined behavior: reads from a dangling reference
      assert(r != "Example");
      std::string s = f();  // undefined behavior: copy-initializes from a dangling reference
      std::cout << s;
      assert(s != "Example");
      assert(s.empty());
      std::cout << '\n';
    }
  }

    void test() {
//    int& a[3]; // error
//    int&* p;   // error
//    int&& r;   // error
    ReferenceCollapsing::test();
    LValueReference::test();
    RValueReference::test();
    ForwardingReferences::test();
#if defined(DANGLING_REFERENCE)
    DanglingReferences::test();
#endif
  }
}