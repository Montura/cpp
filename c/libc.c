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
}

// 1.5 Character Input and Output
void foo() {
  int c = getchar();
  putchar(c);
}

void test_libc() {
  printf("---------------------- Start testing libc functions ---------------------- \n");
  printf("hello, world\n");
  fahrenheit_celsius_table();
  printf("---------------------- End testing libc functions ---------------------- \n");
}
