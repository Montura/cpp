#include <vector>
#include <iostream>

using VecI = std::vector<int>;
int medianOf3(VecI& arr, int left, int right);
void printArr(const std::vector<int>& arr);

struct Pivot {
  int left;
  int right;
};

// Lomuto's partition scheme
Pivot lomutoPartition(VecI& arr, int left, int right) {
  int pivot = medianOf3(arr, left, right);
  int pIndex = left; // left of pivot

  for (int idx = left; idx < right; ++idx) {
    if (arr[idx] < pivot) {
      std::swap(arr[pIndex], arr[idx]);
      pIndex++; // increment index of smaller element
    }
  }
  std::swap(arr[pIndex], arr[right]); // restore pivot
  return Pivot {pIndex - 1, pIndex + 1}; // return pivot location
}

Pivot hoarePartition(VecI& arr, int left, int right) {
  int pivot = medianOf3(arr, left, right);
  int leftPtr = left - 1;
  int rightPtr = right + 1;

  while (true) {
    while (arr[++leftPtr] < pivot); // Find leftmost element greater than or equal to pivot
    while (arr[--rightPtr] > pivot); // Find rightmost element smaller than or equal to pivot

    // If two pointers met.
    if (leftPtr >= rightPtr) {
      return Pivot { rightPtr, rightPtr + 1};
    }

    std::swap(arr[leftPtr], arr[rightPtr]);
  }
}

int medianOf3(VecI& arr, int left, int right) {
  int middle = left + (right - left) / 2;

  int& lhs = arr[left];
  int& mid = arr[middle];
  int& rhs = arr[right];

  if (lhs > mid) { // order left & middle
    std::swap(lhs, mid);
  }
  if (lhs > rhs) { // order left & right
    std::swap(lhs, rhs);
  }
  if (mid > rhs) { // order middle & right
    std::swap(mid, rhs);
  }

  std::swap(mid, rhs); // move pivot to the right end

  return arr[right]; // return median value
}


void quickSort(VecI& arr, int left, int right) {
  if (left < right) {
    Pivot pivotPos = hoarePartition(arr, left, right);

    quickSort(arr, left, pivotPos.left);
    quickSort(arr, pivotPos.right, right);
  }
}

void test_quick_sort() {
  std::vector<int> arr { 9, -3, 5, 2, 6, 8, -6, 1, 3 };
  int right = static_cast<int>(arr.size()) - 1;
  std::cout << "Before: ";
  printArr(arr);
  quickSort(arr, 0, right);
  std::cout << "After: ";
  printArr(arr);
}
