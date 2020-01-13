#pragma once

#include <vector>
#include <iostream>

class BinaryHeap {
  int* data = nullptr;
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
    while (data[i] < data[parentIdx(i)]) {
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
      if (right < heapSize && data[right] < data[left]) {
        smallestChild = right;
      }
      std::swap(data[i], data[smallestChild]);
      i = smallestChild;
    }
  }

  static void siftDown(std::vector<int>& arr, int n, int i) {
    while (leftIdx(i) < n) {
      int left = leftIdx(i);
      int right = rightIdx(i);
      int smallestChild = left;
      if (right < n && arr[right] > arr[left]) {
        smallestChild = right;
      }
      std::swap(arr[i], arr[smallestChild]);
      i = smallestChild;
    }
  }

public:
  explicit BinaryHeap(int size) {
    capacity = size;
    data = new int[size];

    for (int i = size / 2; i >= 0; --i) {
      siftDown(i);
    }
  }

  ~BinaryHeap() {
    delete[] data;
  }

  // O (log n)
  void insert(int key) {
    if (heapSize == capacity) {
      std::cout << "can't insert key: heap capacity is not enough!" << std::endl;
      return;
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
    if (newValue < data[i]) {

    }
    data[i] = newValue;
    siftUp(i);
  }

  void deleteKey(int i) {
    decreaseKey(i, INT_MIN);
    extractMin();
  }

};

