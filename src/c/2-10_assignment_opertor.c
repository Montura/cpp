#include <assert.h>

int bitcount(unsigned int x) {
  int res = 0;
  while (x) {
    if (x & 1) {
      ++res;
    }
    x >>= 1;
  }
  return res;
}

// x &= (x - 1);
// Ex.:
//    1111 1000 - 0000 0001 = 1111 0111
//    1111 0111 - 0000 0001 = 1111 0110
//    1111 0110 - 0000 0001 = 1111 0101
//    1111 0101 - 0000 0001 = 1111 0100
// ...
// In order to subtract we have to "borrow" 1 from the next left-available 1-bit for the rightmost 0-bit.
// This has an effect of turning off the next left-available 1-bit and turning on any 0-bits after it.
// So, all the bits starting with the rightmost 1-bit will be inverted.

// We get a mask that ANDing with the original value and turning off the rightmost 1-bit, leaving the other
// bits unchanged.

// USE:
//    x &= (x - 1) instead of x >>= 1
//  as the increment expression in the loop.
//
//  we can omit the test for the least significant bit inside the loop.

// Exercise 2-9.
// In a two's complement number system, x &= (x-1) deletes the rightmost 1-bit in x. Explain why.
// Use this observation to write a faster version of bitcount.
int fast_bitcount(unsigned int x) {
  int res = 0;

  while (x) {
    ++res;
    x &= (x - 1);
  }
  return res;
}

void tests_for_bitcount() {
  assert(bitcount(1) == 1);
  assert(bitcount(2) == 1);
  assert(bitcount(3) == 2);
  assert(bitcount(4) == 1);
  assert(bitcount(5) == 2);
  assert(bitcount(6) == 2);
  assert(bitcount(7) == 3);
  assert(bitcount(8) == 1);
  assert(bitcount(9) == 2);
  assert(bitcount(10) == 2);
  assert(bitcount(11) == 3);
  assert(bitcount(12) == 2);
  assert(bitcount(13) == 3);
  assert(bitcount(14) == 3);
  assert(bitcount(15) == 4);

  assert(fast_bitcount(1) == 1);
  assert(fast_bitcount(2) == 1);
  assert(fast_bitcount(3) == 2);
  assert(fast_bitcount(4) == 1);
  assert(fast_bitcount(5) == 2);
  assert(fast_bitcount(6) == 2);
  assert(fast_bitcount(7) == 3);
  assert(fast_bitcount(8) == 1);
  assert(fast_bitcount(9) == 2);
  assert(fast_bitcount(10) == 2);
  assert(fast_bitcount(11) == 3);
  assert(fast_bitcount(12) == 2);
  assert(fast_bitcount(13) == 3);
  assert(fast_bitcount(14) == 3);
  assert(fast_bitcount(15) == 4);
}