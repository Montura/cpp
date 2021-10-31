## [setjmp(env)| longjmp]
```c++
  jmp_buf env_;
  do {
    if (!setjmp(env_) { // TRY
      printf("In TRY statement\n");
      longjmp(env_, 1); // THROW
      printf("I do not appear\n");
    } else { // CATCH
      printf("Got an exception\n");
    }
  } while (0); // ETRY
  
  ETRY
```
1. When `setjmp(env_)` is called first time it:
   1. saves the Execution Context (**EC**) in a `jmp_buf` structure
   2. returns 0
2. When `longjmp(env_, 1)` is called (_non-local goto_):
   1. **EC** saved in `jmp_buf` is copied back to the CPU and computation starts over from the point of `setjmp(env_)` function.
    2. The returned value of `setjmp(env_)` is the one passed as **SECOND** argument to `longjmp(env_, 1)` function.

##### Performance drawbacks:
```c++
struct Resource {
  Resource() {};
  ~Resource() {};
};
```
1. `setjmp(env_)` must be called at the beginning of the every `"__try"-block` and the `List<jmp_buf*>` must be maintained.
2. The `List<Resource*>` on the stack must be maintained at all times and kept a consistent state with respect to the list of `List<jmp_buf*>`.
3. All the variables are stored in the registers and that are declared outside the `"__try"-block` have to be restored to their initial values when `longjmp(env_, 1)` is invoked.

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

A Termination Handler is executed no matter how the `__try` statement block is terminated. Causes include _**jumping**_ out of the `__try` block, a `longjmp` statement that transfers control out of the block, and unwinding the stack due to exception handling.

When the cause for interruption is an exception, the system must first execute the filter portion of one or more exception handlers before deciding what to terminate. The order of events is:

1. An exception is raised.
2. The system looks at the hierarchy of active exception handlers and executes the filter of the handler with highest precedence. That's the exception handler most recently installed and most deeply nested, going by blocks and function calls.

3. If this filter passes control (returns 0), the process continues until a filter is found that doesn't pass control.

4. If this filter returns -1, execution continues where the exception was raised, and no termination takes place.

5. If the filter returns 1, the following events occur:

    * The system unwinds the stack: It clears all stack frames between where the exception was raised and the stack frame that contains the exception handler.

    * As the stack is unwound, each termination handler on the stack is executed.

    * The exception handler itself is executed.

    * Control passes to the line of code after the end of this exception handler.

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


[comment]: <> (##  Zoom up the stack)

[comment]: <> (![]&#40;https://github.com/simpleton/stack-unwind-samples/raw/master/art/function_frame_big_pic.svg&#41;)

[comment]: <> (**EBP** and **ESP** point to the base and top of the stack frame of currently executing function. All other stack frames save **EBP** and **EIP** values before transferring control to another function.)

[comment]: <> (![]&#40;https://github.com/simpleton/stack-unwind-samples/raw/master/art/frame_stack.svg&#41;)
