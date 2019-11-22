#pragma once

#include <iostream>

struct alpha_tag {};
struct betta_tag {};

template <typename T>
struct traits {
  typedef alpha_tag tag;
};

template <>
struct traits<int> {
  typedef betta_tag tag;
};

template <typename T>
void work_dispatch(const T& val, const alpha_tag& st) {
  std::cout << "Slow function" << std::endl;
}

template <typename T>
void work_dispatch(const T& val, const betta_tag& ft) {
  std::cout << "Fast function" << std::endl;
}

template <typename T>
void work_dispatch(const T& val) {
  work_dispatch(val, typename traits<T>::tag());
}

void test_tag_dispatching() {
  work_dispatch(3.0);
  work_dispatch(3);
}