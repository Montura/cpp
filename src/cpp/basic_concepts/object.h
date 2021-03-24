#pragma once

#include <typeinfo>

namespace Object {
  namespace Description {
    // An object, in C++, has
    //  1. size (can be determined with sizeof);
    //  2. alignment requirement (can be determined with alignof);
    //  3. storage duration (automatic, static, dynamic, thread-local);
    //  4. lifetime (bounded by storage duration or temporary);
    //  5. type;
    //  6. value (which may be indeterminate, e.g. for default-initialized non-class types);
    //  7. optionally, a name.

    // The following entities are NOT objects:
    // - value
    // - reference
    // - function
    // - enumerator
    // - type
    // - non-static class member
    // - template
    // - class or function template specialization
    // - namespace
    // - parameter pack
    // - "this"

    // A variable is an object or a reference that is not a non-static data member, that is introduced by a declaration.
  }

  namespace Creation {
    // Objects can be explicitly created by:
    //    1. Definitions
    //    2. New-expressions
    //    3. Throw-expressions
    //    4. Changing the active member of a union
    //    5. Evaluating expressions that require temporary objects

    // Objects can be implicitly created by:
    //    1. Operations that begin lifetime of an array of type char, unsigned char, or (std::byte, since C++17)
    //    in which case such objects are created in the array
    //    2. Call to following allocating functions, in which case such objects are created in the allocated storage:
    //       - operator new
    //       - operator new[]
    //       - std::malloc
    //       - std::calloc
    //       - std::realloc
    //       - std::aligned_alloc (since C++17)
    //    3. Call to following object representation copying functions, in which case such objects are created in the
    //    destination region of storage or the result:
    //       - std::memcpy
    //       - std::memmove
    //       - std::bit_cast (since C++20)

    struct X {
      int a, b;
    };

    X* makeX() {
      //  std::malloc implicitly creates an object of type X and its subobjects (a and b), and returns a pointer to that X object.
      X* p = static_cast<X*>(std::malloc(sizeof(X)));
      p->a = 1;
      p->b = 2;
      return p;
    }

    void test() {
      X* xPtr = makeX();
    }
  }

  namespace ObjectRepresentationValueRepresentation {
    //    The object representation for an object of type T -> is the sequence of sizeof(T) objects of type unsigned char
    // beginning at the same address as the T object.
    //    The value representation of an object -> is the set of bits that hold the value of its type T.

    namespace TriviallyCopyable {
      //    For TriviallyCopyable types, value representation is a part of the object representation,
      // which means that copying the bytes occupied by the object in the storage is sufficient to produce another
      // object with the same value
    }

    // The reverse is not necessarily true:
    //    Two objects of TriviallyCopyable type with different object representations may represent the same value.
    // Ex.:
    //  1. Multiple floating-point bit patterns represent the same special value NaN.
    //  2. More commonly, some bits of the object representation may not participate in the value representation at all;
    //  (such bits may be padding introduced to satisfy alignment requirements, bit field sizes, etc).

    // some bits of the object representation may not participate in the value representation at all []
    struct S {
      char c;  // 1 byte value
      // 3 bytes padding (assuming alignof(float) == 4)
      float f; // 4 bytes value (assuming sizeof(float) == 4)

      bool operator==(const S& arg) const { // value-based equality
        return (c == arg.c) && (f == arg.f);
      }
    };

    void test() {
      assert(sizeof(S) == 8);
      S s1 = {'a', 3.14};
      S s2 = s1;
      reinterpret_cast<unsigned char*>(&s1)[2] = 'b'; // change 2nd byte of padding
      assert(s1 == s2); // value did not change
    }
    // todo:
    //  see TriviallyCopiable (https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)
  }

  namespace SubObjects {
    // Object can have subobjects:
    //  - member objects
    //  - base class subobjects
    //  - array elements

    // Complete object - an object that is not a subobject of another object.

    // Most derived objects - complete objects, member objects, and array elements,
    //                        to distinguish them from base class subobjects.

    // The size of an object that is neither potentially overlapping nor a bit field is required to be non-zero!!!!!!!!
    // (the size of a base class subobject may be zero: see empty base optimization*)

    //    Any two objects with overlapping lifetimes* are guaranteed to have different addresses unless:
    //    - one of them is nested within another
    //    - if they are subobjects of different type within the same complete object, and one of them is a subobject of zero size.

    // todo:
    //   * lifetimes (https://en.cppreference.com/w/cpp/language/lifetime)
    //   * empty base optimization (https://en.cppreference.com/w/cpp/language/ebo)

    void test_different_addresses() {
      static const char c1 = 'x';
      static const char c2 = 'x';
      assert(&c1 != &c2); // same values, different addresses
    }

    void test() {
      test_different_addresses();
    }
  }

  namespace PolymorphicObjects {
    // Polymorphic objects - objects of a class type that declares or inherits at least one virtual function.
    //
    //    Within each polymorphic object, the implementation stores additional information
    // (in every existing implementation, it is one pointer unless optimized out),
    // which is used by:
    //  - virtual function calls
    //  - RTTI features (dynamic_cast and typeid)
    //  to determine, at run time, the type with which the object was created, regardless of the expression it is used in.

    // For non-polymorphic objects:
    //    The interpretation of the value is determined from the expression in which the object is used,
    //    and is decided at compile time.

    struct Base1 {
      // polymorphic type: declares a virtual member
      virtual ~Base1() {}
    };

    struct Derived1 : Base1 {
      // polymorphic type: inherits a virtual member
    };

    struct Base2 {
      // non-polymorphic type
    };

    struct Derived2 : Base2 {
      // non-polymorphic type
    };

    void test() {
      Derived1 obj1; // object1 created with type Derived1
      Derived2 obj2; // object2 created with type Derived2

      Base1& b1 = obj1; // b1 refers to the object obj1
      Base2& b2 = obj2; // b2 refers to the object obj2

      std::cout << "Expression type of b1: " << typeid(decltype(b1)).name() << '\n'
                << "Expression type of b2: " << typeid(decltype(b2)).name() << '\n'
                << "Object type of b1: " << typeid(b1).name() << '\n'
                << "Object type of b2: " << typeid(b2).name() << '\n'
                << "Size of b1: " << sizeof b1 << '\n'
                << "Size of b2: " << sizeof b2 << '\n';
    }

    // todo:
    //  casts (static, dynamic, const, reinterpret)
  }

  namespace Alignment {
    // Every object type has the property called alignment requirement:
    //  - Is an integer value of type std::size_t
    //  - Is always a power of 2
    //  - Is representing the number of bytes between successive addresses at which objects of this type can be allocated.

    // Each object type imposes its alignment requirement on every object of that type.

    // Since C++11
    // 1. The alignment requirement of a type can be queried with alignof or std::alignment_of.
    // 2. The pointer alignment functions can be use to ... :
    //   - std::align -> ... obtain a suitably-aligned pointer within some buffer
    //   - std::aligned_storage -> ... obtain suitably-aligned storage.

    namespace AlignOf {
      //  Returns the alignment, in bytes, required for any instance of the type indicated by type-id, which is:
      //    - complete object type
      //    - an array type whose element type is complete
      //    - a reference type to one of those types
      //
      //  1. If the type is reference type, the operator returns the alignment of referenced type.
      //  2. If the type is array type, alignment requirement of the element type is returned.

      struct Foo {
        int   i;
        float f;
        char  c;
      };

      // Note: `alignas(alignof(long double))` below can be simplified to simply
      // `alignas(long double)` if desired.
      struct alignas(alignof(long double)) Foo2 {
        // put your definition here
      };

      struct Empty {};

      struct alignas(64) Empty64 {};

      void test() {
        std::cout << "Alignment of"  "\n"
         "- char             : " << alignof(char)    << "\n"
         "- pointer          : " << alignof(int*)    << "\n"
         "- class Foo        : " << alignof(Foo)     << "\n"
         "- class Foo2       : " << alignof(Foo2)    << "\n"
         "- empty class      : " << alignof(Empty)   << "\n"
         "- alignas(64) Empty: " << alignof(Empty64) << "\n";
      }
    }

    //    In order to satisfy alignment requirements of all non-static members of a class,
    // padding may be inserted after some of its members.

    namespace AlignmentPadding {
      // Objects of type S can be allocated at any address because:
      //  1. Both S.a and S.b can be allocated at any address
      struct S {
        char a; // size: 1, alignment: 1
        char b; // size: 1, alignment: 1
      }; // size: 2, alignment: 1

      // Objects of type X must be allocated at 4-byte boundaries because:
      //  1. X.n must be allocated at 4-byte boundaries
      //  2. int's alignment requirement is (usually) 4
      struct X {
        int n;  // size: 4, alignment: 4
        char c; // size: 1, alignment: 1
        // three bytes padding
      }; // size: 8, alignment: 4

      void test() {
        std::cout << "sizeof(S) = " << sizeof(S) << " alignof(S) = " << alignof(S) << '\n';
        std::cout << "sizeof(X) = " << sizeof(X) << " alignof(X) = " << alignof(X) << '\n';
      }

      // The weakest alignment (the smallest alignment requirement equals 1) is the alignment of:
      // - char
      // - signed char
      // - unsigned char

      // Since C++11
      // The largest fundamental alignment of any type is implementation-defined and equals to the alignment of std::max_align_t


      namespace ExtendedAlignment {
        // Extended alignment requirement - if a type's alignment is made stricter (larger) than std::max_align_t

        // Over-aligned type:
        // - A type whose alignment is extended
        // - A class type whose non-static data member has extended alignment.

        // It is implementation-defined if next functions support over-aligned types:
        //  1. new-expression (until C++17)
        //  2. std::allocator::allocate
        //  3. std::get_temporary_buffer (until C++20).

        // Allocators instantiated with over-aligned types are allowed:
        //  1. To fail to instantiate at compile time
        //  2. To throw std::bad_alloc at runtime
        //  3. To silently ignore unsupported alignment requirement
        //  4. To handle them correctly.
      }

    }

    void test() {
      AlignOf::test();
      AlignmentPadding::test();
    }
  }

  void test() {
    Creation::test();
    ObjectRepresentationValueRepresentation::test();
    SubObjects::test();
    PolymorphicObjects::test();
    Alignment::test();
  }
}