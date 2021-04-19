#pragma once

namespace Scope {
  // 1. Each name that appears in a C++ program is only valid in some possibly discontiguous portion of the source code
  // called its scope.
  // 2. Within a scope, unqualified name lookup can be used to associate the name with its declaration.
  // todo: unqualified name lookup (https://en.cppreference.com/w/cpp/language/unqualified_lookup)

  namespace BlockScope {
    namespace nested_scope {
      // The potential scope of a variable introduced by a declaration in a block:
      // - begins at the point of declaration
      // - ends at the end of the block.
      void test() {
        int a = 0; // scope of the first 'a' begins
        assert(a == 0);
        ++a; // the name 'a' is in scope and refers to the first 'a'
        assert(a == 1);
        {
          int a = 1; // scope of the second 'a' begins
          assert(a == 1);
          // scope of the first 'a' is interrupted
          a = 42;    // 'a' is in scope and refers to the second 'a'
          assert(a == 42);
        } // block ends, scope of the second 'a' ends
        assert(a == 1);
        //             scope of the first 'a' resumes
      } // block ends, scope of the first 'a' ends
//    int b = a; // Error: name 'a' is not in scope
    }

    namespace exception_handler {
      // The potential scope of a name declared in an exception handler:
      // - begins at the point of declaration
      // - ends when the exception handler ends
      // - is not in scope in another exception handler or in the enclosing block
      void f() {}

      void test() {
        try {
          f();
        } catch (const std::runtime_error& re) { // scope of re begins
          int n = 1; // scope of n begins
          std::cout << re.what(); // re is in scope
        } // scope of re ends, scope of n ends
        catch (std::exception& e) {
//        std::cout << re.what(); // error: re is not in scope
//        ++n; // error: n is not in scope
        }
      }
    }

    namespace init_statement_scope {
      struct Base {
        virtual ~Base() = default;
      };

      struct Derived : Base {
        void f() {};
      };

      void test() {
        Base* bp = new Derived;
        if (Derived* dp = dynamic_cast<Derived*>(bp)) {
          dp->f(); // dp is in scope
        } // scope of dp ends

        for(int n = 0; // scope of n begins
            n < 10;    // n is in scope
            ++n)       // n is in scope
        {
          std::cout << n << ' '; // n is in scope
        } // scope of n ends
        std::cout << '\n';
      }
    }


    void test() {
      nested_scope::test();
      exception_handler::test();
    }
  }

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

  namespace TemplateScope {
    template <class T, class U = T>
    struct Array {};

    // The potential scope of a template parameter name:
    // - Begins immediately at the point of declaration
    // - Continues to the end of the smallest template declaration in which it was introduced.
    //
    // Template parameter can be used:
    // - In the declarations of subsequent template parameters
    // - In the specifications of base classes

    // !!! Can't be used in the declarations of the preceding template parameters.

    namespace  TemplateParameter {
      template<typename T, // scope of T begins
        const T* p,       // T can be used for a non-type parameter
        class U = T // T can be used for a default type
      >
      class X : public Array<T> // T can be used in base class name
      {
        // T can be used inside the body as well
      }; // scopes of T and U end, scope of X continues

      int arr[1];
      X<int, arr> x;
    }

    namespace TemplateTemplateParameter {
      // Template template parameter:
      // - Is the smallest template parameter list in which that name appears
      template<
          // template template parameter
          template<
          typename Y,     // scope of Y begins
          typename G = Y // Y is in scope
      > // scopes of Y and G end
          class T,
//    typename U = Y // Error: Y is not in scope
          typename U
      >
      class Z {
      }; // scopes of T and U end

      Z<Array, int> z;

    // Similar to other nested scopes,
    // the name of a template parameter hides the same name from the outer scope for the duration of its own:
      typedef int N;

      template <N X,  // non-type parameter of type int
          typename N, // scope of this N begins, scope of ::N interrupted
          template<N Y> class T // N here is the template parameter, not int
      >
      struct A { };

      struct B {};

      template <N T>
      struct C {};

      int p[10];

//      A<0, B, C> a;
    }


    void test() {
      auto x = TemplateParameter::x;
//      int arr2[0];
//      TemplateParameter::X<int, arr2> a; // error: a variable with non-static storage duration cannot be used as a non-type argument

//      auto a = TemplateTemplateParameter::a;
      auto z = TemplateTemplateParameter::z;
    }
  }

  namespace PointOfDeclaration {

    namespace Variables {
      //  The locus for variables and other names introduced by simple declarations is immediately after that
      //  name's declarator and before its initializer

      void test() {
        {
          unsigned char x = 32; // the first 'x' becomes visible
          {
            unsigned char x = x; // the second 'x' becomes visible before the initializer (= x)
            // - This does not initialize the second 'x' with the value 32,
            // - This initializes the second 'x' with its own, indeterminate, value
            assert(x != 32);
          }
          assert(x == 32);
        }

        std::function<int(int)> f = [&](int n) {
          return (n > 1) ? n * f(n - 1) : n;
        };
        // the name of the function 'f' is in scope within the lambda, and can
        // be correctly captured by reference, giving a recursive function

        assert(f(5) == 120);

        {
          const int x = 2; // the first 'x' becomes visible
          {
            int x[x] = {}; // the second 'x' becomes visible before the initializer (= {})
            assert(std::size(x) == 2);
            assert(x[0] == 0);
            assert(x[1] == 0);
            // but after the declarator (x[x]).
            // Within the declarator, the outer 'x' is still in scope. This declares an array of 2 int.
          }
          assert(x == 2);
        }
      }
    }

    namespace ClassAndEnum {
      // The locus of a:
      //  - class
      //  - class template declaration
      //  - enum specifier
      //  - opaque enum declaration
      // is immediately after the identifier that names the class|enum

      // (Or the template-id that names the template specialization) appears in its class-head.

      // The class or class template name is already in scope in the list of the base classes:
      struct S: std::enable_shared_from_this<S> { };

      // The locus for an injected-class-name is immediately following the opening brace of its class (or class template) definition:
      template <class T>
      struct Array
      // : std::enable_shared_from_this<Array> // Error: the injected class name is not in scope
          : std::enable_shared_from_this< Array<T> > //OK: the template-name Array is in scope
      { // the injected class name Array is now in scope as if a public member name
        Array* p; // pointer to Array<T>
      };

      enum E : int { // E is already in scope
        A = sizeof(E)
      };

      // The locus of an enumerator is immediately after its definition (not before the initializer as it is for variables):
      void test() {
        const int x = 12;
        {
          enum {
            x = x + 1, // point of declaration is at the comma, x is initialized to 13
            y = x + 1  // the enumerator x is now in scope, y is initialized to 14
          };
          assert(x == 13);
          assert(y == 14);
        }
      }
    }

    namespace UsingDeclaration {
      // The locus for a declarator in a using declaration that does not name a constructor is immediately after the declarator:
      template<int N>
      class base {
      protected:
        static const int next = N + 1;
        static const int value = N;
      };

      struct derived : base<0>, base<1>, base<2> {
        using base<0>::next;      // next is now in scope
        using base<next>::value; // derived::value is 1
      };

      // The locus of a type alias or alias template declaration is immediately after the type-id to which the alias refers:
      void test() {
        using T = int; // point of declaration of T is at the semicolon
        {
          int x;
          using T = T*;   // same as T = int*
        }

        assert(derived::value == 1);
        assert(derived::next == 1);
      }
    }

    namespace cxx17 {
      // The locus for a template parameter is immediately after its complete template parameter (including the optional default argument):
      typedef unsigned char T;
      template<class T =
      T  // lookup finds the typedef name of unsigned char
          , T               // lookup finds the template parameter
          N = 0> struct A { };

      // The locus for:
      // todo: see the structured bindings (since C++17) https://en.cppreference.com/w/cpp/language/structured_binding
      // - the variable or structured bindings declared in the range_declaration of a range-based for statement
      // is immediately after the range_expression
      void test() {
        std::vector<int> x;
        for (auto x : x) { // OK: the second x refers the std::vector<int>
          // x refers the loop variable in the body of the loop
        }

        A a;
      }
    }

    void test() {
      Variables::test();
      ClassAndEnum::test();
      UsingDeclaration::test();
      cxx17::test();
    }
  }


  void test() {
    BlockScope::test();
    FunctionParameterScope::test();
    ClassScope::test();
    NamespaceScope::test();
    EnumerationScope::test();
    TemplateScope::test();
    PointOfDeclaration::test();
  }
}