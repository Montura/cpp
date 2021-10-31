#include <csetjmp>
#include <cstdio>

#define TRY   do { jmp_buf env_; if (!setjmp(env_)) {
#define CATCH } else {
#define ETRY  } } while (0);
#define THROW longjmp(env_, 1);

int test_setjmp_longjmp() {
  printf(" ------------------  Start SETJMP LONGJMP test ------------------ \n");
  TRY {
    printf("In TRY statement\n");
    THROW
    printf("I do not appear\n");
  } CATCH {
    printf("Got an exception\n");
  }
  ETRY
  printf(" ------------------  End SETJMP LONGJMP test ------------------ \n");
  return 0;
}