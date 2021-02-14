//
// https://contest.yandex.ru/contest/8458/problems/D/
//

#include <iostream>
#include <string>
#include <vector>

void genSequence(int n, const std::string& answer, int opened_count = 0, int closed_count = 0) {
  if (answer.size() == 2 * n) {
    std::cout << answer << std::endl;
    return;
  }
  if (opened_count < n) {
    genSequence(n, answer + "(", opened_count + 1, closed_count);
  }
  if (opened_count > closed_count) {
    genSequence(n, answer + ")", opened_count, closed_count + 1);
  }
}

void genSequence(int len, std::vector<char>& answer, int diff = 0, int idx = 0) {
  if (diff <= len - idx - 2) {
    answer[idx] = '(';
    genSequence(len, answer, diff + 1, idx + 1);
  }
  if (diff > 0) {
    answer[idx] = ')';
    genSequence(len, answer, diff - 1, idx + 1);
  }
  if (idx == len && diff == 0) {
    for (auto& s : answer) {
      std::cout << s;
    }
    std::cout << std::endl;
    return;
  }
}

void impl1(const int size) {
  std::cout << "Use std::string" << std::endl;
  std::string s;
  genSequence(size, s);
}

void impl2(const int size) {
  std::cout << "Use std::vector" << std::endl;
  int answerSize = size * 2; //
  std::vector<char> answer(answerSize);
  genSequence(answerSize, answer);
}

bool isValid(std::string const& s) {
  stack<char> st;
  char curr = 0;
  for (auto& ch : s) {
    if (ch == '(' || ch == '[' || ch == '{') {
      st.push(ch);
    } else {
      if (st.empty()) {
        return false;
      } else {
        curr = st.top();
        st.pop();
        if (curr == '(' && ch == ')') {
          continue;
        } else if (curr == '[' && ch == ']') {
          continue;
        } else if (curr == '{' && ch == '}') {
          continue;
        } else {
          return false;
        }
      }
    }
  }
  return st.empty();
}

int main() {
  int n = 0;
  std::cin >> n;
  impl1(n);
  impl2(n);
}