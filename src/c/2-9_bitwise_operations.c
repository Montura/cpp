#include <stdio.h>
#include <assert.h>

// 2.9 Bitwise Operators

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

// get n bits from position p
// getbits(x, 4, 3) returns 3 bits in positions 4, 3, 2 right adjusted;
unsigned int getbits(unsigned int x, int p, int n) {
  unsigned int right_adjusted = x >> (p + 1 - n);
  return right_adjusted & n_bits(n);
}

// Exercise 2-6.
// returns x with the n bits that begin at position p set to the rightmost n bits of y, leaving the other bits unchanged.
unsigned int setbits(unsigned int x, int p, int n, unsigned int y) {
  int bits = n_bits(n);
  unsigned int new_bits = (y & bits);
  // x &= ~(1 << n); // clearing n-th bit
  return (x & ~(bits << p)) | (new_bits << p);
}

// Exercise 2-7.
// returns x with the n bits that begin at position p inverted, leaving the others unchanged.
unsigned int invert(unsigned int x, int p, int n) {
  int bits = n_bits(n);
  return x ^ (bits << p);
}

// Exercise 2-8.
// returns the value of the integer x rotated to the right by n positions.
unsigned int right_rot(unsigned int x, int n) {
  int nbits = sizeof(int) * 8 - 1;
  int left_shift = nbits & -n; // number of bits to rotate in left direction
  return (x << left_shift) | (x >> n);
  // Rotate to the left by n positions (x << n) | (x >> (-n & 31))
}

// -------------- Tests --------------
void test_get_bits(unsigned int x, int pos, int n, unsigned int expected_value) {
  unsigned int bits = getbits(x, pos, n);
//  printf("Run getbits for %u, %d, %d. Expected = %d, actual = %d\n", x, pos, n, expected_value, bits);
  assert(bits == expected_value);
}

void test_set_bits(unsigned int x, int pos, int n, int y, unsigned int expected_value) {
  unsigned int bits = setbits(x, pos, n, y);
//  printf("Run setbits for x = %u, pos = %d, n = %d, y = %d. Expected = %d, actual = %d\n",
//         x, pos, n, y, expected_value, bits);
  assert(bits == expected_value);
}

void test_invert(unsigned int x, int pos, int n, unsigned int expected_value) {
  unsigned int bits = invert(x, pos, n);
//  print_binary(x);
//  print_binary(bits);
  assert(bits == expected_value);
}

void test_right_rot(unsigned int x, int n, unsigned int expected_value) {
  unsigned int bits = right_rot(x, n);
//  print_binary(x);
//  print_binary(bits);
  assert(bits == expected_value);
}

void test_for_get_bits() {
  test_get_bits(0, 0, 0, 0);
  test_get_bits(1, 0, 0, 0);
  test_get_bits(2, 0, 0, 0);

  test_get_bits(1, 1, 0, 0);
  test_get_bits(1, 2, 0, 0);
  test_get_bits(1, 3, 0, 0);

  test_get_bits(2, 1, 0, 0);
  test_get_bits(2, 2, 0, 0);
  test_get_bits(2, 3, 0, 0);

  test_get_bits(7, 0, 1, 1);
  test_get_bits(7, 1, 1, 1);
  test_get_bits(7, 1, 2, 3);
  test_get_bits(7, 2, 1, 1);
  test_get_bits(7, 2, 2, 3);
  test_get_bits(7, 2, 3, 7);
  test_get_bits(7, 3, 1, 0);
  test_get_bits(7, 3, 2, 1);
  test_get_bits(7, 3, 3, 3);
  test_get_bits(7, 3, 4, 7);
}

void tests_for_set_bits() {
  test_set_bits(0, 0, 0, 0, 0);

  test_set_bits(1, 0, 0, 0, 1);
  test_set_bits(1, 0, 0, 1, 1);
  test_set_bits(1, 0, 1, 0, 0);
  test_set_bits(1, 0, 1, 1, 1);
  test_set_bits(1, 1, 0, 0, 1);
  test_set_bits(1, 1, 0, 1, 1);
  test_set_bits(1, 1, 1, 0, 1);
  test_set_bits(1, 1, 1, 1, 3);

  test_set_bits(2, 0, 0, 0, 2);
  test_set_bits(2, 0, 0, 1, 2);
  test_set_bits(2, 0, 1, 0, 2);
  test_set_bits(2, 0, 1, 1, 3);
  test_set_bits(2, 1, 0, 0, 2);
  test_set_bits(2, 1, 0, 1, 2);
  test_set_bits(2, 1, 1, 0, 0);
  test_set_bits(2, 1, 1, 1, 2);

  test_set_bits(15, 0, 1, 2, 14);
  test_set_bits(15, 0, 1, 2, 14);
  test_set_bits(14, 0, 1, 3, 15);
  test_set_bits(13, 0, 2, 3, 15);
  test_set_bits(9, 1, 2, 3, 15);
  test_set_bits(1, 1, 3, 7, 15);
}

void tests_for_invert() {
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

void tests_for_right_rot() {
  test_right_rot(0, 0, 0);
  test_right_rot(0, 1, 0);
  test_right_rot(0, 2, 0);
  test_right_rot(0, 32, 0);
  test_right_rot(0, 31, 0);
  test_right_rot(0, 30, 0);

  test_right_rot(1, 0, 1);
  test_right_rot(1, 1, 1 << 31);
  test_right_rot(1, 2, 1 << 30);
  test_right_rot(1, 3, 1 << 29);

  test_right_rot(2, 0, 2);
  test_right_rot(2, 1, 1);
  test_right_rot(2, 2, 1 << 31);
  test_right_rot(2, 3, 1 << 30);
  test_right_rot(2, 4, 1 << 29);

  test_right_rot(3, 0, 3);
  test_right_rot(3, 1, 1 << 31 | 1);
  test_right_rot(3, 2, 1 << 31 | 1 << 30);
  test_right_rot(3, 3, 1 << 30 | 1 << 29);
  test_right_rot(3, 4, 1 << 29 | 1 << 28);

  test_right_rot(5, 0, 5);
  test_right_rot(5, 1, 1 << 31 | 2);
  test_right_rot(5, 2, 1 << 30 | 1);
  test_right_rot(5, 3, 1 << 31 | 1 << 29);
  test_right_rot(5, 4, 1 << 30 | 1 << 28);
}

void test_bitwise_operations() {
  test_for_get_bits();
  tests_for_set_bits();
  tests_for_invert();
  tests_for_right_rot();
  printf("Test for '2.9 Bitwise operations' is passed!\n");
}