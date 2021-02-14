#include "data_structures/binary_heap.h"
#include "data_structures/heap_node.h"
#include "data_structures/heap_comparators.h"

void test_binary_heap() {
  BinaryHeap<int, Min> h;
  h.insert(3);
  h.insert(2);
  std::cout << h.getMin() << std::endl;
  h.deleteKeyByIdx(0);
  std::cout << h.getMin() << std::endl;
  h.insert(15);
  h.insert(16);
  h.insert(20);
  h.insert(17);
  h.insert(88);
  std::cout << h.getMin() << " ";
  h.insert(5);
  std::cout << h.getMin() << " ";
  h.insert(4);
  std::cout << h.getMin() << " ";
  h.insert(45);
  std::cout << h.getMin() << " ";
  h.decreaseKey(2, 1);
  std::cout << h.getMin() << std::endl;

  std::vector<int> arr = {12, 11, 13, 5, 6, 7};
  BinaryHeap<int, Max>::heapSort(arr);
  printArray(arr);

  arr =  { 9, -3, 5, 2, 6, 8, -6, 1, 3 };
  BinaryHeap<int, Min>::heapSort(arr);
  printArray(arr);

  BinaryHeap<int, Min> h3;
  h3.insert(4);
  h3.insert(9);
  h3.insert(7);
  std::cout << h3.getMin() << std::endl;
  h3.deleteKeyByValue(4);
  std::cout << h3.getMin() << std::endl;

  auto h2 = h.merge(h3);
  std::cout << h2.getMin() << std::endl;

  std::vector<HeapNode> strArr =
    { HeapNode {12, "12"}, HeapNode {11, "11"}, HeapNode {13, "13"}, HeapNode {7, "7"}, HeapNode {5, "5"}, HeapNode {6, "6"}};
  BinaryHeap<HeapNode, Min>::heapSort(strArr);
  printArray(strArr);

  BinaryHeap<HeapNode, Max>::heapSort(strArr);
  printArray(strArr);

  BinaryHeap<HeapNode, Min> h4(strArr);
  std::cout << h4.getMin() << std::endl;
  h4.deleteKeyByIdx(1);
  std::cout << h4.getMin() << std::endl;
  h4.deleteKeyByValue(HeapNode(11, "11"));
  std::cout << h4.getMin() << std::endl;

}