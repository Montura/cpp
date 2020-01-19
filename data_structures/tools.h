#include <iostream>
#include <vector>
#include <limits>

template <class T>
using Comparator = bool(*)(T&, T&);

template <class T>
void printArr(const std::vector<T>& arr) {
  for (T const& elem : arr) {
    std::cout << elem << std::endl;
  }
  std::cout << std::endl;
}
