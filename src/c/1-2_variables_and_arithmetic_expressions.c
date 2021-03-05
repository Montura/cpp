#include <stdio.h>

void fahrenheit_celsius_table() {
  printf(__PRETTY_FUNCTION__);
  printf('\n');

  printf("Fahr\tCels\n");
  for (int fahr = 0; fahr <= 300; fahr += 50) {
    int celsius = 5 * (fahr - 32) / 9;
    printf("%d\t%d\n", fahr, celsius);
  }
  fflush(stdout);
}