#include <cassert>

int test_setjmp_longjmp();
#if defined(MSVC)
  int testSEH();
  int test_except_handler();
#else
  int stack_unwinding();
#endif

int main() {
#if defined(MSVC)
  assert(test_except_handler() == 0);
  assert(testSEH() == 0 );
#else
  assert(stack_unwinding() == 0);
#endif
  assert(test_setjmp_longjmp() == 0);
}