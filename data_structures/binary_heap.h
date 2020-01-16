#pragma once

#include <vector>
#include <iostream>
#include <array>
#include <limits>

template <class T>
using Comparator = bool(*)(T&, T&);

template <class Key, template <class> class Type>
class BinaryHeap {
  Comparator<Key> const comparator = Type<Key>::compare;
  int const MAX = std::numeric_limits<Type<Key>>::max();

  std::vector<Key> data;
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

  const std::vector<int>& getData() const {
    return data;
  }

public:
  explicit BinaryHeap(std::vector<Key> const& arr = {}) {
    std::cout << __PRETTY_FUNCTION__ <<  MAX << std::endl;
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
  Key extractMin() {
    if (heapSize == 0) {
      std::cout << "Heap is emtpy!" << std::endl;
      return data[heapSize - 1];
    }
    Key min = data[0];
    data[0] = data[heapSize - 1];
    heapSize -= 1;
    siftDown(0);
    return min;
  }

  Key getMin() const {
    return data[0];
  }

  int getSize() const {
    return heapSize;
  }

  void decreaseKey(int i, int newValue) {
    if (i > capacity) {
      std::cout << "Can't decrease value at index: " << i << ". Index ouf of range. Heap size is: " << capacity <<std::endl;
    }
    data[i] = newValue;
    siftUp(i);
  }

  void deleteKeyByIdx(int i) {
    data[i] = MAX;
    siftDown(0);
    extractMin();
  }

  void deleteKeyByValue(Key const & i) {
    for (int j = 0; j < heapSize; ++j) {
      if (data[j] == i) {
        deleteKeyByIdx(j);
      }
    }
  }

  BinaryHeap<Key, Type> merge(const BinaryHeap<Key, Type>& h) {
    int hSize = h.getSize();
    int nCapacity = heapSize + hSize;
    std::vector<int> v(nCapacity);

    std::copy(data.cbegin(), data.cbegin() + heapSize, v.begin());
    std::vector<int> const& hData = h.getData();
    std::copy(hData.cbegin(), hData.cbegin() + hSize, v.begin() + heapSize);

    return BinaryHeap(v);

  }

  // O(n * log n)
  static void heapSort(std::vector<Key>& array) {
    int size = static_cast<int>(array.size());
    BinaryHeap heap(array);
    for (int i = 0; i < size; ++i) { // O(n)
      array[i] = heap.extractMin(); // O(log n)
    }
  }
};

