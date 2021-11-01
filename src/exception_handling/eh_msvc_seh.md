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

P.S> Compiler's `_try/_except` syntax, the compiler also builds the `EXCEPTION_REGISTRATION` struct on the stack.




[comment]: <> (0. An exception has occurred in a `THREAD_0`)

[comment]: <> (1. CPU interrupt `asm&#40;"int3"&#41;`)

[comment]: <> (2. OS process the interruption)

[comment]: <> (3. If the exception has occurred in a USER space:)

[comment]: <> (   1. Windows kernel do some staff)

[comment]: <> (   2. Pass the control flow to the `THREAD_0`.)

[comment]: <> (4. `THREAD_0` continues its execution:)

[comment]: <> (   1. Not from the point whrere the exceptions was thrown. )

[comment]: <> (   2. But from the function: `KiUserExceptionDispatcher&#40;NTDLL.DLL&#41; - exception dispatcher`)

[comment]: <> (5. `KiUserExceptionDispatcher` has all info about exception:)

[comment]: <> (   * `EXCEPTION_RECORD`)

[comment]: <> (   * `CONTEXT`)

[comment]: <> (`KiUserExceptionDispatcher` загружает цепочку обработчиков исключений&#40;об этом позже&#41; и поочерёдно вызывает их, пока исключение не будет обработано.)


## MSVC SEH (Structured Exception handling)
* Is a Microsoft extension to C to handle certain exceptional code situations, such as hardware faults, gracefully.
    * Although Windows and Microsoft C++ support SEH, we recommend that you use ISO-standard C++ exception handling.

  There are two **SEH** mechanisms:
1. Exception handlers `"__except" blocks` -> respond to or dismiss the exception.
2. Termination handlers `"__finally" blocks` -> are always called, whether an exception causes termination or not.

### try-except statement (Exception Handler)

```c++
// . . .
__try {
// guarded code
}
__except ( /* filter expression */ ) {
// termination code
}
// . . .
```
### try-finally statement (Termination Handler)

```c++
// . . .
__try {
// guarded code
}
__finally {
// termination code
}
// . . .
```

## MSVC Compiler keys

`/EHa`
1. Enables standard C++ stack unwinding.
2. Catches both **SEH (asynchronous)** and standard **C++ (synchronous)** exceptions when you use `catch(...)` syntax.
3. Overrides `/EHs` and `/EHc`.

`/EHs (doesn't catch SEH exceptions)`
1. Enables standard C++ stack unwinding.
2. Catches only standard C++ exceptions when you use `catch(...)` syntax.
3. Unless `/EHc` is also specified, the compiler assumes that functions declared as extern "C" may throw a C++ exception.

`/EHc`
1. When used with `/EHs`, the compiler assumes that functions declared as `extern "C"` never throw a C++ exception.
2. It has no effect when used with `/EHa` (that is, `/EHca` is equivalent to /EHa).
3. `/EHc` is ignored if `/EHs` or `/EHa` aren't specified.

`/EHr`
1. Tells the compiler to always generate runtime termination checks for all noexcept functions.
2. By default, runtime checks for noexcept may be optimized away if the compiler determines the function calls only non-throwing functions.
3. This option gives strict C++ conformance at the cost of some extra code. `/EHr` is ignored if `/EHs` or `/EHa` aren't specified.
