#include <climits>
#include "binary_heap.h"

using VecI = std::vector<int>;
void printArr(const VecI& arr);

void test_binary_heap() {
  BinaryHeap<max> h;
  h.insert(3);
  h.insert(2);
  h.deleteKeyByIdx(1);
  h.insert(15);
  h.insert(16);
  h.insert(20);
  h.insert(17);
  h.insert(88);
  std::cout << h.extractMin() << " ";
//  h.insert(5);
  std::cout << h.extractMin() << " ";
//  h.insert(4);
  std::cout << h.extractMin() << " ";
//  h.insert(45);
  std::cout << h.getMin() << " ";
  h.decreaseKey(2, 1);
  std::cout << h.getMin() << std::endl;

  std::vector<int> arr = {12, 11, 13, 5, 6, 7};
  BinaryHeap<max>::heapSort(arr);
  printArr(arr);

  arr =  { 9, -3, 5, 2, 6, 8, -6, 1, 3 };
  BinaryHeap<max>::heapSort(arr);
  printArr(arr);

  BinaryHeap<min> h3;
  h3.insert(4);
  h3.insert(9);
  h3.insert(1e9);
  std::cout << h3.getMin() << std::endl;
  h3.deleteKeyByValue(4);
  std::cout << h3.getMin() << std::endl;
}