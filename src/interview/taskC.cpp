//
// https://contest.yandex.ru/contest/8458/problems/C/
//

#include <iostream>

int main() {
  int n = 0;
  std::cin >> n;
  int symbol = 0, prevSymbol = 0;

  for (int i = 0; i < n; ++i) {
    std::cin >> symbol;
    if (i > 0 && prevSymbol == symbol) {
      continue;
    } else {
      std::cout << symbol << std::endl;
      prevSymbol = symbol;
    }
  }

  return 0;
}


