#include <iostream>
#include <array>

namespace Item1_TemplateDeduction {
  template<typename T>
  void fRef(T &param) { std::cout << __PRETTY_FUNCTION__ << std::endl; }

  template<typename T>
  void fCRef(T const &param) { std::cout << __PRETTY_FUNCTION__ << std::endl; }

  template<typename T>
  void fPointer(T *param) { std::cout << __PRETTY_FUNCTION__ << std::endl; }

  template<typename T>
  void fVal(T param) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  template<typename T>
  void f1(T func) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  template<typename T>
  void f2(T &func) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

  /// CASE 1. ParamType is a Reference or Pointer, but not a Universal Reference
  /// 1) If expr’s type is a reference, ignore the reference part.
  /// 2) Then pattern-match expr’s type against ParamType to determine T.
  void deduceTemplateOne() {
    int x = 27;
    int const cx = x;
    int const &rcx = cx;

    fRef(x);   // T - int      , ParamType - int&
    fRef(cx);  // T - int const, ParamType - int const&
    fRef(rcx); // T - int const, ParamType - int const&

    fCRef(x);   // T - int, ParamType - int const&
    fCRef(cx);  // T - int, ParamType - int const&
    fCRef(rcx); // T - int, ParamType - int const&

    int const *px = &x;
    fPointer(&x); // T - int      , ParamType - int*
    fPointer(px); // T - int const, ParamType - int const*
  }


  template<typename T>
  void fUniRef(T &&param) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  };

  /// CASE 2. ParamType is a Universal Reference
  /// 1) If expr is an lvalue, both T and ParamType are deduced to be lvalue references.
  ///    That’s doubly unusual. First, it’s the only situation in template type deduction
  ///    where T is deduced to be a reference. Second, although ParamType is declared
  ///    using the syntax for an rvalue reference, its deduced type is an lvalue reference.
  /// 2) If expr is an rvalue, the “normal” (i.e., Case 1) rules apply.

  void deduceTemplateTwo() {
    int x = 27;
    int const cx = x;
    int const &rcx = cx;

    fUniRef(x);   // х - lvalue,    T - int&      , ParamType - int&
    fUniRef(cx);  // cх - lvalue,   T - int const&, ParamType - int const&
    fUniRef(rcx); // rcx - lvalue,  T - int const&, ParamType - int const&
    fUniRef(27);  // 27 - rvalue,   T - int       , ParamType - int &&
  }

  /// CASE 3. ParamType is Neither a Pointer nor a Reference
  /// 1) As before, if expr’s type is a reference, ignore the reference part.
  /// 2) If, after ignoring expr’s reference-ness, expr is const, ignore that, too. If it’s volatile, also ignore that.

  void deduceTemplateThree() {
    int x = 27;
    int const cx = x;
    int const &rcx = cx;

    fVal(x);   // T - int, ParamType - int
    fVal(cx);  // T - int, ParamType - int
    fVal(rcx); // T - int, ParamType - int

    /// VOLATILE and CONST are ignored only for by-value parameters.
    const char *const ptr = "Fun with pointers";
    fVal(ptr); // T - const char*, ParamType - const char
  }

  typedef void(* SomeFunc)(int, double);

  void checkTypes() {
    SomeFunc f{};
    f1(f); // T - void (*)(int, double) as PTR to func
    f2(f); // T - void (&)(int, double) as REF to func
  }

  void test() {
    deduceTemplateOne();
    deduceTemplateTwo();
    deduceTemplateThree();
    checkTypes();
  }

  /// Things to Remember
  /// - During template type deduction, arguments that are references are treated as non-references,
  ///   i.e., their reference-ness is ignored.
  /// - When deducing types for universal reference parameters, lvalue arguments get special treatment.
  /// - When deducing types for by-value parameters, const and/or volatile arguments are treated as non-const and non-volatile.
  /// - During template type deduction, arguments that are array or function names decay to pointers,
  ///   unless they’re used to initialize references.
}

namespace Item1_ArrayArguments {
  const char name[] = "J. P. Briggs"; // const char[13]
  const char *prtToName = name; // because of the array-to-pointer decay rule, the code compiles.

// the same function declaration
  void myFunc(int param[]);
  void myFunc(int *param);

// return size of an array as a compile-time constant.
// (The array parameter has no name, because we care only about the number of elements it contains.)

  template<typename T, std::size_t N>
  static constexpr std::size_t arraySize(T (&)[N]) {
    return N;
  }

  template<typename T, std::size_t N>
  static std::size_t arraySizeDump(T (&)[N]) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return N;
  }

  static void by_value(const int array[]) {
    std::cout << __PRETTY_FUNCTION__ << ", array_size is: " << sizeof(array) << std::endl;
  }

  static void by_ptr(int const* array) {
    std::cout << __PRETTY_FUNCTION__ << ", array_size is: " << sizeof(array) << std::endl;
  }

  static void by_ref(int (&array)[5]) {
    std::cout << __PRETTY_FUNCTION__ << ", array_size is: " << sizeof(array) << std::endl;
  }

  void array_decay_example() {
    int array[5] = {};
    by_value(array);
    by_ptr(array);
    by_ref(array);
  }

  void test() {
    Item1_TemplateDeduction::fVal(name); // name is array, T - const char*
    Item1_TemplateDeduction::fRef(name); // name is array, T - const char [13]
    // IMPORTANT: Type of Reference to array: const char (&) [13]

    array_decay_example();

    int keys[] = {1, 3, 7, 9, 11, 22, 35};
    size_t i = arraySizeDump(keys);
    int values[arraySize(keys)];
    std::array<int, arraySize(keys)> mappedValues;
    
  }
}
