#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "common.h"

/* squeeze_symbol: delete all c from s */
void squeeze_symbol(char * s, int c) {
  if (strlen(s) == 0) {
    return;
  }
  int j = 0;
  for (int i = 0; s[i] != 0; ++i) {
    if (s[i] != c) {
      s[j++] = s[i];
    }
  }
  s[j] = '\0';
}

void squeeze_symbols(char s[], char to_delete[]) {
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

void m_strcat(char s[], const char* t) {
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


//    Returns the first location in a string s1 where m_strpbrk character from the string s2 occurs, or -1 if s1 contains no
// characters from s2.
//    (The standard library function strpbrk does the same job but returns a pointer to the location).
int m_strpbrk(const char* s1, const char* s2) {
  for (int i = 0; s1[i] != '\0'; ++i) {
    for (int j = 0; s2[j] != '\0'; ++j) {
      if (s1[i] == s2[j]) {
        return i;
      }
    }
  }

  return -1;
}


void tests_for_squeeze_symbol(const char* s, char c, const char* expected_value) {
  char tmp[MAXLENGTH];
  size_t len = strlen(s);
  memcpy(tmp, s, len);
  tmp[len] = 0;
  squeeze_symbol(tmp, c);
  assert(strcmp(tmp, expected_value) == 0);
}

void tests_for_squeeze_symbols(const char* s, char* to_remove, const char* expected_value) {
  char tmp[MAXLENGTH];
  size_t len = strlen(s);
  memcpy(tmp, s, len);
  tmp[len] = 0;
  squeeze_symbols(tmp, to_remove);
  assert(strcmp(tmp, expected_value) == 0);
}

void tests_for_squeeze() {
  tests_for_squeeze_symbol("", '0', "");
  tests_for_squeeze_symbols("", "", "");
  tests_for_squeeze_symbols("", "0", "");

  tests_for_squeeze_symbol("010101", '0', "111");
  tests_for_squeeze_symbols("010101", "0", "111");
  tests_for_squeeze_symbols("010101", "01", "");

  char* string = "clean*--up-* *this- *str*-ing *f*r* -o-m- s*-ymbols";

  tests_for_squeeze_symbol(string, '*', "clean--up- this- str-ing fr -o-m- s-ymbols");
  tests_for_squeeze_symbol(string, '-', "clean*up* *this *str*ing *f*r* om s*ymbols");
  tests_for_squeeze_symbols(string, "*-", "cleanup this string fr om symbols");
}

void test_strcat(const char* base_str, const char* str_to_append, const char* expected_value) {
  char buf[MAXLENGTH] = "";

  m_strcat(buf, base_str);
  assert(strcmp(buf, base_str) == 0);
  m_strcat(buf, str_to_append);
  assert(strcmp(buf, expected_value) == 0);

  // strcmp returns:
  // - Negative value if lhs appears before rhs in lexicographical order.
  // - Zero if lhs and rhs compare equal.
  // - Positive value if lhs appears after rhs in lexicographical order.
}

void tests_for_strcat() {
  test_strcat("", "", "");
  test_strcat("", "a", "a");
  test_strcat("a", "", "a");
  test_strcat("a", "a", "aa");
  test_strcat("ab", "a", "aba");
  test_strcat("a", "ba", "aba");
  test_strcat("abc", "defg", "abcdefg");
}

void test_m_strpbrk(const char* s1, const char* s2, int expected_value) {
  assert(m_strpbrk(s1, s2) == expected_value);
}

void tests_for_m_strpbrk() {
  test_m_strpbrk("", "", -1);
  test_m_strpbrk("", "a", -1);
  test_m_strpbrk("a", "", -1);
  test_m_strpbrk("a", "a", 0);
  test_m_strpbrk("b", "a", -1);
  test_m_strpbrk("ab", "a", 0);
  test_m_strpbrk("bc", "c", 1);
  test_m_strpbrk("acbc", "c", 1);
  test_m_strpbrk("acbde", "ba", 0);
  test_m_strpbrk("bcbde", "ca", 1);
  test_m_strpbrk("acbde", "fe", 4);
}

void test_increment_decrement() {
  tests_for_squeeze();
  tests_for_strcat();
  tests_for_m_strpbrk();
  printf("Test for '2.8 Increment/decrement' is passed!\n");
}