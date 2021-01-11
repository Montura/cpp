#include <iostream>
#include <vector>
#include <atomic>

#include "Widget.h"
#include "Elements.h"

namespace Item7_InitializerList {

  void braced_primitive_initialization() {
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

  void ctor_overloading_with_initializer_list() {
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

  void vector_initialization() {
    std::vector<int> v1(10, 20); // non-std::initializer_list ctor: create 10-element std::vector, all elements have value of 20
    std::vector<int> v2{10, 20}; // std::initializer_list ctor: create 2-element std::vector, element values are 10 and 20

    // You need to be aware that if your set of overloaded constructors includes one or more functions taking a
    // std::initializer_list, client code using braced initialization may see only the std::initializer_list overloads.

    // !!! It’s best to design your constructors so that the overload called isn’t affected by whether clients use parentheses or braces.
  }

  template<typename>
  struct is_std_vector : std::false_type {};

  template<typename T, typename A>
  struct is_std_vector<std::vector<T,A>> : std::true_type {};

  template<
      typename T, // type of object to create
      typename... Ts> // types of arguments to use
  void doSomeWork(Ts&&... params) {
    T localObject_parens(std::forward<Ts> (params) ...); // using parens
    T localObject_braces{std::forward<Ts> (params) ...}; // using braces

    // localObject_parens -> for any T calls non-std::initializer_list ctor ()
    // localObject_parens -> for any T calls std::initializer_list ctor ()

    std::cout << (is_std_vector<T>::value ? localObject_parens.size() : 0) << std::endl; // for T - std::vector the result is a std::vector with 10 elements
    std::cout << (is_std_vector<T>::value ? localObject_braces.size() : 0) << std::endl; // the T - std::vector the result is a std::vector with 2 elements
  }

  void templates_initialization() {
    std::vector<int> v;
    doSomeWork<std::vector<int>>(10, 20);
  }

  void test() {
    braced_primitive_initialization();
    ctor_overloading_with_initializer_list();
    templates_initialization();
  }

  /// Things to Remember:
  ///  - Braced initialization is the most widely usable initialization syntax, it prevents narrowing conversions,
  ///    and it’s immune to C++’s most vexing parse.
  ///  - During constructor overload resolution, braced initializers are matched to std::initializer_list
  ///    parameters if at all possible, even if other constructors offer seemingly better matches.
  ///  - An example of where the choice between parentheses and braces can make a significant difference is creating
  ///    a std::vector<numeric type> with two arguments.
  ///  - Choosing between parentheses and braces for object creation inside templates can be challenging.
}