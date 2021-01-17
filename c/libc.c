//
// The C Programming Language. Brian W. Kernighan Dennis M. Ritchie

#include <stdio.h>

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

void test_libc() {
  printf("---------------------- Start testing libc functions ---------------------- \n");
  printf("hello, world\n");
  fahrenheit_celsius_table();
//  input_output();
//  count_stdin_lines();
//  replace_multiple_spaces();
//  word_count();
//  test_pow();
  print_longest_line();
  printf("---------------------- End testing libc functions ---------------------- \n");
}
