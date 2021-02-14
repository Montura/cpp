#include <array>
#include <vector>
#include <string>
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

void array_rotation() {
  IntArray<6> a = {1, 2, 3, 4, 5, 6};
  IntArray<9> b = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  printArray(a, "Original array:");
  printArray(b, "Original array:");

  leftRotateByOne(a);
  printArray(a, "Left rotate by 1");

  leftRotateByOne(b);
  printArray(b, "Left rotate by 1");

  leftRotate(a, 3);
  printArray(a, "Left rotate by " + std::to_string(3));

  leftRotate(b, 4);
  printArray(b, "Left rotate by " + std::to_string(4));

  leftRotateFast(a, 2);
  printArray(a, "Left fast (gcd) rotate by " + std::to_string(2));

  leftRotateFast(b, 4);
  printArray(b, "Left fast (gcd) rotate by " + std::to_string(4));
}

// Remove all zeros from array for O (n)
// 1 0 2 0 0 3 0 -> 1 2 3
void removeZeros(std::vector<int> data) {
  size_t size = data.size();
  int zeroCount = 0;
  for (int i = 0; i < size; ++i) {
    if (data[i] == 0) {
      ++zeroCount;
    } else {
      if (zeroCount > 0) {
        int nonZeroElem = i - zeroCount;
        data[nonZeroElem] = data[i];
      }
    }
  }
  for (auto it = data.cend(); *it >= size - zeroCount; --it) {
    data.erase(it);
  }
}