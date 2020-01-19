#include <iostream>
#include <vector>

using VecI = std::vector<int>;

// std::binary_search(arr.begin(), arr.end(), key);
// Time Complexity: O(log n)
// Auxiliary Space: O(1)
int binarySearch(const VecI& arr, int key, int left, int right) {
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
