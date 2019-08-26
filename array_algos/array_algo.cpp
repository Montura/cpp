#include <iostream>

int gcd(int dividend, int divisor) {
  if (divisor == 0) {
    return dividend;
  }
  return gcd(divisor, dividend % divisor);
}

void printArray(int* array, int size) {
  for (int i = 0; i < size; ++i) {
    std::cout << array[i] << " ";
  }
  std::cout << "\n";
}

/*Function to left Rotate arr[] of size n by 1*/
void leftRotateByOne(int *array, int size) {
  if (size < 1) return;

  int tmp = array[0];
  for (int i = 0; i < size - 1; ++i) {
    array[i] = array[i + 1];
  }
  array[size - 1] = tmp;
}

/*Function to left Rotate arr[] of size n by step*/
// O(n * d)
void leftRotate(int *array, int size, int count) {
  for (int i = 0; i < count; ++i) {
    leftRotateByOne(array, size);
  }
}

// O(n)
void leftRotateFast(int *array, int size, int dist) {
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

void rotateByOneAndPrint(int* array, int size) {
  leftRotateByOne(array, size);
  printArray(array, size);
}

void rotateAndPrint(int* array, int size, int count) {
  leftRotate(array, size, count);
  printArray(array, size);
}

void rotateFastAndPrint(int* array, int size, int count) {
  leftRotateFast(array, size, count);
  printArray(array, size);
}

void array_rotation() {
  int a[] = {1, 2, 3, 4, 5, 6};
  int b[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

//  rotateByOneAndPrint(a, 6);
//  rotateByOneAndPrint(b, 9);
//
//  rotateAndPrint(a, 6, 3);
//  rotateAndPrint(b, 9, 4);

  rotateFastAndPrint(a, 6, 2);
  rotateFastAndPrint(b, 9, 4);

//  std::cout << gcd(6, 12);
//  std::cout << gcd(12, 6);
}
