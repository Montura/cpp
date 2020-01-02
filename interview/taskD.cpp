//
// https://contest.yandex.ru/contest/8458/problems/D/
//

#include <iostream>

void genSequence(int len, char* answer, int diff = 0, int idx = 0) {
  if (diff <= len - idx - 2) {
    answer[idx] = '(';
    genSequence(len, answer, diff + 1, idx + 1);
  }
  if (diff > 0) {
    answer[idx] = ')';
    genSequence(len, answer, diff - 1, idx + 1);
  }
  if (idx == len && diff == 0) {
    std::cout << answer << std::endl;
    return;
  }
}

int main() {
  int n = 0;
  std::cin >> n;
  int const size = 2 * n;
  char* answer = new char[size + 1];
  answer[size] = '\0';
  genSequence(size, answer);
  delete[] answer;
}