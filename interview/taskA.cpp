//
// https://contest.yandex.ru/contest/8458/problems/A/
//
#include <string>
#include <iostream>

int main () {
  std::string jewelry, stones;
  std::cin >> jewelry >> stones;

  int totalCount = 0;
  for (char x : stones) {
    totalCount += jewelry.find(x) != std::string::npos;
  }
  std::cout << totalCount;
}