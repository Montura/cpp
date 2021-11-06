## A Crash Course on the Depths of Win32™ Structured Exception Handling

###### Inspired by: [Matt Pietrek](https://bytepointer.com/resources/pietrek_crash_course_depths_of_win32_seh.htm)

#### SEH in the Buff
1. When a thread faults, the operating system calls a user-defined callback function.
2. This prototype, which comes from the standard Win32 header file `excpt.h`.
```c++
EXCEPTION_DISPOSITION
__cdecl _except_handler(
   struct _EXCEPTION_RECORD *ExceptionRecord,
   void * EstablisherFrame,
   struct _CONTEXT *ContextRecord,
  void * DispatcherContext
);
```
3. It receives a wealth of information and decide what to do.

How does the OS know where to call when a fault occurs?
The answer is an `EXCEPTION_REGISTRATION`.

```c
typedef struct _EXCEPTION_REGISTRATION {
  struct _EXCEPTION_REGISTRATION *prev; // ptr to the previous handler's EXCEPTION_REGISTRATION
  PEXCEPTION_ROUTINE handler; // ptr to function-handler
};
```

In `Win32` the pointer to the last `EXCEPTION_REGISTRATION` is stored in `TIB (Thread Information Block)`.
```c
// from #include <winnt.h>
typedef struct _NT_TIB32 {
   DWORD ExceptionList;
   DWORD StackBase;
   DWORD StackLimit;
   DWORD SubSystemTib;
   DWORD FiberData;
   DWORD ArbitraryUserPointer;
   DWORD Self;
} NT_TIB32,*PNT_TIB32;
```

1. `DWORD ExceptionList` points to the `EXCEPTION_REGISTRATION` of the current thread.
2. Register `FS` points to the `_NT_TIB32` of the current thread, so the `EXCEPTION_REGISTRATION` pointer can be found at address: `FS:[0]`

![](https://bytepointer.com/resources/pietrek_crash_course_depths_of_win32_seh_files/pietrek2.jpg)

1. Create an `EXCEPTION_REGISTRATION` structure on the stack via two `PUSH` instructions:
    1. `PUSH handler` (saves my_exception_handler     as `EXCEPTION_REGISTRATION.handler`)
    2. `PUSH FS:[0]`  (saves the prev val of `FS:[0]` as `EXCEPTION_REGISTRATION.prev`)

2. Now there's an 8-byte `EXCEPTION_REGISTRATION` structure on the stack.

2. `MOV FS:[0],ESP` set `_NT_TIB32.ExceptionList` to point at the new `EXCEPTION_REGISTRATION`.

```c
DWORD handler = (DWORD)_except_handler;

// Build EXCEPTION_REGISTRATION record:
__asm {     
  push handler // Address of handler function
  push FS:[0] // Address of previous handler
  mov FS:[0],ESP // Install new EXECEPTION_REGISTRATION
}
  
 __asm
 {
     mov eax,0 // Zero out EAX
     mov [eax], 1 // Write to EAX to deliberately cause a fault
 }
  
 printf( "After writing!\n" );
  
 __asm
 {
     // Remove our EXECEPTION_REGISTRATION record
     mov eax,[ESP] // Get pointer to previous record
     mov FS:[0], EAX // Install previous record
     add esp, 8 // Clean our EXECEPTION_REGISTRATION off stack
 }
```

P.S> Compiler's `__try/__except` syntax, the compiler also builds the `EXCEPTION_REGISTRATION` struct on the stack.

### There are two MSVC SEH (Structured Exception handling) mechanisms:
1. **_Exception handlers_** `"__except" blocks` -> respond to or dismiss the exception.
2. **_Termination handlers_** `"__finally" blocks` -> are always called, whether an exception causes termination or not.

#### 1. Try-except statement (exception handlers)
```c++
__try { /* guarded code */ }
__except ( /* filter expression */ ) {  /* termination code */ }
```
#### 2. Try-finally statement (termination handlers)
```c++
__try { /* guarded code */ }
__finally { /* termination code */ }
```

### Visual C++ SEH x86

* Keywords `__try`, `__except` and `__finally` can be used for compiler-level `SEH` support
* The compiler uses:
  1. **_A single exception handler per all functions_** with `SEH`
  2. But different supporting structures (**_scope tables_**) per function
* The `SEH` **_handler registration frame is placed on the stack_**


* **_ExceptionHandler_** points to `__except_handler3` (`SEH3`) or `__except_handler4` (`SEH4`)
* The frame set-up is often delegated to compiler helper (`__SEH_prolog`/`__SEH_prolog4/etc`)
* **_ScopeTable_** points to a table of entries describing all `__try` blocks in the function