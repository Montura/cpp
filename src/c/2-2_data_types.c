// Byte
//  A byte is the smallest addressable unit of memory.
//  It is defined as a contiguous sequence of bits, large enough to hold any member of the basic execution character set
//  (the 96 characters that are required to be single-byte). C supports bytes of sizes 8 bits and greater.

//  The types char, unsigned char, and signed char use one byte for both storage and value representation.
//  The number of bits in a byte is accessible as CHAR_BIT.

#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
// Memory location
//  A memory location is an object of scalar type (arithmetic type, pointer type, enumeration type) or the largest
//  contiguous sequence of bit fields of non-zero length


// identifier(optional) : width
//   identifier	-	the name of the bit field that is being declared.
//                The name is optional: nameless bit fields introduce the specified number of bits of padding
//    width	-	an integer constant expression with a value greater or equal to zero and less or equal the number of bits
//            in the underlying type. When greater than zero, this is the number of bits that this bit field will occupy.
//            The value zero is only allowed for nameless bit fields and has special meaning: it specifies that the next
//            bit field in the class definition will begin at an allocation unit's boundary.

struct A {
  char a;     // memory location #1
  int b : 3;  // memory location #2
  int c : 3, // memory location #2 (continued)
        : 0,
      d : 3;  // memory location #3
  struct {
      int ee : 2; // memory location #4
  } e;
} obj; // The object 'obj' consists of 4 separate memory locations

// https://en.cppreference.com/w/c/language/bit_field
// Because bit fields do not necessarily begin at the beginning of a byte, address of a bit field cannot be taken.
// Pointers to bit fields are not possible. Bit fields cannot be used with sizeof

struct B {
  // three-bit unsigned field,
  // allowed values are 0...7
  unsigned int b : 3;
};

struct C {
  // will usually occupy 4 bytes:
  // 5 bits: value of b1
  // 11 bits: unused
  // 6 bits: value of b2
  // 2 bits: value of b3
  // 8 bits: unused
  unsigned b1 : 5, : 11, b2 : 6, b3 : 4;
};

struct D {
  // will usually occupy 8 bytes:
  // 5 bits: value of b1
  // 27 bits: unused
  // 6 bits: value of b2
  // 15 bits: value of b3
  // 11 bits: unused
  unsigned b1 : 5;
  unsigned :0; // start a new unsigned int
  unsigned b2 : 6;
  unsigned b3 : 15;
};

// %d print as decimal integer
// %6d print as decimal integer, at least 6 characters wide
// %f print as floating point
// %6f print as floating point, at least 6 characters wide
// %.2f print as floating point, 2 characters after decimal point
// %6.2f print as floating point, at least 6 wide and 2 after decimal point

void test_bitfield(void) {
  struct B d = {7};
  ++d.b; // unsigned overflow
//  printf("Value of B::b = %d\n", d.b); // output: 0
  assert(d.b == 0);
//  printf("Size of struct C = %zu\n",sizeof(struct C)); // usually prints 4
  assert(sizeof(struct C) == 4);
//  printf("Size of struct D = %zu\n",sizeof(struct D)); // usually prints 8
  assert(sizeof(struct D) == 8);

#ifdef _MSC_VER
  printf("Size of struct A = %lld\n", sizeof(obj)); // usually prints 16
  assert(sizeof(obj) == 16);
#else
  printf("Size of struct A = %lu\n", sizeof(obj)); // usually prints 12
  assert(sizeof(obj) == 12);
#endif

//  printf("Size of struct A::e = %lld\n", sizeof(obj.e)); // 4
  assert(sizeof(obj.e) == 4);
}

void print_sizeof_s() {
  // todo: add asserts for platforms __WIN32__, __APPLE__, __LINUX__
  test_bitfield();
  printf("sizeof(char)                = %lld\n", sizeof(char));
  printf("sizeof(signed char)         = %lld\n", sizeof(signed char));
  printf("sizeof(unsigned char)       = %lld\n", sizeof(unsigned char));
  printf("sizeof(short)               = %lld\n", sizeof(short));
  printf("sizeof(unsigned short)      = %lld\n", sizeof(unsigned short));
  printf("sizeof(int)                 = %lld\n", sizeof(int));
  printf("sizeof(unsigned int)        = %lld\n", sizeof(unsigned int));
  printf("sizeof(long)                = %lld\n", sizeof(long));
  printf("sizeof(unsigned long)       = %lld\n", sizeof(unsigned long));
  printf("sizeof(long long)           = %lld\n", sizeof(long long));
  printf("sizeof(unsigned long long)  = %lld\n", sizeof(unsigned long long));
  printf("sizeof(float)               = %lld\n", sizeof(float));
  printf("sizeof(double)              = %lld\n", sizeof(double));
  printf("sizeof(long double)         = %lld\n", sizeof(long double));
}

void print_limits() {
  printf("The number of bits in a byte is accessible as %d\n", CHAR_BIT);
  printf("char                 limits: [%d, %d]\n", CHAR_MIN, CHAR_MAX);
  printf("unsigned char        limits: [%d, %d]\n", 0, UCHAR_MAX);
  printf("short                limits: [%d, %d]\n", SHRT_MIN, SCHAR_MAX);
  printf("unsigned short       limits: [%d, %d]\n", 0, USHRT_MAX);
  printf("int                  limits: [%d, %d]\n", INT_MIN, INT_MAX);
  printf("unsigned int         limits: [%d, %u]\n", 0, UINT_MAX);
  printf("long                 limits: [%ld, %ld]\n", LONG_MIN, LONG_MAX);
  printf("unsigned long        limits: [%d, %lu]\n", 0, ULONG_MAX);
  printf("long long            limits: [%lld, %lld]\n", LLONG_MIN, LLONG_MAX);
  printf("unsigned long long   limits: [%d, %llu]\n", 0, ULLONG_MAX);
  printf("float                limits: [%E, %E]\n", -FLT_MAX, FLT_MAX);
  printf("double               limits: [%E, %E]\n", -DBL_MAX, DBL_MAX);
  printf("long double          limits: [%E, %E]\n", -DBL_MAX, DBL_MAX);
}


void test_data_types() {
  test_bitfield();
  print_sizeof_s();
  print_limits();
  printf("Test for '2.2 Data types' is passed!\n");
}