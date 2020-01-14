#pragma once

#include <vector>
#include <iostream>
#include <array>

inline bool min(int a, int b) {
  return a < b;
}

inline bool max(int a, int b) {
  return a > b;
}

typedef bool(* Comparator)(int, int);

template <Comparator comp>
class BinaryHeap {
  Comparator const comparator = comp;
  std::vector<int> data;
  int heapSize = 0;
  int capacity = 0;

  static inline int parentIdx(int i) {
    return (i - 1) / 2;
  }

  static inline int leftIdx(int i) {
    return 2 * i + 1;
  }

  static inline int rightIdx(int i) {
    return 2 * i + 2;
  }

  // O (log n)
  void siftUp(int i) {
    while (comparator(data[i], data[parentIdx(i)])) {
      std::swap(data[i], data[parentIdx(i)]);
      i = parentIdx(i);
    }
  }

  // O (log n)
  void siftDown(int i) {
    while (leftIdx(i) < heapSize) {
      int left = leftIdx(i);
      int right = rightIdx(i);
      int smallestChild = left;
      if (right < heapSize && comparator(data[right], data[left])) {
        smallestChild = right;
      }
      if (comparator(data[i], data[smallestChild])) {
        return;
      }
      std::swap(data[i], data[smallestChild]);
      i = smallestChild;
    }
  }

public:
  template <class ArrType = std::array<int, 0>>
  explicit BinaryHeap(ArrType const& arr = {}) {
    int size = static_cast<int>(arr.size());
    if (!arr.empty()) {
      data.resize(size);
      for (int i = 0; i < size; ++i) {
        data[i] = arr[i];
        heapSize++;
      }
    }

    for (int i = size / 2; i >= 0; --i) {
      siftDown(i);
    }
  }

  ~BinaryHeap() {
    capacity = heapSize = 0;
  }

  // O (log n)
  void insert(int key) {
    if (heapSize == capacity) {
      capacity = 2 * (heapSize + 1);
      data.resize(capacity);
      std::cout << "Heap capacity increased to " << capacity << std::endl;
    }
    heapSize += 1;
    data[heapSize - 1] = key;
    siftUp(heapSize - 1);
  }

  // O (log n)
  int extractMin() {
    if (heapSize == 0) {
      std::cout << "Heap is emtpy!" << std::endl;
      return INT32_MAX;
    }
    int min = data[0];
    data[0] = data[heapSize - 1];
    heapSize -= 1;
    siftDown(0);
    return min;
  }

  int getMin() {
    return data[0];
  }


  void decreaseKey(int i, int newValue) {
    if (i > capacity) {
      std::cout << "Can't decrease value at index: " << i << ". Index ouf of range. Heap size is: " << capacity <<std::endl;
    }
    data[i] = newValue;
    siftUp(i);
  }

  void deleteKeyByIdx(int i) {
    decreaseKey(i, INT_MIN);
    extractMin();
  }

  void deleteKeyByValue(int i) {
    for (int j = 0; j < heapSize; ++j) {
      if (data[j] == i) {
        decreaseKey(j, INT_MIN);
        extractMin();
      }
    }
  }

  // O(n * log n)
  static void heapSort(std::vector<int>& array) {
    int size = static_cast<int>(array.size());
    BinaryHeap heap(array);
    for (int i = 0; i < size; ++i) { // O(n)
      array[i] = heap.extractMin(); // O(log n)
    }
  }
};

