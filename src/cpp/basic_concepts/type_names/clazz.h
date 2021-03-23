#pragma once
// User-defined types, defined by class-specifier

namespace Clazz {

  namespace ForwardDeclaration {
    class Vector; // forward declaration
    class Matrix {
      // ...
      friend Vector operator*(const Matrix&, const Vector&);
    };

    class Vector {
      // ...
      friend Vector operator*(const Matrix&, const Vector&);
    };

    struct s {
      int a;
    };
    struct s; // does nothing (s already defined in this scope)

    void test() {
      struct s; // forward declaration of a new, local struct "s"
      // this hides global struct s until the end of this block
      s* p;     // pointer to local struct s
      struct s {
        char* p;
      }; // definitions of the local struct s
    }
  }


  void test() {
    ForwardDeclaration::test();
  }

  // todo:
  //  https://en.cppreference.com/w/cpp/language/class
  //  Member specification
  //  Local classes
}