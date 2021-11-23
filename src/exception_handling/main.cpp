#include <cassert>

int test_setjmp_longjmp();

#if defined(_WIN32) && defined(_M_X86)
  int testSEH();
  int test_except_handler();
#endif

#if defined(_WIN64)
  void testStackTrace64();
#endif

#if defined(__GNUC__) || defined(__clang__)
  int stack_unwinding();
#endif

int main() {
#if defined(_WIN32) && defined(_M_X86)
  assert(test_except_handler() == 0);
  assert(testSEH() == 0 );
#endif

#if defined(_WIN64)
  testStackTrace64();
#endif

#if defined(__GNUC__) || defined(__clang__)
  assert(stack_unwinding() == 0);
#endif

  assert(test_setjmp_longjmp() == 0);
}