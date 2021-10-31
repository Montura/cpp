#include <cstdio>
#include <Windows.h>

#ifdef CPPEX
  #include <exception>
#endif

#ifdef JMP
  #define TRY do {}
#endif

class TestClass {
public:
  ~TestClass() {
    printf("Destroying TestClass!\r\n");
  }
};

__declspec(noinline) void TestCPPEX() {
#ifdef CPPEX
  printf("Throwing C++ exception\r\n");
    throw std::exception("");
#else
  printf("\t\t\tTriggering SEH exception\r\n");
  volatile int *pInt = 0x00000000;
  *pInt = 20;
#endif
}

__declspec(noinline) void TestExceptions() {
  TestClass d;
  TestCPPEX();
}

int detectExceptionStatus(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
  printf("\tin  __except(filterGetExceptionCode(), GetExceptionInformation()): ");
  if (code == EXCEPTION_ACCESS_VIOLATION) {
    printf("caught SEH as expected.\n");
    return EXCEPTION_EXECUTE_HANDLER;
  } else {
    printf("didn't catch SEH, unexpected.\n");
    return EXCEPTION_CONTINUE_SEARCH;
  }
}

int testSEH() {
  printf(" ------------------  Start SEH test ------------------ \n");
  printf("hello\n");
  __try
  {
    printf("\tin try(-except) block\n");
    __try
    {
      printf("\t\tin try(-finally) block\n");
      TestExceptions();
    }
    __finally
    {
      printf("\t\tin (tyr-)finally. termination: ");
      printf(AbnormalTermination() ? "\tabnormal" : "\tnormal");
      printf("\n");
    }
  }
  __except(detectExceptionStatus(GetExceptionCode(), GetExceptionInformation()))
  {
    printf("\tin (try)-except block\n");
  }
  printf("world\n");
  printf(" ------------------  END SEH test ------------------ \n");

  return 0;
}
