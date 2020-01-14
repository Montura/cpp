#include "binary_heap.h"

using VecI = std::vector<int>;
void printArr(const VecI& arr);

void test_binary_heap() {
  BinaryHeap h(11);
  h.insert(3);
  h.insert(2);
  h.deleteKey(1);
  h.insert(15);
  h.insert(5);
  h.insert(4);
  h.insert(45);
  std::cout << h.extractMin() << " ";
  std::cout << h.getMin() << " ";
  h.decreaseKey(2, 1);
  std::cout << h.getMin() << std::endl;
}