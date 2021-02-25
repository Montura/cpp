#include <stdio.h>

// 1.5 Character Input and Output
void copy_input_to_output() {
  int c = getchar();
//  printf("EOF = %d", EOF);
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


void test_input_output() {
  //todo: implement
}