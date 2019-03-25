#include "Widget.h"

#include <iostream>
#include <vector>
#include <atomic>
#include "InitializerList.h"

struct Point {
  int x{ 0 }; // fine, x's default value is 0
  int y = 0; // also fine
//  int z(0); // error!
};

static void foo() {
  int x(0);       // initializer is in parentheses
  int y = 0;      // initializer follows "="
  int z{ 0 };     // initializer is in braces
  int w = { 0 };  // initializer uses "=" and braces

  Widget w1;      // call default constructor
  Widget w2 = w1; // not an assignment; calls copy ctor
  w1 = w2;        // an assignment; calls copy operator=

  // C++11 introduces uniform initialization:
  //    A single initialization syntax that can, at least in concept, be used anywhere and express everything.
  //    It’s based on braces, and for that reason I prefer the term braced initialization.
  //    “Uniform initialization” is an idea. “Braced initialization” is a syntactic construct.

  std::vector<int> v{ 1, 3, 5 }; // v's initial content is 1, 3, 5

  std::atomic<int> ai1{ 0 };  // fine
  std::atomic<int> ai3(0);    // fine
//  std::atomic<int> ai2 = 0;   // error


  // A novel feature of braced initialization is that it prohibits implicit narrowing conversions among built-in types.
  double a{ 1.2 }, b{ 1.3 }, c{ 1.5 };
//  int sum{a + b + c};   // error! sum of doubles may not be expressible as int
  int sum1(a + b + c);  // fine! okay (value of expression truncated to an int)
  int sum2 = a + b + c; // fine! okay (value of expression truncated to an int)

  // Another noteworthy characteristic of braced initialization is its immunity to C++’s most vexing parse.
  // A side effect of C++’s rule that anything that can be parsed as a declaration must be interpreted as one,
  // the most vexing parse most frequently afflicts developers when they want to default-construct an object,
  // but inadvertently end up declaring a function instead.

  Widget w3(10); // call Widget ctor with argument 10
  Widget w4(); // most vexing parse! declares a function named w4 that returns a Widget!

  // Functions can’t be declared using braces for the parameter list,
  // so default constructing an object using braces doesn’t have this problem:
  Widget w5{}; // calls Widget constructor with no args
}



// Relationship among braced initializers, std::initializer_lists, and constructor overload resolution.

void bracedInitializer() {
  ElementA a1(10, true);   // calls first ctor
  ElementA a2{10, true};   // also calls first ctor
  ElementA a3(10, 5.0);    // calls second ctor
  ElementA a4{10, 5.0};    // also calls second ctor

  // If, however, one or more constructors declare a parameter of type std::initializer_list,
  // calls using the braced initialization syntax strongly prefer the overloads taking std::initializer_lists.

  ElementB b1(10, true);  // uses parens and, as before, calls first ctor
  ElementB b2{10, true};  // uses braces, but now calls std::initializer_list ctor(10 and true convert to long double)
  ElementB b3(10, 5.0);   // uses parens and, as before, calls second ctor
  ElementB b4{10, 5.0};   // uses braces, but now calls std::initializer_list ctor(10 and 5.0 convert to long double)

  // Even what would normally be copy and move construction can be hijacked by std::initializer_list constructors:
  ElementB b5(b4);            // uses parens, calls copy ctor
  ElementB b6{b4};            // uses braces, calls std::initializer_list ctor(w4 converts to float, and float converts to long double)
  ElementB b7(std::move(b4)); // uses parens, calls move ctor
  ElementB b8{std::move(b4)}; // uses braces, calls std::initializer_list ctor(for same reason as w6)


  ElementC c1(10, true);  // uses parens and, as before, calls first ctor
//  ElementC c2{10, true};  // uses braces, but cause an error! Requires narrowing conversions!!!
  ElementC c3(10, 5.0);   // uses parens and, as before, calls second ctor
//  ElementC c4{10, 5.0};   // uses braces, but cause an error! Requires narrowing conversions!!!

  ElementD d1(10, true);  // uses parens and, as before, calls first ctor
  ElementD d2{10, true};  // uses braces, now calls first ctor
  ElementD d3(10, 5.0);   // uses parens and, as before, calls second ctor
  ElementD d4{10, 5.0};   // uses braces, now calls second ctor

  ElementE e1;   // calls default ctor
  ElementE e2{}; // also calls default ctor
  ElementE e3(); // most vexing parse! declares a function!

  // To call a std::initializer_list constructor with an empty std::initializer_list,
  // you do it by making the empty braces a constructor argument -
  // by putting the empty braces inside the parentheses or braces demarcating what you’re passing:
  ElementE e4({}); // calls std::initializer_list ctor with empty list
  ElementE e5{{}}; // calls std::initializer_list ctor with empty list
}