#pragma once

namespace TypeClassification {
  #include <type_traits>
  #include <iostream>

  typedef void(*FuncPtr)(int*);

  struct A {
    int x;
    int foo() { return x; };

    static int f(int arg) { return 0; };
  };

  class B {};

  struct C {
    C() {}
  };

  struct D {
    int m1;
  private:
    int m2;
  };

  struct F {
    virtual void foo();
  };

  typedef int(A::*MethodPtr)();

  typedef union {
    int a;
    float b;
  } U;

  enum E {};
  enum class Ec : int {};

  void print(int i) {}

  namespace Fundamental {
    // Possible implementation
    template< class T >
    struct is_fundamental
        : std::integral_constant<
            bool,
            std::is_arithmetic<T>::value ||
            std::is_void<T>::value  ||
            std::is_null_pointer<T>::value
        > {};

    void test() {
      std::cout << std::boolalpha;
      std::cout << "A is fundamental:     \t"   << std::is_fundamental_v<A> << '\n';
      std::cout << "int is fundamental: \t"    << std::is_fundamental_v<int> << '\n';
      std::cout << "int& is fundamental: \t"   << std::is_fundamental_v<int&> << '\n';
      std::cout << "int* is fundamental: \t"   << std::is_fundamental_v<int*> << '\n';
      std::cout << "float is fundamental: \t"  << std::is_fundamental_v<float> << '\n';
      std::cout << "float& is fundamental: \t" << std::is_fundamental_v<float&> << '\n';
      std::cout << "float* is fundamental: \t" << std::is_fundamental_v<float*> << '\n';

      std::cout << "is_void_v<void>:               \t" << std::is_void_v<void> << '\n';
      std::cout << "is_null_pointer_v<nullptr_t>:  \t" << std::is_null_pointer_v<nullptr_t> << '\n';

      std::cout << "is_arithmetic_v<int>:          \t" << std::is_arithmetic_v<int> << '\n';
      std::cout << "is_arithmetic_v<float>:        \t" << std::is_arithmetic_v<float> << '\n';

      std::cout << "is_floating_point<double>:     \t" << std::is_floating_point_v<double> << '\n';
      std::cout << "is_floating_point<float>:      \t" << std::is_floating_point_v<float> << '\n';

      std::cout << "is_integral<bool>:             \t" << std::is_integral_v<bool> << '\n';
      std::cout << "is_integral<char>:             \t" << std::is_integral_v<char> << '\n';
      std::cout << "is_integral<int>:              \t" << std::is_integral_v<int> << '\n';
      std::cout << "is_integral<short>:            \t" << std::is_integral_v<short> << '\n';
      std::cout << "is_integral<long>:             \t" << std::is_integral_v<long> << '\n';
    }
  }

  namespace Compound {

    namespace IsCompound {
      // Possible implementation
      template< class T >
      struct is_compound : std::integral_constant<bool, !std::is_fundamental<T>::value> {};

      void test() {
        std::cout << "A is compound:      \t" << std::is_compound_v<A> << '\n';
        std::cout << "A* is compound:     \t" << std::is_compound_v<A*> << '\n';
        std::cout << "A& is compound:     \t" << std::is_compound_v<A&> << '\n';
        std::cout << "A&& is compound:    \t" << std::is_compound_v<A&&> << '\n';
        std::cout << "int is compound:    \t" << std::is_compound_v<int> << '\n';
        std::cout << "int& is compound:   \t" << std::is_compound_v<int&> << '\n';
        std::cout << "int* is compound:   \t" << std::is_compound_v<int*> << '\n';
        std::cout << "float is compound:  \t" << std::is_compound_v<float> << '\n';
        std::cout << "float& is compound: \t" << std::is_compound_v<float&> << '\n';
        std::cout << "float* is compound: \t" << std::is_compound_v<float*> << '\n';
      }
    }

    namespace IsReference {
      // Possible implementations
      template<class T> struct is_lvalue_reference     : std::false_type {};
      template<class T> struct is_lvalue_reference<T&> : std::true_type {};

      template<class T> struct is_rvalue_reference      : std::false_type {};
      template<class T> struct is_rvalue_reference<T&&> : std::true_type {};

      void test() {
        std::cout << "A is lvalue     \t" << std::is_lvalue_reference_v<A> << '\n';
        std::cout << "A* is lvalue    \t" << std::is_lvalue_reference_v<A*> << '\n';
        std::cout << "A& is lvalue    \t" << std::is_lvalue_reference_v<A&> << '\n';
        std::cout << "A&& is lvalue   \t" << std::is_lvalue_reference_v<A&&> << '\n';
        std::cout << "int is lvalue   \t" << std::is_lvalue_reference_v<int> << '\n';
        std::cout << "int& is lvalue  \t" << std::is_lvalue_reference_v<int&> << '\n';
        std::cout << "int&& is lvalue \t" << std::is_lvalue_reference_v<int&&> << '\n';

        std::cout << "A is rvalue     \t" << std::is_rvalue_reference_v<A> << '\n';
        std::cout << "A* is rvalue    \t" << std::is_rvalue_reference_v<A*> << '\n';
        std::cout << "A& is rvalue    \t" << std::is_rvalue_reference_v<A&> << '\n';
        std::cout << "A&& is rvalue   \t" << std::is_rvalue_reference_v<A&&> << '\n';
        std::cout << "int is rvalue   \t" << std::is_rvalue_reference_v<int> << '\n';
        std::cout << "int& is rvalue  \t" << std::is_rvalue_reference_v<int&> << '\n';
        std::cout << "int&& is rvalue \t" << std::is_rvalue_reference_v<int&&> << '\n';
      }
    }

    namespace IsPointer {
      // Possible implementations
      template<class T>
      struct is_pointer_helper : std::false_type {};

      template<class T>
      struct is_pointer_helper<T*> : std::true_type {};

      template<class T>
      struct is_pointer : is_pointer_helper< typename std::remove_cv<T>::type > {};

      void test() {
        std::cout << "int is pointer     \t" << std::is_pointer_v<int> << '\n';
        std::cout << "int& is pointer    \t" << std::is_pointer_v<int&> << '\n';
        std::cout << "int* is pointer    \t" << std::is_pointer_v<int*> << '\n';
        std::cout << "int** is pointer   \t" << std::is_pointer_v<int**> << '\n';
        std::cout << "A is pointer       \t" << std::is_pointer_v<A> << '\n';
        std::cout << "A& is pointer      \t" << std::is_pointer_v<A&> << '\n';
        std::cout << "A* is pointer      \t" << std::is_pointer_v<A*> << '\n';
        std::cout << "A** is pointer     \t" << std::is_pointer_v<A**> << '\n';
        std::cout << "FuncPtr is pointer \t" << std::is_pointer_v<FuncPtr> << '\n';


        int A::* xPtr = &A::x;
        A a = {7};
        assert(a.*xPtr == 7);
        MethodPtr methodPtr = &A::foo;
        assert((a.*methodPtr)() == 7);
        std::cout << "&A::x is pointer                    \t" << std::is_pointer_v<int A::*> << '\n';
        std::cout << "&A::foo is pointer                  \t" << std::is_pointer_v<MethodPtr> << '\n';
        std::cout << "&A::x is member pointer             \t" << std::is_member_pointer_v<int A::*> << '\n';
        std::cout << "&A::x is member object pointer      \t" << std::is_member_object_pointer_v<int A::*> << '\n';
        std::cout << "&A::foo is member pointer           \t" << std::is_member_pointer_v<MethodPtr> << '\n';
        std::cout << "&A::foo is member function pointer  \t" << std::is_member_function_pointer_v<MethodPtr> << '\n';
      }
    }

    namespace IsArray {
      // Possible implementations
      template<class T>
      struct is_array : std::false_type {};

      template<class T>
      struct is_array<T[]> : std::true_type {};

      template<class T, std::size_t N>
      struct is_array<T[N]> : std::true_type {};

      void test() {
        std::cout << "A                   is array  \t" << std::is_array_v<A> << '\n';
        std::cout << "A[]                 is array  \t" << std::is_array_v<A[]> << '\n';
        std::cout << "A[3]                is array  \t" << std::is_array_v<A[3]> << '\n';
        std::cout << "float               is array  \t" << std::is_array_v<float> << '\n';
        std::cout << "int                 is array  \t" << std::is_array_v<int> << '\n';
        std::cout << "int[]               is array  \t" << std::is_array_v<int[]> << '\n';
        std::cout << "int[3]              is array  \t" << std::is_array_v<int[3]> << '\n';
        std::cout << "std::array<int, 3>  is array  \t" << std::is_array_v<std::array<int, 3>> << '\n';
        std::cout << "char*               is array  \t" << std::is_array_v<char*> << '\n';
        std::cout << "char[]              is array  \t" << std::is_array_v<char[]> << '\n';
      }
    }

    namespace IsFunction {
      // Possible implementation
      template<class T>
      struct is_function : std::integral_constant<
          bool,
          !std::is_const<const T>::value && !std::is_reference<T>::value
      > {};

      namespace Edu {
        // primary template
        template<class>
        struct is_function : std::false_type {
        };

        // specialization for regular functions
        template<class Ret, class... Args>
        struct is_function<Ret(Args...)> : std::true_type {
        };
      }

      void test() {
        // Types like:
        // - std::function
        // - lambdas
        // - classes with overloaded operator()
        // - pointers to functions
        // DON'T count as function types.

        int i = 2;
        std::function<int(int)> x = A::f;
        assert(x(i) == 0);

        std::function<int()> lambda = [=]() { return x(i); };
        assert(lambda() == 0);

        std::cout << "int(int)                is function \t" << std::is_function_v<int(int)> << '\n';
        std::cout << "void print()            is function \t" << std::is_function_v<decltype(print)> << '\n';
        std::cout << "A                       is function \t" << std::is_function_v<A> << '\n';
        std::cout << "int                     is function \t" << std::is_function_v<int> << '\n';
        std::cout << "std::function<int(int)> is function \t" << std::is_function_v<decltype(x)> << '\n';
        std::cout << "[=](){ return x(i); }   is function \t" << std::is_function_v<decltype(lambda)> << '\n';
        std::cout << "&A::foo                 is function \t" << std::is_function_v<MethodPtr> << '\n';
      }
    }

    namespace IsEnumClassUnion {
      // Possible implementation
      namespace detail {
        template <class T>
        std::integral_constant<bool, !std::is_union<T>::value> test(int T::*);

        template <class>
        std::false_type test(...);
      }

      template <class T>
      struct is_class : decltype(detail::test<T>(nullptr))
      {};

      void test() {
        std::cout << "A is enum     \t" << std::is_enum_v<A> << '\n';
        std::cout << "A is union    \t" << std::is_union_v<A> << '\n';
        std::cout << "A is class    \t" << std::is_class_v<A> << '\n';

        std::cout << "B is enum     \t" << std::is_enum_v<B> << '\n';
        std::cout << "B is union    \t" << std::is_union_v<B> << '\n';
        std::cout << "B is class    \t" << std::is_class_v<B> << '\n';

        std::cout << "int is enum   \t" << std::is_enum_v<int> << '\n';
        std::cout << "int is union  \t" << std::is_union_v<int> << '\n';
        std::cout << "int is class  \t" << std::is_class_v<int> << '\n';

        std::cout << "E is enum     \t" << std::is_enum_v<E> << '\n';
        std::cout << "E is union    \t" << std::is_union_v<E> << '\n';
        std::cout << "E is class    \t" << std::is_class_v<E> << '\n';

        std::cout << "Ec is enum    \t" << std::is_enum_v<Ec> << '\n';
        std::cout << "Ec is union   \t" << std::is_union_v<Ec> << '\n';
        std::cout << "Ec is class   \t" << std::is_class_v<Ec> << '\n';

        std::cout << "U is enum   \t" << std::is_enum_v<U> << '\n';
        std::cout << "U is union  \t" << std::is_union_v<U> << '\n';
        std::cout << "U is class  \t" << std::is_class_v<U> << '\n';
      }
    }

    namespace IsObject {
      // Possible implementation for is_object
      // That is any possibly cv-qualified type other than function, reference, or void types

      template<class T>
      struct is_object : std::integral_constant<bool,
          std::is_scalar<T>::value ||
          std::is_array<T>::value ||
          std::is_union<T>::value ||
          std::is_class<T>::value> {
      };

      void test() {
        std::cout << "int is object     \t" << std::is_object_v<int> << '\n';
        std::cout << "int& is object    \t" << std::is_object_v<int&> << '\n';
        std::cout << "int* is object    \t" << std::is_object_v<int*> << '\n';
        std::cout << "A is object       \t" << std::is_object_v<A> << '\n';
        std::cout << "A& is object      \t" << std::is_object_v<A&> << '\n';
        std::cout << "A* is object      \t" << std::is_object_v<A*> << '\n';
        std::cout << "U is object       \t" << std::is_object_v<U> << '\n';
        std::cout << "U& is object      \t" << std::is_object_v<U&> << '\n';
        std::cout << "U* is object      \t" << std::is_object_v<U*> << '\n';
      }
    }

    namespace IsScalar {
      // Possible implementation for is_scalar
      // That is a possibly cv-qualified arithmetic, pointer, pointer to member, enumeration, or std::nullptr_t type
      template<class T>
      struct is_scalar : std::integral_constant<bool,
          std::is_arithmetic<T>::value ||
          std::is_enum<T>::value ||
          std::is_pointer<T>::value ||
          std::is_member_pointer<T>::value ||
          std::is_null_pointer<T>::value> {
      };

      void test() {
        std::cout << "int is scalar     \t" << std::is_scalar_v<int> << '\n';
        std::cout << "int& is scalar    \t" << std::is_scalar_v<int&> << '\n';
        std::cout << "int* is scalar    \t" << std::is_scalar_v<int*> << '\n';
        std::cout << "A is scalar       \t" << std::is_scalar_v<A> << '\n';
        std::cout << "A& is scalar      \t" << std::is_scalar_v<A&> << '\n';
        std::cout << "A* is scalar      \t" << std::is_scalar_v<A*> << '\n';
        std::cout << "U is scalar       \t" << std::is_scalar_v<U> << '\n';
        std::cout << "U& is scalar      \t" << std::is_scalar_v<U&> << '\n';
        std::cout << "U* is scalar      \t" << std::is_scalar_v<U*> << '\n';
        std::cout << "E is scalar       \t" << std::is_scalar_v<E> << '\n';
        std::cout << "E& is scalar      \t" << std::is_scalar_v<E&> << '\n';
        std::cout << "E* is scalar      \t" << std::is_scalar_v<E*> << '\n';
      }
    }

    namespace IsTrivial {
      // That is:
      // - a scalar type
      // - a trivially copyable class with a trivial default constructor
      // - an array of such type/class

      // Possible implementation
      template< class T >
      struct is_trivial : std::integral_constant<
          bool,
          std::is_trivially_copyable<T>::value &&
          std::is_trivially_default_constructible<T>::value
      > {};

      void test() {
        std::cout << "A is trivial \t" << std::is_trivial_v<A> << '\n';
        std::cout << "B is trivial \t" << std::is_trivial_v<B> << '\n';
        std::cout << "C is trivial \t" << std::is_trivial_v<C> << '\n';
      }
    }

    namespace IsPod {
      // https://en.cppreference.com/w/cpp/named_req/PODType
      // POD - "plain old data type".
      // That is, both trivial and standard-layout type
      void test() {
        std::cout << "A is POD    \t" << std::is_pod_v<A> << '\n';
        std::cout << "A& is POD   \t" << std::is_pod_v<A&> << '\n';
        std::cout << "A* is POD   \t" << std::is_pod_v<A*> << '\n';
        std::cout << "B is POD    \t" << std::is_pod_v<B> << '\n';
        std::cout << "C is POD    \t" << std::is_pod_v<C> << '\n';
        std::cout << "D is POD    \t" << std::is_pod_v<D> << '\n';
        std::cout << "E is POD    \t" << std::is_pod_v<E> << '\n';
        std::cout << "F is POD    \t" << std::is_pod_v<F> << '\n';
        std::cout << "U is POD    \t" << std::is_pod_v<U> << '\n';
        std::cout << "int is POD  \t" << std::is_pod_v<int> << '\n';
        std::cout << "int& is POD \t" << std::is_pod_v<int&> << '\n';
        std::cout << "int* is POD \t" << std::is_pod_v<int*> << '\n';
      }
    }

    namespace IsLiteralType {
      // This type trait has been deprecated in C++17 and removed in C++20 as offering negligible value to generic code
      void test() {
        std::cout << "A is literal type     \t" << std::is_literal_type_v<A> << '\n';
        std::cout << "A& is literal type    \t" << std::is_literal_type_v<A&> << '\n';
        std::cout << "A* is literal type    \t" << std::is_literal_type_v<A*> << '\n';
        std::cout << "B is literal type     \t" << std::is_literal_type_v<B> << '\n';
        std::cout << "C is literal type     \t" << std::is_literal_type_v<C> << '\n';
        std::cout << "D is literal type     \t" << std::is_literal_type_v<D> << '\n';
        std::cout << "E is literal type     \t" << std::is_literal_type_v<E> << '\n';
        std::cout << "F is literal type     \t" << std::is_literal_type_v<F> << '\n';
        std::cout << "U is literal type     \t" << std::is_literal_type_v<U> << '\n';
        std::cout << "int is literal type   \t" << std::is_pod_v<int> << '\n';
        std::cout << "int& is literal type  \t" << std::is_pod_v<int&> << '\n';
        std::cout << "int* is literal type  \t" << std::is_pod_v<int*> << '\n';
      }
    }

    void test() {
      IsCompound::test();
      IsReference::test();
      IsPointer::test();
      IsArray::test();
      IsFunction::test();
      IsEnumClassUnion::test();
      IsObject::test();
      IsScalar::test();
      IsTrivial::test();
      IsPod::test();
      IsLiteralType::test();
    }
  }

  void test() {
    Fundamental::test();
    Compound::test();
  }
}