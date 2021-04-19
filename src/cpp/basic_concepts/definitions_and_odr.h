#pragma once

#include "extern_template_foo.h"
template struct Foo<int, char>; // explicit instantiation Foo<int, char>}
void testExternTemplateInstantiation();

namespace DefinitionsAdnODR {


  namespace Definitions {
// A function declaration without a function body
    int f(int); // declares, but doesn't define f

    extern const int a; // declares, but doesn't define a
    extern const int b = 1; // defines b


    struct S {
      int n;               // defines S::n
      static int i;        // declares, but doesn't define S::i
      inline static int x; // defines S::x
    };                     // defines S

    int S::i;              // defines S::i


    namespace cxx17 {
      struct S {
        static constexpr int x = 42; // implicitly inline, defines S::x
      };
      constexpr int S::x; // declares S::x, not a redefinition

      void test() {
        assert(S::x == 42);
      }
    }

    // Declaration of a class name (by forward declaration or by the use of the elaborated type specifier in another declaration)
    struct S; // declares, but doesn't define S
    class Y f(class T p); // declares, but doesn't define Y and T (and also f and p)

    // An opaque declaration of an enumeration
    enum Color : int; // declares, but doesn't define Color

    // A parameter declaration in a function declaration that isn't a definition
    int f(int x); // declares, but doesn't define f and x
    int f(int x) { // defines f and x
      return x + a;
    }

    namespace N {

      template<class T>
      struct A; // declares, but doesn't define A<T>
    }


    template<>
    struct N::A<int>; // declares, but doesn't define A<int>



    typedef S S2; // declares, but doesn't define S2 (S may be incomplete)
    using S2 = S; // declares, but doesn't define S2 (S may be incomplete)

    const int a = 1;

    void test() {
      assert(S::i == 0);
      std::cout << "S::i = " << S::i << "\n";
      f(0);
      testExternTemplateInstantiation();
    }

    // Where necessary, the compiler may implicitly define:
    // 1. Default constructor
    // 2. Copy constructor
    // 3. Move constructor
    // 4. Copy assignment operator
    // 5. Move assignment operator
    // 6. Destructor.
  }


  namespace ODR {
    struct S {
      static const int x = 0; // static data member
      // a definition outside of class is required if it is odr-used

      static const int a = 1; // static data member
      static const int b = 2; // static data member
    };

    const int& f(const int& r) { return r; };

    template<bool condition>
    int test() {
      return condition ? (S::x) // S::x is not odr-used here
                       : f(S::x);  // S::x is odr-used here: a definition is required
    }

    int f() {
      S::x;        // discarded-value expression does not odr-use S::x
      return S::x; // expression where lvalue-to-rvalue conversion applies does not odr-use S::x
    }

    int f(bool x) {
      return x ? S::a : S::b;
      // Notes:

      // 1. x is a part of the subexpression "x" (to the left of ?), which applies lvalue-to-rvalue conversion,
      //    but applying that conversion to x does not yield a constant expression, so x is odr-used

      // 2. S::a and S::b are lvalues, and carry over as "potential results" to the result of the glvalue conditional
      //
      // 3. That result is then subject to lvalue-to-rvalue conversion requested to copy-initialize the return value,
      //    therefore S::a and S::b are not odr-used
    }

    void test() {
      test<true>();
//    test<false>(); // error: ODR is used in test()
      f();
      f(true);
      f(false);
    }


  }

  void test() {
    Definitions::test();
    ODR::test();
  }
}