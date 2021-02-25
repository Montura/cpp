#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "common.h"

// 1.7 Functions
long long pow_1(int base, int n) {
  long long res = 1;
  for (int i = 1; i <= n; ++i) {
    res *= base;
  }
  return res;
}

// 1.8 Arguments - Call by Value
// passing args by value, so we can rewrite pow and modify N
long long pow_2(int base, int n) {
  long long res = 1;
  while (n--) {
    res *= base;
  }
  return res;
}

void test_pow() {
  double eps = 1e-5;
  for (int base = -10; base < 10; ++base) {
    for (int exponent = 0; exponent < 10; ++exponent) {
      long long int pow1 = pow_1(base, exponent);
      long long int pow2 = pow_2(base, exponent);
      double d = pow(base, exponent);
      assert(fabs(d - pow1) < eps);
      assert(fabs(d - pow2) < eps);
    }
  }
}

// 1.9 Character Arrays

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

void test_functions_and_characters_array() {
  test_pow();
}