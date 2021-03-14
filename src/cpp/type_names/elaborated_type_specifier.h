#pragma once


// Elaborated type specifier (https://en.cppreference.com/w/cpp/language/elaborated_type_specifier)
namespace ElaboratedTypeSpecifier {
  namespace PreviouslyDeclaredClass {
    //    These specifiers may be used to refer to a previously-declared class name (class, struct, or union) or
    // to a previously-declared enum name even if the name was hidden by a non-type declaration.
    class T {
    public:
      class U;

    private:
      int U;
    };

    void test() {
      int T;
//  T t; // error: the local variable T is found
      class T t; // OK: finds ::T, the local variable T is ignored
//  T::U* u; // error: lookup of T::U finds the private data member
      class T::U* u; // OK: the data member is ignored
    }
  }

  namespace TypeDeclaration {
    //    If the name lookup does not find a previously declared type name, the elaborated-type-specifier is introduced
    // by class, struct, or union (i.e. not by enum), and class-name is an unqualified identifier,
    // then the elaborated-type-specifier is a class declaration of the class-name.

    template<typename T = int>
    struct Node {
      struct Node* Next; // OK: lookup of Node finds the injected-class-name
      struct Data* Data; // OK: declares type Data at global scope and also declares the data member Data

//    friend class ::List; // error: cannot introduce a qualified name
//    enum Kind* kind; // error: cannot introduce an enum
    };

    void test() {
      Node n;
    }
  }

  namespace InTemplateSpecialization {
    //    If the name refers to a typedef name, a type alias, a template type parameter, or an alias template specialization,
    // the program is ill-formed, otherwise the elaborated type specifier introduces the name into the declaration the same way
    // a simple type specifier introduces its type-name.

    template<typename T>
    class Node {
//      friend class T; // error: type parameter cannot appear in an elaborated type specifier
    };

    class A {
    };

    enum b {
      f, t
    };

    void test() {
      class A a; // OK: equivalent to 'A a;'
      enum b flag; // OK: equivalent to 'b flag;'
    }
  }

  namespace InEnumsAndUnions {
    //  The class-key or enum keyword present in the elaborated-type-specifier must agree in kind with the declaration
    // to which the name in the elaborated-type-specifier refers:
    //  - the enum keyword must be used to refer to an enumeration type (whether scoped or unscoped)
    //  - the union class-key must be used to refer to a union
    //  - either the class or struct class-key must be used to refer to a non-union class type
    //  (the keywords class and struct are interchangeable here).

    enum class E { a, b };

    struct A {};

    union U {
      int u;
    };

    void test() {
      E y = E::b; // OK
      enum E e = E::a; // OK
//    enum class E d = E::b; // error: 'enum class' cannot introduce a elaborated type specifier

      class A a; // OK

      U u; // OK
      union U w = { 1 }; // OK
//      union class U x = { 1 }; // error: 'union class' cannot introduce a elaborated type specifier

      union {
        int x;
      }; // OK

//      union class {
//        int x;
//      }; // error: 'union class' cannot introduce a elaborated type specifier

    }
  }

  void test() {
    PreviouslyDeclaredClass::test();
    TypeDeclaration::test();
    InTemplateSpecialization::test();
    InEnumsAndUnions::test();
  }
}