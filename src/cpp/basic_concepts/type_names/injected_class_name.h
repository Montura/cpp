#pragma once

// The injected-class-name is the name of a class within the scope of said class.
//
//    In a class template, the injected-class-name can be used either as a template name that refers to the current template,
//  or as a class name that refers to the current instantiation.

namespace InjectedClassName {
  // In a class scope, the name of the current class is treated as if it were a public member name; this is called injected-class-name.
  // The point of declaration of the name is immediately following the opening brace of the class definition.
  int X;
  struct X {
    void f() {
      X* p; // OK. X refers to the injected-class-name
//      ::X* q; // Error: name lookup finds a variable name, which hides the struct name
    }
  };

  // injected-class-names are inherited
  //    In the presence of private or protected inheritance, the injected-class-name of an indirect base class
  // might end up being inaccessible in a derived class.
  struct A {};
  struct B : private A {};
  struct C : B {
//    A* p; // Error: injected-class-name A is inaccessible
    ::InjectedClassName::A* q; // OK, does not use the injected-class-name
  };

  // todo: In class template https://en.cppreference.com/w/cpp/language/injected-class-name
}