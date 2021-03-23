#pragma once

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
  }


  void test() {
    Creation::test();
    ObjectRepresentationValueRepresentation::test();
  }
}