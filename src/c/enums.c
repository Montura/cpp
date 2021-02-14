#include <stdio.h>

// Error: Unlike struct or union, there are no forward-declared enums in C:
// Forward references to 'enum' types are a Microsoft extension
#if _WINDOWS
  enum Boolean;
#endif
enum Boolean { NO, YES };

// Names in different enumerations must be distinct. Values need not be distinct in the same enumeration.
// YES - Error: must be distinct!!!!
enum Integer { ONE = 1, TWO = 1, THREE = 1, /*YES*/ };

// Each enumerated type is compatible with one of:
// - char
// - signed integer
// - unsigned integer
enum Escapes { BELL = '\a', BACKSPACE = '\b', TAB = '\t',
  NEWLINE = '\n', VTAB = '\v', RETURN = '\r' };

enum Months { JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC } months;

void test_enums() {
  printf("Size of enum boolean: %llu\n", sizeof(enum Boolean));
  printf("Size of espaces boolean: %llu\n", sizeof(enum Escapes));
  printf("Size of months boolean: %llu\n", sizeof(enum Months));

  enum color { RED, GREEN, BLUE } r = RED;
  switch (r) {
    case RED:
      puts("red");
      break;
    case GREEN:
      puts("green");
      break;
    case BLUE:
      puts("blue");
      break;
  }

  for (int i = JAN; i <= DEC; ++i) {
    printf("%d\n", months + i);
  }
}

// Enumerations provide (vs #difine):
// 1. A convenient way to associate constant values with names
// 2. An alternative to #define with the advantage that the values can be generated for you.
// 3. A debugger may be able to print values of enumeration variables in their symbolic form.