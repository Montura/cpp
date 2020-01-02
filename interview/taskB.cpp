//
// https://contest.yandex.ru/contest/8458/problems/B/
//

#include <iostream>

inline int max(int a, int b) {
  return a > b ? a : b;
}

int main() {
  int n = 0;
  std::cin >> n;
  int currentLen = 0, maxLen = 0;
  char symbol = -1, prevSymbol = '1';
  for (int i = 0; i < n; ++i) {
    std::cin >> symbol;
    if ( symbol == '1' && symbol == prevSymbol) {
      currentLen += 1;
      prevSymbol = symbol;
    } else {
      maxLen = max(currentLen, maxLen);
      currentLen = 0;
    }
  }
  maxLen = max(currentLen, maxLen);

  std::cout << maxLen;
}