#include <iostream>

static void by_value(const int array[]) {
  std::cout << __PRETTY_FUNCTION__ << ", array_size is: " << sizeof(array) << std::endl;
}

static void by_ptr(int const* array) {
  std::cout << __PRETTY_FUNCTION__ << ", array_size is: " << sizeof(array) << std::endl;
}

static void by_ref(int (&array)[5]) {
  std::cout << __PRETTY_FUNCTION__ << ", array_size is: " << sizeof(array) << std::endl;
}

void array_decay_example() {
  int array[5] = {};
  by_value(array);
  by_ptr(array);
  by_ref(array);
}
