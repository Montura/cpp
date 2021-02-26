#include <assert.h>

// Common error:
//  — Doesn't work with array of 0/1/2 elements
//  — Doesn't find the first or the last element
//  — Doesn't work if there is no key element in array
//  — Doesn't work if there are duplicated elements in array
//  — Middle index overflow

int bin_search(int key, const int* arr, int size) {
  int left = 0;
  int right = size - 1;
  int mid = 0;

  while (left <= right) {
    mid = left + (right - left) / 2;

    if (key < arr[mid]) {
      right = mid - 1;
    } else if (key > arr[mid]) {
      left = mid + 1;
    } else {
      return mid;
    }
  }
  return -1;
}

// Exercise 3-1.
// Write a version of binary search with only one test inside the loop and measure the difference in run-time


int fast_bin_search(int key, const int* arr, int size) {
  if (size == 0) {
    return -1;
  }

  int left = 0;
  int right = size - 1;
  int mid = 0;

  while (left != right) {
    // mid >= (r + l) / 2. To avoid overflow use next statement
    mid = left + ((right - left) + 1) / 2;

    if (arr[mid] > key) {
      right = mid - 1;
    } else {
      left = mid;
    }
  }

  return (arr[left] == key) ? left : -1;
}

typedef int (*BinSearch)(int key, const int* arr, int size);

void tests_for_bis_search_a(BinSearch bin_search) {
  int a[] = {};
  int b[] = { 0 };
  int c[] = { 0, 1 };
  int d[] = { 0, 1, 2 };
  int e[] = { 0, 1, 2, 3 };
  int f[] = { 0, 1, 3, 4 };
  int g[] = { -1, 1, 3, 5, 6 };

  assert(bin_search(0, a, 0) == -1);

  assert(bin_search(0, b, 1) == 0);
  assert(bin_search(1, b, 1) == -1);

  assert(bin_search(0, c, 2) == 0);
  assert(bin_search(1, c, 2) == 1);
  assert(bin_search(2, c, 2) == -1);

  assert(bin_search(-1, d, 3) == -1);
  assert(bin_search(0, d, 3) == 0);
  assert(bin_search(1, d, 3) == 1);
  assert(bin_search(2, d, 3) == 2);
  assert(bin_search(3, d, 3) == -1);

  assert(bin_search(0, e, 4) == 0);
  assert(bin_search(1, e, 4) == 1);
  assert(bin_search(2, e, 4) == 2);
  assert(bin_search(3, e, 4) == 3);
  assert(bin_search(4, e, 4) == -1);

  assert(bin_search(0, f, 4) == 0);
  assert(bin_search(1, f, 4) == 1);
  assert(bin_search(2, f, 4) == -1);
  assert(bin_search(3, f, 4) == 2);
  assert(bin_search(4, f, 4) == 3);
  assert(bin_search(5, f, 4) == -1);

  assert(bin_search(-1, g, 5) == 0);
  assert(bin_search(0, g, 5) == -1);
  assert(bin_search(1, g, 5) == 1);
  assert(bin_search(2, g, 5) == -1);
  assert(bin_search(3, g, 5) == 2);
  assert(bin_search(4, g, 5) == -1);
  assert(bin_search(5, g, 5) == 3);
  assert(bin_search(6, g, 5) == 4);
}

void tests_for_bis_search() {
  tests_for_bis_search_a(&bin_search);
  tests_for_bis_search_a(&fast_bin_search);
}