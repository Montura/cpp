## `Itanium EH - idea of “successive unwinding"`

Sources:
1. [Itanium C++ ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html)
2. [C++ exception handling ABI](https://maskray.me/blog/2020-12-12-c++-exception-handling-abi)

Throwing an exception **_allocating thread local memory_** to hold the exception.

1. Call `__cxa_allocate_exception` to allocate space to store the exception object and the exception header `__cxa_exception`
2. Jump to `__cxa_throw` and set the `__cxa_exception` fields
```c++
// Level 1
struct _Unwind_Exception {
  _Unwind_Exception_Class exception_class; // an identifier, used to tell whether the exception is native
  _Unwind_Exception_Cleanup_Fn exception_cleanup;
  _Unwind_Word private_1; // zero: normal unwind; non-zero: forced unwind, the _Unwind_Stop_Fn function
  _Unwind_Word private_2; // saved stack pointer
} __attribute__((aligned));
```
3. Jump to `_Unwind_RaiseException` (in `libgcc_s.so`) to perform stack unwinding:
    1. Phase 1 `SEARCH_PHASE`
    2. Phase 2 `CLEANUP_PHASE`
```c++
void _Unwind_Reason_Code _Unwind_RaiseException(_Unwind_Exception *obj);
```
* In the `SEARCH_PHASE` find matched `catch` _**handler**_ and **_record the stack pointer_** in `_Unwind_Word private_2` (using `std::type_info`):
    1. Trace the call chain based on `IP`/`SP` and other saved registers
    2. **_For each stack frame_**:
        1. Skip if there is no personality routine;
        2. The stack frame marked as `_UA_HANDLER_FRAME`
        2. Call **_personality_** and if it returns:
            - `_URC_CONTINUE_UNWIND` -> **continue searching**
            - `_URC_HANDLER_FOUND` -> found a matched catch handler and **the search stops**
            - `_URC_END_OF_STACK` -> **an unmatched exception specification is found**


* In the `CLEANUP_PHASE` jump to `non-catch` **_handlers_** (usually **_local variable destructors_**), and then transfer the control to the **_matched_** `catch` **_handler_** located in the `SEARCH_PHASE`
    1. Trace the call chain based on `IP`/`SP` and other saved registers
    2. **_For each stack frame_**:
        1. Skip if there is no personality routine;
        2. Call **_personality_** and if it returns:
            - `_URC_CONTINUE_UNWIND` -> there is **no landing pad**, _continue to unwind_
            - `_URC_INSTALL_CONTEXT` -> there is **a landing pad**, _jump to the landing pad_
    5. For **`intermediate stack frames`** the **_landing pad_** performs:
        1. **_Cleanup Work_** (if they are **NOT marked** during the `SEARCH_PHASE`):
            * Usually call destructors of out-of-scope variables
            * Calls `_Unwind_Resume` to jump back to the `CLEANUP_PHASE`
        2. **_Catch-Block Work_** (if they are **marked** as `_UA_HANDLER_FRAME` during the `SEARCH_PHASE`):
            - Calls `__cxa_begin_catch`
            - Then executes the code in the `catch` block
            - Calls `__cxa_end_catch` to destroy the exception object

_**Note**_: each stack frame may use a different personality routine. It is common that all frames share the same routine, though.

### Different personality routines`__gxx_personality_v0`:

```c++
void __gxx_personality_v0(...);   // C++
void __gxx_personality_sj0(...);  // sjlj
void __gcc_personality_v0(...);   // C -fexceptions for __attribute__((cleanup(...)))
void __CxxFrameHandler3(...);     // Windows MSVC
void __gxx_personality_seh0(...); // MinGW-w64 -fseh-exceptions
void __objc_personality_v0(...);  // ObjC in the macOS environment
```