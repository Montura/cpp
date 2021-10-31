#include <cassert>

int test_setjmp_longjmp();
int testSEH();

int main() {
  assert(test_setjmp_longjmp() == 0);
  assert(testSEH() == 0);
}