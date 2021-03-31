#pragma once

namespace Scope {
  namespace FunctionParameterScope {
  // The potential scope:
  // - Of a function parameter (including parameters of a lambda expression)
  // - Of a function-local predefined variable
  // begins at its point of declaration!

    const int n = 3;

    // scope of global 'n' interrupted, scope of the parameter 'n' begins
    // error: default argument references a parameter
//       int f1(int n, int y = n);

    // OK:
    // - The scope of the function parameter 'n' ends at the end of its function declarator in the array declarator,
    // - Global n is in scope (this declares a pointer to function returning a pointer to an array of 3 int)
    int (* (* f2)(int n))[n];

    // Error: parameter 'n' as array bound
//      auto (* f3)(int n) -> int (*)[n];


    void f(int n = 2) { // scope of 'n' begins
      try {
        ++n;   // 'n' is in scope and refers to the function parameter
        assert(n == 3);
        {
          int n = 2; // scope of the local variable 'n' begins
          // scope of function parameter 'n' interrupted
          ++n; // 'n' refers to the local variable in this block
          assert(n == 3);
          // scope of the local variable 'n' ends
        }
        // scope of function parameter 'n' resumes
        throw std::exception();
      } catch (...) {
        ++n; // n is in scope and refers to the function parameter
        assert(n == 4);
      } // last exception handler ends, scope of function parameter 'n' ends
    }

    int a = n; // OK: global 'n' is in scope

    void test() {
      f();
      assert(a == 3);
    }
  }

  namespace NamespaceScope {
    // A name declared in a namespace becomes visible:
    // - At the locus of its first declaration and is in scope in all namespace definitions for the same namespace that follow.
    // - After using-directive that introduced this name or its entire namespace into another scope, the rest of that scope.
    //
    //  1. The top-level scope of a translation unit is also a namespace - "global namespace scope".
    //  2. Any name declared in the global namespace scope becomes visible at the locus of its first declaration and
    //  remains in scope until the end of the translation unit.
    //
    //  A name declared in an unnamed namespace or in an inline namespace is visible in the enclosing namespace.

    namespace N { // scope of N begins (as a member of global namespace)
      int i; // scope of i begins
      int g(int a) { return a; } // scope of g begins
      int j(); // scope of j begins
      void q(); // scope of q begins

      namespace {
        int x; // scope of x begins
      } // scope of x does not end

      inline namespace inl { // scope of inl begins
        int y; // scope of y begins
      } // scope of y does not end

    } // scope of i,g,j,q,inl,x,y interrupted

    namespace {
      int l = 1; // scope of l begins
    } // scope of l does not end (it's a member of unnamed namespace)

    namespace N { // scope of i,g,j,q,inl,x,y continues
      int g(char a) {  // overloads N::g(int)
        return l + a;  // l from unnamed namespace is in scope
      }
      int l = 2;
//      int i; // error: duplicate definition (i is already in scope)
      int j(); // OK: repeat function declaration is allowed
      int j() { // OK: definition of the earlier-declared N::j()
        return g(i); // calls N::g(int)
      }
//      int q(); // error: q is already in scope with different return type
//      int g(int a) { return a; } // error: ODR
    } // scope of i,g,j,q,inl,x,y interrupted

    void test() {
      using namespace N; // scope of i,g,j,q,inl,x,y resumes
      i = 1; // N::i is in scope
      x = 1; // N::(anonymous)::x is in scope
      y = 1; // N::inl::y is in scope
      inl::y = 2; // N::inl is also in scope
    } // scope of i,g,j,q,inl,x,y interrupted
  }

  namespace ClassScope {
    // Scope of a name declared in a class begins at the point of declaration and includes:
    // - The rest of the class body
    // - All function bodies (even if defined outside the class definition or before the declaration of the name)
    // - Default arguments
    // - Exception specifications
    // - In-class brace-or-equal initializers
    // - And all these things in nested classes, recursively


    namespace A {
      class X {
        int f(int a = n) { // X::n is in scope inside default parameter
          return a * n;   // X::n is in scope inside function body
        }

        using intT = int;

        intT g();

        int i = n * 2;   // X::n is in scope inside initializer

//  int x[n];      // Error: n is not in scope in class body
        static const int n = 1;
        int x[n];      // OK: n is now in scope in class body
      };

//r X::g() {       // Error: r is not in scope outside of out-of-class member function body

      auto X::g() -> intT {   // OK: trailing return type X::r is in scope
        return n;             // X::n is in scope in out-of-class member function body
      }
    }

    //  If a name is used in a class body before it is declared,
    // and another declaration for that name is in scope, the program is ill-formed, no diagnostic required.

    namespace B {
      typedef int c; // ::c
      enum { i = 1 }; // ::i

      struct X {
        // todo: check error?
        char v[i]; //  OK: at this point, i refers to ::i, but there is also X::i

        int f() {
          return sizeof(c); // OK: X::c, not ::c is in scope inside a member function
        }
        char c; // X::c
        enum { i = 2 }; // X::i
      };

      // todo: check error?
      typedef char* T;

      struct Y {
        T a; // Error: at this point, T refers to ::T, but there is also Y::T

        typedef long T;
        T b;
      };
    }

    void test() {
      A::X ax;
      B::X bx;
      assert(std::size(bx.v) == 1);
      assert(bx.f() == sizeof(char));

      B::Y y;
    }

  }

  namespace EnumerationScope {
    // Scoped enumeration:
    //  - Names scope begins at the point of declaration and ends at the end of the enum specifier
    // Unscoped enumerators:
    //  - Names are in scope after the end of the enum specifier

    void test() {
      enum e1_t { // unscoped enumeration
        A,
        B = A * 2
      }; // scope of A and B does not end

      enum class e2_t { // scoped enumeration
        SA = 1,
        SB = SA * 2 // SA is in scope
      }; // scope of SA and SB ends

      e1_t e1 = B; // OK, B is in scope
// e2_t e2 = SB; // Error: SB is not in scope
      e2_t e2 = e2_t::SB; // OK

      assert(e1 == 0);
      assert(e2 == e2_t(2));
    }
  }


  void test() {
    FunctionParameterScope::test();
    ClassScope::test();
    NamespaceScope::test();
    EnumerationScope::test();
  }
}