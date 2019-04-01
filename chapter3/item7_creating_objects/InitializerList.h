#pragma once

using namespace std;

struct ElementA {
  ElementA(int i, bool b) {}; // ctors not declaring
  ElementA(int i, double d) {}; // std::initializer_list params
//  Element(std::initializer_list<long double> il) {}; // added
};

struct ElementB {
  ElementB(int i, bool b) { cout << __PRETTY_FUNCTION__ << endl; };   // ctors not declaring
  ElementB(int i, double d) { cout << __PRETTY_FUNCTION__ << endl; }; // std::initializer_list params
  ElementB(initializer_list<long double> il) { cout << __PRETTY_FUNCTION__ << endl; }; // added

  operator float() const {
    return 1.0;
  } ;
};

struct ElementC {
  ElementC(int i, bool b) { cout << __PRETTY_FUNCTION__ << endl; };   // ctors not declaring
  ElementC(int i, double d) { cout << __PRETTY_FUNCTION__ << endl; }; // std::initializer_list params
  ElementC(initializer_list<bool> il) { cout << __PRETTY_FUNCTION__ << endl; }; // added

//  Here, compilers will ignore the first two constructors (the second of which offers an exact match on both argument types)
//  and try to call the constructor taking a std::initializer_list<bool>.

//  Calling that constructor would require converting an int (10) and a double (5.0) to bools.
//  Both conversions would be narrowing (bool can’t exactly represent either value), and narrowing conversions
//  are prohibited inside braced initializers, so the call is invalid, and the code is rejected.
};

struct ElementD {
  ElementD(int i, bool b) { cout << __PRETTY_FUNCTION__ << endl; };   // ctors not declaring
  ElementD(int i, double d) { cout << __PRETTY_FUNCTION__ << endl; }; // std::initializer_list params
  ElementD(initializer_list<string> il) { cout << __PRETTY_FUNCTION__ << endl; }; // no implicit conversion

  // Only if there’s no way to convert the types of the arguments in a braced initializer to the type in a
  // std::initializer_list do compilers fall back on normal overload resolution.
  //
  // For example, if we replace the std::initializer_list<bool> constructor with std::initializer_list<std::string>,
  // the non std::initializer_list constructors become candidates again,
  // because there is no way to convert ints and bools to std::strings.
};

struct ElementE {
  ElementE() {};
  ElementE(initializer_list<int> il) {};
};
