#include <cstdio>

void swap(int& a, int& b);
void printArr(int const* arr);
int binarySearch(int* arr, int key, int left, int right);
int find(int* arr, int const size, bool findMax);

// Time Complexity:
// - O(n^2), swaps O(n) as there are two nested loops.
// Auxiliary Space: O(1)
// Sorting In Place: Yes
// Stability : The default implementation is not stable. However it can be made stable.
// The good thing about selection sort is it never makes more than O(n) swaps and can be useful when memory write is a costly operation.
void selectionSort(int* arr, int const size) {
  for (int i = 0, minIdx = 0; i < size - 1; ++i) {
    minIdx = i;
    for (int j = i + 1; j < size; ++j) {
      if (arr[j] < arr[minIdx]) {
        minIdx = j;
      }
    }
    swap(arr[i], arr[minIdx]);
  }
}

// Time Complexity:
//  - Worst case: O(n^2), swaps O(n^2). It occurs when array is reverse sorted.
//  - Best case:  O(n), swaps O(1). It occurs when array is already sorted.
// Auxiliary Space: O(1)
// Sorting In Place: Yes
// Stable: Yes

void bubbleSort(int* arr, int const size) {
  for (int i = 0; i < size - 1; ++i) {
    bool swapped = false; // Optimization O(n) for sorted array
    for (int j = i + 1; j < size; ++j) {
      if (arr[i] > arr[j]) {
        swap(arr[i], arr[j]);
        swapped = true;
      }
    }
    if (!swapped) {
      break;
    }
  }
}

// Time Complexity:
//  - Worst case :  O(n^2), it occurs when array is reverse sorted.
//  - Best case:    O(n),  it occurs when array is already sorted.
// Auxiliary Space: O(1)
// Sorting In Place: Yes
// Stable: Yes
void insertionSort(int* arr, int const size) {
  for (int i = 1; i < size; ++i) {
    int curr = arr[i];
    int j = i - 1;
    for(; j >= 0 && curr < arr[j]; --j) {
      arr[j + 1] = arr[j];
    }
    arr[j + 1] = curr;
  }
}

void binaryInsertionSort(int* arr, int const size) {
  for (int i = 1; i < size; ++i) {
    int curr = arr[i];
    int j = i - 1;

    int pos = binarySearch(arr, curr, 0, j);

    for(; j >= pos; --j) {
      arr[j + 1] = arr[j];
    }
    arr[j + 1] = curr;
  }
}

// Time Complexity: O(n+k) where n is the number of elements in input array and k is the range of input.
// Auxiliary Space: O(n+k)
void countSort(int* arr, int const size) {
  int maxV = find(arr, size, true);
  int minV = find(arr, size, false);
  int range = maxV - minV + 1;

  int* count = new int[range];
  int* result = new int[size];

  for (int i = 0; i < size; ++i) {
    count[i] = 0;
  }

  for (int i = 0; i < size; ++i) {
    count[arr[i] - minV]++;
  }

  for (int i = 1; i < range; ++i) {
    count[i] += count[i - 1];
  }

  for (int i = size - 1; i >= 0; --i) {
    int pos = arr[i] - minV;
    count[pos] = count[pos] - 1;
    result[count[pos]] = arr[i];
  }

  for (int i = 0; i < size; ++i) {
    arr[i] = result[i];
  }
  delete[] count;
  delete[] result;
}

// Time Complexity: O(log n)
// Auxiliary Space: O(1)
int binarySearch(int* arr, int key, int left, int right) {
  int mid = 0;

  while (left < right) {
    mid = left + (right - left) / 2;

    if (arr[mid] < key) {
      left = mid + 1;
    } else if (arr[mid] > key) {
      right = mid;
    } else {
      return mid;
    }
  }
  return (key > arr[left]) ? (left + 1) : left;
}

void swap(int& a, int& b) {
  int tmp = a;
  a = b;
  b = tmp;
}

void printArr(int const* arr, int const size) {
  for (int i = 0; i < size; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int find(int* arr, int const size, bool findMax) {
  int res = 0;
  for (int i = 0; i < size; ++i) {
    if (findMax) {
      res = res > arr[i] ? res : arr[i];
    } else {
      res = res < arr[i] ? res : arr[i];
    }
  }
  return res;
}

