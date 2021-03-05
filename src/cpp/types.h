#pragma once

namespace Types {
  #include <type_traits>
  #include <iostream>

  namespace Fundamental {
    template< class T >
    struct is_fundamental
        : std::integral_constant<
            bool,
            std::is_arithmetic<T>::value ||
            std::is_void<T>::value  ||
            std::is_null_pointer<T>::value
        > {};

    class A {};
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
    template< class T >
    inline constexpr bool is_compound_v = std::is_compound<T>::value;

    template< class T >
    struct is_compound : std::integral_constant<bool, !std::is_fundamental<T>::value> {};

    class A {};
    void test() {
      std::cout << std::boolalpha;
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

      std::cout << "int is pointer    \t" << std::is_pointer_v<int> << '\n';
      std::cout << "int& is pointer   \t" << std::is_pointer_v<int&> << '\n';
      std::cout << "int* is pointer   \t" << std::is_pointer_v<int*> << '\n';
      std::cout << "int** is pointer  \t" << std::is_pointer_v<int**> << '\n';
      std::cout << "A is pointer      \t" << std::is_pointer_v<A> << '\n';
      std::cout << "A& is pointer     \t" << std::is_pointer_v<A&> << '\n';
      std::cout << "A* is pointer     \t" << std::is_pointer_v<A*> << '\n';
      std::cout << "A** is pointer    \t" << std::is_pointer_v<A**> << '\n';

    }
  }

  void test() {
    Fundamental::test();
    Compound::test();
  }
}