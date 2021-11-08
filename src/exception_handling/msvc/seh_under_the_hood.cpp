//==================================================
// MYSEH - Matt Pietrek 1997
// Microsoft Systems Journal, January 1997
// To compile: add_link_options(/SAFESEH:NO)
//==================================================
#include <windows.h>
#include <cstdio>

DWORD scratch;

EXCEPTION_DISPOSITION
__cdecl _except_handler(
    struct _EXCEPTION_RECORD* ExceptionRecord,
    void* EstablisherFrame,
    struct _CONTEXT* ContextRecord,
    void* DispatcherContext) {
  unsigned i;

  // Indicate that we made it to our exception handler
  printf("Hello from an exception handler\n");

  // Change EAX in the context record so that it points to someplace where we can successfully write
  ContextRecord->Eax = (DWORD) &scratch;

  // Tell the OS to restart the faulting instruction
  return ExceptionContinueExecution;
}

typedef EXCEPTION_DISPOSITION (*except_handler_ptr)(struct _EXCEPTION_RECORD*, void*, _CONTEXT*, void*);

int test_except_handler() {
  except_handler_ptr f = _except_handler;

  __asm
  {
    // Build EXCEPTION_REGISTRATION record:
    push f // Address of handler function
    push FS:[0] // Address of previous handler
    mov FS:[0], ESP // Install new EXECEPTION_REGISTRATION
  }

  // First SEH
  int*p = 0;
  *p = 1;

  // Second SEH
  __asm
  {
    mov eax, 0 // Zero out EAX
    mov[eax], 1 // Write to EAX to deliberately cause a fault
  }

  printf("After writing!\n");

  __asm
  {
    // Remove our EXECEPTION_REGISTRATION record
    mov eax,[ESP] // Get pointer to previous record
    mov FS:[0], EAX // Install previous record
    add esp, 8 // Clean our EXECEPTION_REGISTRATION off stack
  }

  return 0;
}