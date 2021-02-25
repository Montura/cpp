//
// The C Programming Language. Brian W. Kernighan Dennis M. Ritchie

#include <stdio.h>
#include <assert.h>

void print_sizeof_s();
void print_limits();
void test_enums();

// %d print as decimal integer
// %6d print as decimal integer, at least 6 characters wide
// %f print as floating point
// %6f print as floating point, at least 6 characters wide
// %.2f print as floating point, 2 characters after decimal point
// %6.2f print as floating point, at least 6 wide and 2 after decimal point

void fahrenheit_celsius_table () {
  printf(__PRETTY_FUNCTION__);
  printf("\n");

  printf("Fahr\tCels\n");
  for (int fahr = 0; fahr <= 300; fahr += 20) {
    int celsius = 5 * (fahr - 32) / 9;
    printf("%d\t%d\n", fahr, celsius);
  }
  fflush(stdout);
}

// 1.5 Character Input and Output
void input_output() {
  int c = getchar();
  printf("EOF = %d", EOF);
  while (c != EOF) {
//    printf("c = %d\n", c);
    putchar(c);
    c = getchar();
    fflush(stdout);
  }
}

void count_stdin_lines() {
  int c;
  long lines_count = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      ++lines_count;
    }
  }
  printf("Total lines in stdin: %ld\n", lines_count);
  fflush(stdout);
}

void replace_multiple_spaces() {
  int c;
  int prev_symbol = 0;
  while ((c = getchar()) != EOF) {
    if ((prev_symbol == c) && (c == ' ')) {
      continue;
    } else {
      putchar(c);
      fflush(stdout);
    }
    prev_symbol = c;
  }
}

// UNIX wc
#define OUT 0
#define IN 1
void word_count() {
  int lines_count = 0, words_count = 0, symbols_count = 0;
  int state = OUT;
  int c;
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      ++lines_count;
    }
    if (c == '\n' || c == ' ' || c == '\t') {
      state = OUT;
    } else if (state == OUT) {
      state = IN;
      ++words_count;
    }
    ++symbols_count; // including \n, \t
  }
  printf("Lines: %d, words: %d, symbols: %d\n", lines_count, words_count, symbols_count);
}

long long pow_1(int base, int n) {
  long long res = 1;
  for (int i = 1; i <= n; ++i) {
    res *= base;
  }
  return res;
}

// passing args by value, so we can rewrite pow and modify N
long long pow_2(int base, int n) {
  long long res = 1;
  while (n--) {
    res *= base;
  }
  return res;
}

void test_pow() {
  for (int i = 0; i < 10; ++i) {
    printf("base = %d, n = %d, pow = %lld, pow_2 = %lld\n", 2, i, pow_1(2, i), pow_2(2, i));
  }
  for (int i = 0; i < 10; ++i) {
    printf("base = %d, n = %d, pow = %lld, pow_2 = %lld\n", -3, i, pow_1(-3, i), pow_2(-3, i));
  }
}

// 1.9 Characters array
#define MAXLENGTH 100

void copy(char src[MAXLENGTH], char dst[MAXLENGTH]) {
  int i = 0;
  while ((dst[i] = src[i]) != 0) {
    ++i;
  }
}

// returns len
int getline_1(char line[], int max_length) {
  int i = 0, c = 0;

  while (i < max_length - 1 && ((c = getchar()) != EOF && c != '\n')) {
    line[i++] = (char) c;
  }

  if (c == '\n') {
    line[i++] = (char) c;
  }
  line[i] = '\0';
  return i;
}

void print_longest_line() {
  char line[MAXLENGTH];
  char longest_line[MAXLENGTH];
  char chunk[MAXLENGTH];

  int curr_len = 0, max_len = 0, tmp_len = 0;
  int chunk_len = MAXLENGTH - 1;

  while ((curr_len = getline_1(line, MAXLENGTH)) > 0) {
    // input line is longer than buffer
    if (curr_len == chunk_len) {
      line[chunk_len] = '\n';
      tmp_len = curr_len;
      while (tmp_len == chunk_len) {
        tmp_len = getline_1(chunk, MAXLENGTH);
        curr_len += tmp_len;
      }
    }
    if (curr_len > max_len) {
      max_len = curr_len;
      copy(line, longest_line);
    }
  }

  if (max_len > 0) {
    printf("%s, len = %d", longest_line, max_len);
  }
}
// 1.10 External Variables and Scope
// Declare extern variables for extern_keyword.c
char extern_longest_line[MAXLENGTH];
char extern_line[MAXLENGTH];
int extern_max_len = 0;

// 2.3 Constants
int m_strlen(const char* str) {
  int i = 0;

  while (str[i++] != 0);

  return i > 0 ? i - 1 : 0;
}

void test_m_strlen() {
  const char *s = "";
  printf("Len of \"%s\" is %d\n", s, m_strlen(s));

  const char *s1 = "a";
  printf("Len of \"%s\" is %d\n", s1, m_strlen(s1));

  const char *s2 = "Hello";
  printf("Len of \"%s\" is %d\n", s2, m_strlen(s2));

  const char *s3 = "Hello ";
  printf("Len of \"%s\" is %d\n", s3, m_strlen(s3));

  const char *s4 = "Hello world";
  printf("Len of \"%s\" is %d\n", s4, m_strlen(s4));
}

// 2.4 Declarations

//  1. If the variable in question is not automatic, the initialization is done once only;
//  2. An explicitly initialized automatic variable is initialized each time the function or block it is in is entered;
//  3. External and static variables are initialized to zero by default.
//  4. Automatic variables for which is no explicit initializer have undefined (i.e., garbage) values.

// 2.7 Type Conversions
#include <ctype.h>
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
  if (s[0] != '0' && s[1] != 'x') {
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

// - C guarantees that any character in the machine's standard printing character set will never be negative,
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

// 2.8 Increment and Decrement Operators

/* squeeze: delete all c from s */
void squeeze(char s[], int c) {
  int j = 0;
  for (int i = 0; s[i] != 0; ++i) {
    if (s[i] != c) {
      s[j++] = s[i];
    }
  }
  s[j] = '\0';
}

void squeeze_2(char s[], char to_delete[]) {
  int j = 0, k = 0;
  for (int i = 0; s[i] != 0; ++i) {
    k = 0;
    while ((to_delete[k] != 0) && (s[i] != to_delete[k])) {
      ++k;
    }

    if (to_delete[k] == '\0') {
      s[j++] = s[i];
    }
  }

  s[j] = '\0';
}

void m_strcat(char s[], char t[]) {
  int len = m_strlen(s);

  for (int i = len, j = 0; t[j] != '\0';  ++i, ++j) {
    s[i] = t[j];
  }

  {
    // Ex. with increment and decrement
//    int i = 0, j = 0;
//    while (s[i++] != '0');
//    while ((s[i++] = t[j++]) != '0');
  }
}


void test_squeeze() {
  char string1[MAXLENGTH] = { "clean*--up-* *this- *str*-ing *f*r* -o-m- s*-imbols" };
  char copy_string1[MAXLENGTH];
  copy(string1, copy_string1);
  char string2[MAXLENGTH] = { "*-" };
  char c = '-';
  char x = '*';

  printf("%s before removing %c\n", string1, c);
  squeeze(string1, c);
  printf("%s after removing %c\n", string1, c);

  printf("%s before removing %c\n", string1, x);
  squeeze(string1, x);
  printf("%s after removing %c\n", string1, x);

  printf("%s before removing \'%s\'\n", copy_string1, string2);
  squeeze_2(copy_string1, string2);
  printf("%s after removing \'%s\'\n", copy_string1, string2);
}

void test_strcat() {
  char hello[MAXLENGTH] = "Hello";
  char world[MAXLENGTH] = "world!";

  m_strcat(hello, world);
  printf("\'%s\' after strcat \'hello\' and \'world\'\n", hello);
}

//    Returns the first location in a string s1 where any character from the string s2 occurs, or -1 if s1 contains no
// characters from s2.
//    (The standard library function strpbrk does the same job but returns a pointer to the location).
int any(char s1[], char s2[]) {
  for (int i = 0; s1[i] != '\0'; ++i) {
    for (int j = 0; s2[j] != '\0'; ++j) {
      if (s1[i] == s2[j]) {
        return i;
      }
    }
  }

  return -1;
}

void print_binary(unsigned int n) {
  int bits_count = sizeof(n) * 8;
  printf("Total %d bits count for %u:\n\t ", bits_count, n);
  for (int i = bits_count - 1 ; i >= 0; i--) {
    printf("%x", (n & (1 << i)) >> i);
    if (i % 8 == 0) {
      printf(" ");
    }
  }
  printf("\n");
}

int n_bits(int n) {
  return ~(~0 << n);
}

// 2.9 Bitwise Operators
// get n bits from position p
// getbits(x, 4, 3) returns 3 bits in positions 4, 3, 2 right adjusted;
unsigned int getbits(unsigned int x, int p, int n) {
  unsigned int right_adjusted = x >> (p + 1 - n);
  return right_adjusted & n_bits(n);
}

void test_getbits(unsigned int x, int pos, int n, unsigned int expected_value) {
  unsigned int bits = getbits(x, pos, n);
//  printf("Run getbits for %u, %d, %d. Expected = %d, actual = %d\n", x, pos, n, expected_value, bits);
  assert(bits == expected_value);
}

// Exercise 2-6.
// returns x with the n bits that begin at position p set to the rightmost n bits of y, leaving the other bits unchanged.
unsigned int setbits(unsigned int x, int p, int n, unsigned int y) {
  int bits = n_bits(n);
  unsigned int new_bits = (y & bits);
  // x &= ~(1 << n); // clearing n-th bit
  return (x & ~(bits << p)) | (new_bits << p);
}

void test_setbits(unsigned int x, int pos, int n, int y, unsigned int expected_value) {
  unsigned int bits = setbits(x, pos, n, y);
//  printf("Run setbits for x = %u, pos = %d, n = %d, y = %d. Expected = %d, actual = %d\n",
//         x, pos, n, y, expected_value, bits);
  assert(bits == expected_value);
}

// returns x with the n bits that begin at position p inverted, leaving the others unchanged.
unsigned int invert(unsigned int x, int p, int n) {
  int bits = n_bits(n);
  return x ^ (bits << p);
}

void test_invert(unsigned int x, int pos, int n, unsigned int expected_value) {
  unsigned int bits = invert(x, pos, n);
//  print_binary(x);
//  print_binary(bits);
  assert(bits == expected_value);
}

// returns the value of the integer x rotated to the right by n positions.
unsigned int rightrot(unsigned int x, int n) {

}

void test_getbits_1() {
  test_getbits(0, 0, 0, 0);
  test_getbits(1, 0, 0, 0);
  test_getbits(2, 0, 0, 0);

  test_getbits(1, 1, 0, 0);
  test_getbits(1, 2, 0, 0);
  test_getbits(1, 3, 0, 0);

  test_getbits(2, 1, 0, 0);
  test_getbits(2, 2, 0, 0);
  test_getbits(2, 3, 0, 0);

  test_getbits(7, 0, 1, 1);
  test_getbits(7, 1, 1, 1);
  test_getbits(7, 1, 2, 3);
  test_getbits(7, 2, 1, 1);
  test_getbits(7, 2, 2, 3);
  test_getbits(7, 2, 3, 7);
  test_getbits(7, 3, 1, 0);
  test_getbits(7, 3, 2, 1);
  test_getbits(7, 3, 3, 3);
  test_getbits(7, 3, 4, 7);
}

void test_setbits_1() {
  test_setbits(0, 0, 0, 0, 0);

  test_setbits(1, 0, 0, 0, 1);
  test_setbits(1, 0, 0, 1, 1);
  test_setbits(1, 0, 1, 0, 0);
  test_setbits(1, 0, 1, 1, 1);
  test_setbits(1, 1, 0, 0, 1);
  test_setbits(1, 1, 0, 1, 1);
  test_setbits(1, 1, 1, 0, 1);
  test_setbits(1, 1, 1, 1, 3);

  test_setbits(2, 0, 0, 0, 2);
  test_setbits(2, 0, 0, 1, 2);
  test_setbits(2, 0, 1, 0, 2);
  test_setbits(2, 0, 1, 1, 3);
  test_setbits(2, 1, 0, 0, 2);
  test_setbits(2, 1, 0, 1, 2);
  test_setbits(2, 1, 1, 0, 0);
  test_setbits(2, 1, 1, 1, 2);

  test_setbits(15, 0, 1, 2, 14);
  test_setbits(15, 0, 1, 2, 14);
  test_setbits(14, 0, 1, 3, 15);
  test_setbits(13, 0, 2, 3, 15);
  test_setbits(9, 1, 2, 3, 15);
  test_setbits(1, 1, 3, 7, 15);
}

void test_invert_1() {
  test_invert(0, 0, 1, 1);
  test_invert(1, 0, 1, 0);


  test_invert(2, 0, 1, 3);
  test_invert(2, 0, 2, 1);
  test_invert(2, 1, 1, 0);

  test_invert(7, 0, 1, 6);
  test_invert(7, 0, 2, 4);
  test_invert(7, 0, 3, 0);
  test_invert(7, 1, 1, 5);
  test_invert(7, 1, 2, 1);
  test_invert(7, 2, 1, 3);

  test_invert(15, 0, 1, 14);
  test_invert(15, 0, 2, 12);
  test_invert(15, 0, 3, 8);
  test_invert(15, 0, 4, 0);
  test_invert(15, 1, 1, 13);
  test_invert(15, 1, 2, 9);
  test_invert(15, 1, 3, 1);
  test_invert(15, 2, 1, 11);
  test_invert(15, 2, 2, 3);
  test_invert(15, 3, 1, 7);
}

void test_libc() {
  printf("---------------------- Start testing libc functions ---------------------- \n");
  printf("hello, world\n");
  fahrenheit_celsius_table();
//  input_output();
//  count_stdin_lines();
//  replace_multiple_spaces();
//  word_count();
//  test_pow();
//  print_longest_line();
  print_sizeof_s();
  print_limits();
  test_m_strlen();
  test_enums();
  type_conversions();
  test_squeeze();
  test_strcat();
  test_getbits_1();
  test_setbits_1();
  test_invert_1();
  printf("---------------------- End testing libc functions ---------------------- \n");
}
