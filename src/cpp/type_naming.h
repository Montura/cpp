#pragma once

#include <vector>
#include <functional>
#include <typeinfo>

#include "type_names/clazz.h"
#include "type_names/enum.h"
#include "type_names/type_alias.h"
#include "type_names/typedef.h"
#include "type_names/union.h"
#include "type_names/injected_class_name.h"
#include "type_names/elaborated_type_specifier.h"

namespace TypeNaming {
  // A name can be declared to refer to a type by means of:
  //  - Class declaration;
  //  - Union declaration;
  //  - Enum declaration;
  //  - Typedef alias declaration;
  //  - Type alias declaration.

  // Types that do not have names often need to be referred to in C++ programs;
  // The syntax for that is known as type-id.

  // Type-id may be used in the following situations:

  // To specify the target type in cast expressions;
  //  - as arguments to sizeof, alignof, alignas, new, and typeid;
  //  - on the right-hand side of a type alias declaration;
  //  - as the trailing return type of a function declaration;
  //  - as the default argument of a template type parameter;
  //  - as the template argument for a template type parameter;
  //  - in dynamic exception specification.

  //  Type-id can be used with some modifications in the following situations:
  //  - in the parameter list of a function (when the parameter name is omitted);
  //  - in the name of a user-defined conversion function, the abstract declarator cannot include function or array operators.


  void f(int) {}                    // declaration of a function taking int and returning void

  template <class T>
  void check_typeid(T p, const char* const expected_type_id) {
    const char* pTypeId = typeid(p).name();
    assert(strcmp(pTypeId, expected_type_id) == 0);
  }

  void test_typeids() {
    int* p = 0;               // declaration of a pointer to int
    int* pInt = static_cast<int*>(p); // type-id is "int*"

//    check_typeid(p, "Pi");
//    check_typeid(pInt, "Pi");
    
    int a[3];   // declaration of an array of 3 int
    auto arr = new int[3]; // type-id is "int[3]" (called new-type-id)

//    check_typeid(a, "Pi");
//    check_typeid(arr, "Pi");

    int (*(*x[2])())[3]; // declaration of an array of 2 pointers to functions returning pointer to array of 3 int (PPFPA3_ivE)
    int (*(*z[3])())[4]; // declaration of an array of 2 pointers to functions returning pointer to array of 3 int (PPFPA4_ivE)
    auto fPtr = new (int (*(*[2])())[3]); // type-id is "int (*(*[2])())[3]"

//    check_typeid(x, "PPFPA3_ivE");
//    check_typeid(z, "PPFPA4_ivE");
//    check_typeid(fPtr, "PPFPA3_ivE");

    std::function<void(int)> func = f; // type template parameter is a type-id "void(int)"
    std::function<auto(int) -> void> y = f; // same

//    check_typeid(f, "PFviE");
//    check_typeid(func, "NSt3__18functionIFviEEE");
//    check_typeid(y, "NSt3__18functionIFviEEE");

    std::vector<float> vv;       // declaration of a vector of int
    size_t i = sizeof(std::vector<int>); // type-id is "std::vector<int>"

    struct { int x; } b;         // creates a new type and declares an object b of that type
//      sizeof(struct{ int x; });    // error: cannot define new types in a sizeof expression
    using t = struct { int x; }; // creates a new type and declares t as an alias of that type

//      sizeof(static int); // error: storage class specifiers not part of type-specifier-seq
//      std::function<inline void(int)> f; // error: neither are function specifiers
  }

  void test() {
    test_typeids();
    Clazz::test();
    ElaboratedTypeSpecifier::test();
    Union::test();
    Alias::test();
    Enumeration::test();
  }
}

namespace DynamicType {
// 1. If some glvalue expression refers to a polymorphic object, the type of its most derived object is known as the dynamic type.
// 2. For prvalue expressions, the dynamic type is always the same as the static type.
  struct B { virtual ~B() {} }; // polymorphic type
  struct D: B {}; // polymorphic type

  void test() {
    D d; // most-derived object
    B* ptr = &d; // the static type of (*ptr) is B, the dynamic type of (*ptr) is D
  }
}

//namespace IncompleteType {
//  The following types are incomplete types:

//  1. The type void (possibly cv-qualified);
//  2. Incompletely-defined object types:
//    - Class type that has been declared (e.g. by forward declaration) but not defined;
//    - Array of unknown bound;
//    - Array of elements of incomplete type;
//    - Enumeration type from the point of declaration until its underlying type is determined.
//  All other types are complete.

// An incompletely-defined object type can be completed:
//  A class type might be incomplete at one point in a translation unit and complete later on;

namespace IncompleteType {
  namespace Case_1 {
    struct X;             // X is an incomplete type
    extern X* xp;         // xp is a pointer to an incomplete type

    struct X {
      int i;
    };  // now X is a complete type

    void foo() {
//    xp++;               // ill-formed: X is incomplete
    }

    X x;

    void test() {
//      xp = &x;            // OK: type is “pointer to X”
//      xp++;               // OK: X is complete
    }
  }

  namespace Case_2 {
    extern int arr[];   // the type of arr is incomplete
    typedef int UNKA[]; // UNKA is an incomplete type

    UNKA* arrp;         // arrp is a pointer to an incomplete type
    UNKA** arrpp;

    void foo() {
//      arrp++;           // error: incomplete type
      arrpp++;          // OK: sizeof UNKA* is known
    }

    int arr[10];        // now the type of arr is complete

    void test() {
//      arrp = &arr;      // error: different types
//      arrp++;           // error: UNKA can’t be completed
    }
  }
}