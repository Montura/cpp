// The extern specifier is only allowed in the declarations of variables and functions (except class members or function parameters).
// It specifies external linkage, and does not technically affect storage duration, but it cannot be used in a definition
// of an automatic storage duration object, so all extern objects have static or thread durations.
// In addition, a variable declaration that uses extern and has no initializer is not a definition.

#include <stdio.h>

#define MAXLENGTH 5

// variables declarations are NOT extern by default (static -> for using only in this translation unit)
// Define variables in lib.c
extern char extern_longest_line[];
extern char extern_line[];
extern int extern_max_len;

// function declarations are extern by default (static -> for using only in this translation unit)
// use static to avoid conflicts with copy and getline from libc.c
static int getline(void);
static void copy(void);

/* print extern_longest_line input line; specialized version */
static int print_extern_longest_line() {
  int len;
  extern_max_len = 0;
  while ((len = getline()) > 0)
    if (len > extern_max_len) {
      extern_max_len = len;
      copy();
    }
  if (extern_max_len > 0) /* there was a line */
    printf("%s", extern_longest_line);
  return 0;
}

/* getline: specialized version */
int getline(void) {
  int c, i = 0;
  while (i < MAXLENGTH - 1 && ((c = getchar()) != EOF) && c != '\n') {
    extern_line[i++] = (char) c;
  }
  if (c == '\n') {
    extern_line[i] = (char) c;
    ++i;
  }
  extern_line[i] = '\0';
  return i;
}
/* copy: specialized version */
void copy(void) {
  int i;
  i = 0;
  while ((extern_longest_line[i] = extern_line[i]) != '\0') {
    ++i;
  }
}
