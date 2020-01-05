#include <vector>
#include <algorithm>

using VecI = std::vector<int>;


int binarySearch(const VecI& arr, int key, int left, int right);

// Time Complexity:
// - O(n^2), swaps O(n) as there are two nested loops.
// Auxiliary Space: O(1)
// Sorting In Place: Yes
// Stability : The default implementation is not stable. However it can be made stable.
// The good thing about selection sort is it never makes more than O(n) swaps and can be useful when memory write is a costly operation.
void selectionSort(VecI& arr) {
  auto size = static_cast<int >(arr.size());
  for (int i = 0, minIdx = 0; i < size - 1; ++i) {
    minIdx = i;
    for (int j = i + 1; j < size; ++j) {
      if (arr[j] < arr[minIdx]) {
        minIdx = j;
      }
    }
    std::swap(arr[i], arr[minIdx]);
  }
}

// Time Complexity:
//  - Worst case: O(n^2), swaps O(n^2). It occurs when array is reverse sorted.
//  - Best case:  O(n), swaps O(1). It occurs when array is already sorted.
// Auxiliary Space: O(1)
// Sorting In Place: Yes
// Stable: Yes

void bubbleSort(VecI& arr) {
  auto size = static_cast<int>(arr.size());
  for (int i = 0; i < size - 1; ++i) {
    bool swapped = false; // Optimization O(n) for sorted array
    for (int j = i + 1; j < size; ++j) {
      if (arr[i] > arr[j]) {
        std::swap(arr[i], arr[j]);
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
void insertionSort(VecI& arr) {
  auto size = static_cast<int>(arr.size());
  for (int i = 1; i < size; ++i) {
    int curr = arr[i];
    int j = i - 1;
    for(; j >= 0 && curr < arr[j]; --j) {
      arr[j + 1] = arr[j];
    }
    arr[j + 1] = curr;
  }
}

void binaryInsertionSort(VecI& arr) {
  auto size = static_cast<int>(arr.size());
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
void countSort(VecI& arr) {
  auto minMaxPair = std::minmax_element(arr.cbegin(), arr.cend());
  int minV = *minMaxPair.first;
  int maxV = *minMaxPair.second;
  int range = maxV - minV + 1;

  auto size = static_cast<int>(arr.size());
  VecI count(range);
  VecI result(size);

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

  std::copy(arr.cbegin(), arr.cend(), result.begin());
}




