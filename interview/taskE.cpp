//
//https://contest.yandex.ru/contest/8458/problems/E/
//

#include <iostream>
#include <string>
#include <array>

int main() {
  std::string a, b;
  std::cin >> a >> b;
  std::array<int, 26> array{};
  for (char x : a) {
    int letterNumber = x - 'a';
    array[letterNumber]++;
  }
  for (char x : b) {
    int letterNumber = x - 'a';
    array[letterNumber]--;
  }
  bool answer = true;
  for (int x : array) {
    if (x != 0) {
      answer = false;
      break;
    }
  }
  std::cout << answer;
}
