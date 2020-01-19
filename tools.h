#include <iostream>
#include <vector>
#include <limits>

template <class T>
using Comparator = bool(*)(T&, T&);

template <class Container>
void printArray(const Container& arr, std::string msg = "") {
  if (!msg.empty()) {
    std::cout << msg << std::endl;
  }
  for (auto const& elem : arr) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;
}
