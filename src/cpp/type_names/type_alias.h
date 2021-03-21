#pragma once

#include <vector>
#include "../helper.h"

namespace Alias {
  // Type alias is a name that refers to a previously defined type (similar to typedef).
  // Alias template is a name that refers to a family of types.

  // 1. Introduces a name which can be used as a synonym for the type denoted by type-id.
  // 2. Does not introduce a new type and it cannot change the meaning of an existing type name.
  // 3. There is no difference between a type alias declaration and typedef declaration.
  //  This declaration may appear in block scope, class scope, or namespace scope.

  // Usage:
  //  using identifier attr(optional) = type-id ;	(1)

  //  template < template-parameter-list >
  //  using identifier attr(optional) = type-id ; (2)

  namespace T1 {
    //    An alias template is a template which, when specialized, is equivalent to the result of substituting
    //  the template arguments of the alias template for the template parameters in the type-id
    template<class T>
    struct Alloc : std::allocator<T> {
    };

    template<class T>
    using Vec = std::vector<T, Alloc<T>>; // type-id is vector<T, Alloc<T>>

    void test() {
      Vec<int> v; // Vec<int> is the same as vector<int, Alloc<int>>
    }
  }

  namespace T2 {
    // When the result of specializing an alias template is a dependent template-id, subsequent substitutions apply to that template-id:
    template<typename...>
    using void_t = void;

    struct A {
      typedef int foo;
    };

    template<typename T>
    void_t<typename T::foo> f() {
      std::cout << __PRETTY_FUNCTION__ << '\n';
    }

    void test() {
      //  f<int>(); // error, int does not have a nested type foo
      f<A>();
    }
  }

  namespace T3 {
    //todo: investigate
//    template<class T>
//    struct A;
//
//    template<class T>
//    using B = typename A<T>::U; // type-id is A<T>::U
//
//    template<class T>
//    struct A {
//      typedef B<T> U;
//    };

    void test() {
      //  B<short> b; // error: B<short> uses its own type via A<short>::U
    }
  }

  namespace T4 {
    // type alias, identical to typedef std::ios_base::fmtflags flags;
    using flags = std::ios_base::fmtflags;
    typedef std::ios_base::fmtflags flags;
    // the name 'flags' now denotes a type:
    flags fl = std::ios_base::dec;

    // type alias, identical to typedef void (*func)(int, int);
    using func = void (*)(int, int);
    typedef void (*func)(int, int);

    // the name 'func' now denotes a pointer to function:
    void example(int, int) {}
    func f = example;

    // alias template
    template<class T>
    using ptr = T*;
    // the name 'ptr<T>' is now an alias for pointer to T
    ptr<int> x;

    // type alias used to hide a template parameter
    template<class CharT>
    using mystring = std::basic_string<CharT, std::char_traits<CharT>>;
    mystring<char> str;

    // type alias can introduce a member typedef name
    template<typename T>
    struct Container {
      using value_type = T;
    };

    // which can be used in generic programming
    template<typename ContainerType>
    void g(const ContainerType& c) {
      typename ContainerType::value_type n;
      std::cout << __PRETTY_FUNCTION__ << ", containerType::value_type is "  << type_name<decltype(n)>() << '\n';
    }

    // type alias used to simplify the syntax of std::enable_if
    template<typename T>
    using Invoke = typename T::type;

    template<typename Condition>
    using EnableIf = Invoke<std::enable_if<Condition::value>>;

    template<typename T, typename = EnableIf<std::is_polymorphic<T>>>
    int fpoly_only(T t) {
      return 1;
    }

    struct S {
      virtual ~S() {}
    };

    void test() {
      Container<int> c;
      g(c); // Container::value_type will be int in this function
//      fpoly_only(c); // error: enable_if prohibits this
      S s;
      fpoly_only(s); // okay: enable_if allows this
    }
  }

  void test() {
    T1::test();
    T2::test();
    T3::test();
    T4::test();
  }
}