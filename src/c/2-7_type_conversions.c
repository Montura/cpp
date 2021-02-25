#include <ctype.h>
#include <stdio.h>
#include <assert.h>

#include "common.h"

int m_atoi(const char* s) {
  if (m_strlen(s) == 0) {
    return 0;
  }
  int res = 0;
//  for (int i = 0; (s[i] >= '0') && (s[i] <= '9'); ++i) {
  for (int i = 0; isdigit(s[i]); ++i) {
    res = 10 * res + (s[i] - '0');
  }
  return res;
}

int htoi(const char* s) {
  int res = 0;
  if (m_strlen(s) == 0) {
    return 0;
  }
  if (s[0] != '0' || s[1] != 'x') {
    return 0;
  }
  int mul, c;
  for (int i = 2; ; ++i) {
    c = tolower(s[i]);
    if (isdigit(c)) {
      mul = c - '0';
    } else if (c == 'a') {
      mul = 10;
    } else if (c == 'b') {
      mul = 11;
    } else if (c == 'c') {
      mul = 12;
    } else if (c == 'd') {
      mul = 13;
    } else if (c == 'e') {
      mul = 14;
    } else if (c == 'f') {
      mul = 15;
    } else {
      break;
    }
    res = 16 * res + mul;
  }
  return res;
}

// - C guarantees that m_strpbrk character in the machine's standard printing character set will never be negative,
// so these characters will always be positive quantities in expressions.
// - The arbitrary bit patterns stored in character variables may appear to be negative on some machines, yet positive on others.
// - For portability, specify signed or unsigned if non-character data is to be stored in char variables.

// If a binary operator has operands of different types:
// - the ``lower'' type is promoted to the ``higher'' type before the operation proceeds
// - The result is of the integer type

// https://en.cppreference.com/w/c/language/conversion#Usual_arithmetic_conversions
// https://en.cppreference.com/w/c/language/conversion#Integer_promotions

void type_conversions() {
  float f_res = 1.f + 20000001; // int is converted to float, giving 20000000.00
// addition and then rounding to float gives 20000000.00
  printf("----------------------------------------------\n");
  printf("Before type promotion  |  After type promotion\n");
  printf("----------------------------------------------\n");
  printf("  1.f + 20000001       |    %.3f + %.3f = %.3f\n", 1.f, (float)20000001, f_res);


  int i_res = (char)'a' + 1; // first, char 'a', which is 97, is promoted to int
// different types: int and long
// same signedness: both signed
// different rank: long is of greater rank than int
// therefore, int 97 is converted to long 97
// the result is 97 + 1 = 98 of type signed long
  printf("  (char)'a' + 1L       |    %d + %d = %d\n", (char)'a', 1, i_res);


  unsigned int ui_res = 2u - 10; // different types: unsigned int and signed int
// different signedness
// same rank
// therefore, signed int 10 is converted to unsigned int 10
// since the arithmetic operation is performed for unsigned integers (see "Arithmetic operators" topic),
// the calculation performed is (2 - 10) modulo (2 raised to n), where n is the number of bits of unsigned int
// if unsigned int is 32-bit long, then
// the result is (-8) modulo (2 raised to 32) = 4294967288 of type unsigned int
  printf("     2u - 10           |    %u - %u = %u\n", 2u, 10, ui_res);

  unsigned long long int ull_res = 5UL - 2ULL; // different types: unsigned long and unsigned long long
// same signedness
// different rank: rank of unsigned long long is greater
// therefore, unsigned long 5 is converted to unsigned long long 5
// since the arithmetic operation is performed for unsigned integers (see "Arithmetic operators" topic),
// if unsigned long long is 64-bit long, then
// the result is (5 - 2) modulo (2 raised to 64) = 3 of type unsigned long long
  printf("    5UL - 2ULL         |    %llu - %llu = %llu\n", 5UL, 2ULL, ull_res);


  ull_res = 0U - 1LL; // different types: unsigned long and signed long long
// different signedness
// different rank: rank of signed long long is greater.
// if ULONG_MAX > LLONG_MAX, then signed long long cannot represent all unsigned long
// therefore, this is the last case: both operands are converted to unsigned long long
// unsigned long 0 is converted to unsigned long long 0
// long long 1 is converted to unsigned long long 1
// since the arithmetic operation is performed for unsigned integers (see "Arithmetic operators" topic),
// if unsigned long long is 64-bit long, then
// the calculation is (0 - 1) modulo (2 raised to 64)
// thus, the result is 18446744073709551615 (ULLONG_MAX) of type unsigned long long
  printf("    0U - 1LL           |    %llu - %llu = %llu\n", 0U, 1LL, ull_res);


//  For example, suppose that:
//  - int       is 32 bits
//  - long long is 64 bits
  char res = -1LL < 1U; // true, because 1U, which is an unsigned int, is promoted to a signed long long.
  printf("   -1LL < 1U           |    %lld < %lld = %d\n", -1LL, 1U, res);
  res = -1 > 1U; // true, because -1L, which is an unsigned int promoted to unsigned int.
  printf("   -1L > 1U            |    %u > %u = %d\n", -1, 1U, res);
  printf("----------------------------------------------\n");
}

void tests_for_m_atoi() {
  assert(m_atoi("") == 0);
  assert(m_atoi("01") == 1);
  assert(m_atoi("1") == 1);
  assert(m_atoi("10") == 10);
  assert(m_atoi("10e") == 10);
  assert(m_atoi("1e0e") == 1);
  assert(m_atoi("0.7") == 0);
  assert(m_atoi("12345") == 12345);
}

void tests_for_htoi() {
  assert(htoi("") == 0);
  assert(htoi("1") == 0);
  assert(htoi("0x01") == 1);
  assert(htoi("0x1") == 1);
  assert(htoi("0x10") == 16);
  assert(htoi("0xA") == 10);
  assert(htoi("0xAB") == 171);
  assert(htoi("0xFFFF") == (15 * (4096 + 256 + 16 + 1)));
  assert(htoi("0.7") == 0);
  assert(htoi("0x1234") == (1 * 4096 + 2 * 256 + 3 * 16 + 4));
}

void test_type_conversions() {
  type_conversions();
  tests_for_m_atoi();
  tests_for_htoi();

  printf("Test for '2.7 Type conversions' is passed!\n");
}

unsigned long int next = 1;
/* rand: return pseudo-random integer on 0..32767 */
int rand(void) {
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

/* srand: set seed for rand() */
void srand(unsigned int seed) {
  next = seed;
}