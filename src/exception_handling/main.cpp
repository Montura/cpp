#include <cassert>

int test_setjmp_longjmp();
int testSEH();
int test_except_handler();

int main() {
  assert(test_except_handler() == 0);
  assert(test_setjmp_longjmp() == 0);
  assert(testSEH() == 0);
}