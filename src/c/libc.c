//
// The C Programming Language. Brian W. Kernighan Dennis M. Ritchie

#include <stdio.h>
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
#define MAXLENGTH 5

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
  printf("---------------------- End testing libc functions ---------------------- \n");
}
