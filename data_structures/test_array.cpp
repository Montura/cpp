#include <iostream>
#include <array>
#include <vector>
#include "tools.h"

template <size_t Size>
using IntArray = std::array<int, Size>;

int gcd(int dividend, int divisor) {
  if (divisor == 0) {
    return dividend;
  }
  return gcd(divisor, dividend % divisor);
}

/*Function to left Rotate arr[] of size n by 1*/
template <size_t size>
void leftRotateByOne(IntArray<size>& array) {
  if (size < 1) return;

  int tmp = array[0];
  for (size_t i = 0; i < size - 1; ++i) {
    array[i] = array[i + 1];
  }
  array[size - 1] = tmp;
}

/*Function to left Rotate arr[] of size n by step*/
// O(n * d)
template <size_t size>
void leftRotate(IntArray<size>& array, int count) {
  for (int i = 0; i < count; ++i) {
    leftRotateByOne(array);
  }
}

// O(n)
template <size_t size>
void leftRotateFast(IntArray<size>& array, int dist) {
  int count = gcd(size, dist);
  for (int i = 0; i < count; ++i) {
    int tmpValue = array[i];
    int currIdx = i;
    int nextIdx = (currIdx + dist) % size;

    while (nextIdx != i) {
      array[currIdx] = array[nextIdx];
      currIdx = nextIdx;
      nextIdx = (currIdx + dist) % size;
    }

    array[currIdx] = tmpValue;
  }
}

template <size_t size>
void rotateByOneAndPrint(IntArray<size>& array) {
  leftRotateByOne(array);
  printArray(array, "Left rotate by 1");
}

template <size_t size>
void rotateAndPrint(IntArray<size>& array, int count) {
  leftRotate(array, count);
  printArray(array, "Left rotate by " + std::to_string(count));
}

template <size_t size>
void rotateFastAndPrint(IntArray<size>& array, int count) {
  leftRotateFast(array, count);
  printArray(array, "Left fast (gcd) rotate by " + std::to_string(count));
}

void array_rotation() {
  IntArray<6> a = {1, 2, 3, 4, 5, 6};
  IntArray<9> b = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  printArray(a, "Original array:");
  printArray(b, "Original array:");

  rotateByOneAndPrint(a);
  rotateByOneAndPrint(b);

  rotateAndPrint(a, 3);
  rotateAndPrint(b, 4);

  rotateFastAndPrint(a, 2);
  rotateFastAndPrint(b, 4);

  std::cout << gcd(6, 12);
  std::cout << gcd(12, 6);
}
