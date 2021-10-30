// https://eli.thegreenplace.net/2015/programmatic-access-to-the-call-stack-in-c/
#include <cxxabi.h>    // __cxa_demangle
#include <execinfo.h>  // backtrace
#include <libunwind.h> // unw_step
#include <stdio.h>
#include <stdlib.h> // free

void gcc_builtin_bt() {
  void *pc0 = __builtin_return_address(0);
  void *pc1 = __builtin_return_address(1);
  void *pc2 = __builtin_return_address(2);
  void *pc3 = __builtin_return_address(3);
  void *pc4 = __builtin_return_address(4);
  void *pc5 = __builtin_return_address(5);
  void *pc6 = __builtin_return_address(6);
  void *pc7 = __builtin_return_address(7);

  printf("Frame 0: PC = 0x%016lx\n", reinterpret_cast<uintptr_t>(pc0));
  printf("Frame 1: PC = 0x%016lx\n", reinterpret_cast<uintptr_t>(pc1));
  printf("Frame 2: PC = 0x%016lx\n", reinterpret_cast<uintptr_t>(pc2));
  printf("Frame 3: PC = 0x%016lx\n", reinterpret_cast<uintptr_t>(pc3));
  printf("Frame 4: PC = 0x%016lx\n", reinterpret_cast<uintptr_t>(pc4));
  printf("Frame 5: PC = 0x%016lx\n", reinterpret_cast<uintptr_t>(pc5));
  printf("Frame 6: PC = 0x%016lx\n", reinterpret_cast<uintptr_t>(pc6));
  printf("Frame 7: PC = 0x%016lx\n", reinterpret_cast<uintptr_t>(pc7));
}

#define BT_BUF_SIZE 100
void glibc_bt() {
  void *bt_array[BT_BUF_SIZE];

  int bt_array_size = backtrace(bt_array, BT_BUF_SIZE);
  char** symbols = backtrace_symbols(bt_array, bt_array_size);

  for (int i = 0; i < bt_array_size; ++i) {
    printf("0x%016lx: %s\n", reinterpret_cast<uintptr_t>(bt_array[i]), symbols[i]);
  }

  free(symbols); // malloced by backtrace_symbols
}

void libunwind_bt() {
  unw_cursor_t cursor;
  unw_context_t context;

  int getContextResult = unw_getcontext(&context);    // returns 0 on success
  int initResult = unw_init_local(&cursor, &context); // returns 0 on success
  if (!getContextResult && !initResult) {
    int step = 0;
    while (unw_step(&cursor)) {
      unw_word_t pc, sp, off;

      unw_get_reg(&cursor, UNW_REG_IP, &pc);
      if (pc == 0) {
        break;
      }
      unw_get_reg(&cursor, UNW_REG_SP, &sp);

      char symbol[256] = {"Unknown symbol"};
      char *name = symbol;

      if (!unw_get_proc_name(&cursor, symbol, sizeof(symbol), &off)) {
        int status;
        if ((name = abi::__cxa_demangle(symbol, NULL, NULL, &status)) == 0)
          name = symbol;
      }

      printf("#%-2d 0x%016lx   sp = 0x%016lx, symbol name = %s + 0x%lx\n",
             step++, static_cast<uintptr_t>(pc), static_cast<uintptr_t>(sp), name, static_cast<uintptr_t>(off));

      if (name != symbol)
        free(name);
    }
  } else {
    printf("Ooops! Something went wrong during unwinding with libunwind\n");
  }
}

int foo(int n = 7) {
  if (n == 0) {
    printf("------ gcc backtrace support: ------\n");
    gcc_builtin_bt();
    printf("\n");

    printf("------ glibc backtrace support: ------\n");
    glibc_bt();
    printf("\n");

    printf("------ Libunwind: ------\n");
    libunwind_bt();
    printf("\n");

    return 1;
  } else {
    return n * foo(n - 1);
  }
}

void bar() {
  foo();
}

void baz() {
  bar();
}

int main() {
  baz();
  return 0;
}