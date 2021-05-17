#include "throw.h"

#if __linux__
  #include <sys/personality.h>
#include <stdio.h>
#include <unistd.h>

#endif

int main() {
//  printf("pId = %llu\n", getpid());
//  printf("Address of main = %p\n", (void*)(&main));
  // 0x564cec2932c9
#if __linux__
  personality(ADDR_NO_RANDOMIZE);
#endif
  seppuku();
  return 0;
}


