#pragma once

namespace Lifetime {
  // https://en.cppreference.com/w/cpp/language/lifetime

  // 1. Every object and reference has a a RUNTIME property - Lifetime!
  // 2. For any object or reference, there is a point of execution of a program when its lifetime begins,
  // and there is a moment when it ends.
  //
  //  The lifetime of an object begins when:
  //  1. Storage with the proper alignment and size for its type is obtained
  //  2. Its initialization is complete, except that:
  //    - if the object is a union member or subobject thereof:
  //        + its lifetime only begins if that union member is the initialized member in the union, or it is made active
  //    - if the object is nested in a union object:
  //        + its lifetime may begin if the containing union object is assigned or constructed by a trivial special member function
  //    - an array object's lifetime may also begin if it is allocated by std::allocator::allocate

  // The lifetime of an object ends when:
  // - if it is of a non-class type, the object is destroyed;
  // - if it is of a class type, the destructor call starts;
  // - the storage which the object occupies is released, or is reused by an object that is not nested within it.


  // 1. The Lifetime of an object is equal to or is nested within the lifetime of its storage, see storage duration.
  // 2. The lifetime of a reference begins when its initialization is complete and ends as if it were a scalar object.
  //
  // NOTE: the lifetime of the referred object may end before the end of the lifetime of the reference,
  // which makes dangling references possible (see DanglingReferences::f).
  //
  // 3. Lifetimes of non-static data members and base subobjects begin and end following class initialization order.



  namespace StorageReuse {
    // A program is not required to call the destructor of an object to end its lifetime:
    //  1. if the object is trivially-destructible
    //  2. if the program does not rely on the side effects of the destructor.

    namespace UB {
      namespace Test1 {
        //    However, if a program ends the lifetime of an non-trivially destructible object that is a variable explicitly,
        //  it must ensure that a new object of the same type is constructed in-place (e.g. via placement new)
        //  before the destructor may be called implicitly:
        //  - due to scope exit or exception for automatic objects
        //  - due to thread exit for thread-local objects
        //  - due to program exit for static objects
        //  otherwise the behavior is undefined.

        class T {}; // trivial

        struct B {
          ~B() {} // non-trivial
        };

        void x() {
          long long n; // automatic, trivial
          auto* pDouble = new(&n) double(3.14); // reuse with a different type okay
          assert(*pDouble == 3.14);
        } // okay

        void h() {
          B b; // automatic non-trivially destructible
          b.~B(); // end lifetime (not required, since no side-effects)
          new(&b) T; // wrong type: okay until the destructor is called
        } // todo: !!!! UB: destructor is called: undefined behavior

        void test() {
          x();
          h();
        }
        // destructor is called: undefined behavior
      }

      namespace Test2 {

        // It is undefined behavior to reuse storage that is or was occupied by a const complete object of:
        // - static
        // - thread-local
        // - automatic
        // storage duration because such objects may be stored in read-only memory.

        struct B {
          B() {}; // non-trivial
          ~B() {}; // non-trivial
        };

        const B b; // const static

        void test() {
          b.~B(); // end the lifetime of b
          new(const_cast<B*>(&b)) const B; // todo: !!!! UB: attempted reuse of a const

        }
      }
    }

    namespace TransparencyReplaceableObject {
      //    If a new object is created at the address that was occupied by another object, then all pointers, references,
      // and the name of the original object will automatically refer to the new object and,
      // once the lifetime of the new object begins, can be used to manipulate the new object,
      // but only if the original object is transparently replaceable by the new object.
      //
      //  Object X is transparently replaceable by object Y if:
      //    1. The storage for Y exactly overlays the storage location which X occupied
      //    2. Y is of the same type as X (ignoring the top-level cv-qualifiers)
      //    3. X is not a complete const object
      //    4. neither X nor Y is a base class subobject (or a member subobject declared with [[no_unique_address]] (since C++20))
      //    5. Either:
      //      - X and Y are both complete objects
      //      - X and Y are direct subobjects of objects oX and oY respectively, and oX is transparently replaceable by oY.

      struct C {
        int i;
        void f() {}
        const C& operator=( const C& );
      };

      const C& C::operator=( const C& other) {
        if ( this != &other ) {
          this->~C();          // lifetime of *this ends
          new (this) C(other); // new object of type C created
          f();                 // well-defined
        }
        return *this;
      }

      void test() {
        C c1;
        C c2;
        c1 = c2; // well-defined
        c1.f();  // well-defined; c1 refers to a new object of type C
      }
    }

    namespace cxx17 {
      //  If the conditions listed in TransparencyReplaceableObject are not met, a valid pointer to the new object
      // may still be obtained by applying the pointer optimization barrier std::launder:

      struct A {
        virtual int transmogrify();
      };

      struct B : A {
        int transmogrify() override {
          ::new(this) A;
          return 2;
        }
      };

      inline int A::transmogrify() {
        ::new(this) B;
        return 1;
      }

      // todo: see https://en.cppreference.com/w/cpp/utility/launder
      void test() {
        A i;
        int n = i.transmogrify();
        // int m = i.transmogrify(); // undefined behavior: the new A object is a base subobject, while the old one is a complete object
        A* pA = std::launder(&i); // A* points to B type
        int m = pA->transmogrify(); // OK
        assert(m + n == 3);
      }
    }

    namespace AccessOutsideOfLifetime {
      //  Before the lifetime of an object has started but after the storage which the object will occupy has been allocated or,
      // after the lifetime of an object has ended and before the storage which the object occupied is reused or released,
      // the following uses of the glvalue expression that identifies that object are undefined:

      // 1. Lvalue to rvalue conversion (e.g. function call to a function that takes a value).
      // 2. Access to a non-static data member or a call to a non-static member function.
      // 3. Binding a reference to a virtual base class subobject.
      // 4. dynamic_cast or typeid expressions.

      // The above rules apply to pointers as well (binding a reference to virtual base is replaced by implicit conversion to a pointer to virtual base),
      // with two additional rules:
      //  5. static_cast of a pointer to storage without an object is only allowed when casting to (possibly cv-qualified) void*.
      //  6. pointers to storage without an object that were cast to possibly cv-qualified void* can only be
      //  static_cast to pointers to possibly cv-qualified char, or possibly cv-qualified unsigned char, or possibly cv-qualified std::byte (since C++17).

      // During construction and destruction, other restrictions apply, see virtual function calls during construction and destruction.
      void test() {

      }
    }

    namespace Notes {
      // todo: see: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1358r0.html#2256

      namespace CWG2256 {
        // The end of lifetime rules are different between:
        //  non-class objects (end of storage duration) and class objects (reverse order of construction):

        struct A {
          int* p;

          ~A() { std::cout << *p; } // undefined behavior since CWG2256: n does not outlive a
          // well-defined until CWG2256: prints 123
        };

        void test() {
          A a;
          int n = 123; // if n did not outlive a, this could have been optimized out (dead store)
          a.p = &n;
          assert(*(a.p) == 123);
        }
      }

      namespace RU007 {
        //  A non-static member of a const-qualified type or a reference type prevents its containing object from
        // being transparently replaceable, which makes std::vector and std::deque hard to implement:

        struct X {
          const int n;
        };

        union U {
          X x;
          float f;
        };

        void test() {
          U u = { { 1 } };
          u.f = 5.f;                          // OK: creates new subobject of 'u'
          X *p = new (&u.x) X {2};         // OK: creates new subobject of 'u'
          assert(p->n == 2);                  // OK
          assert(u.x.n == 2);                 // undefined until RU007: 'u.x' does not name the new subobject
          assert(*std::launder(&u.x.n) == 2); // OK even until RU007
        }
      }
    }


    void test() {
      UB::Test1::test();
      UB::Test2::test();
      TransparencyReplaceableObject::test();
      cxx17::test();
      Notes::CWG2256::test();
      Notes::RU007::test();
    }
  }

  void test() {
    StorageReuse::test();
  };
}
