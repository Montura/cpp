# [Exception Handling LLVM overview](https://llvm.org/docs/ExceptionHandling.html#introduction)
1. [`Setjmp/Longjmp`](eh_gcc_sj_lj.md)
2. [`Zero-cost (LSDA)`](eh_gcc_zero_cost.md)
3. [`Windows Runtime SEH x86`](eh_msvc_SEH_win32.md)
4. [`Windows Runtime C++ EH x64`](eh_msvc_cxx_EH_x64.md)

## Overview LLVM
When an exception is thrown in LLVM code, the runtime:

1. Has to find in the current compile unit an **_Exception Frame_** corresponding to the function where the exception was thrown.
2. **_Exception Frame_** contains a reference to an **_Exception Table_**.
   
  The exception handling ABI is defined by `Personality Function` (`__gxx_personality_v0`) 
```c++
_Unwind_Reason_Code __gxx_personality_v0 (int version,
    _Unwind_Action actions,
    uint64_t exceptionClass,
    _Unwind_Exception* unwind_exception, // an exception structure containing the exception object type and value
    _Unwind_Context* context // the context of the exception + reference to the exception table for the current function
)
```
**_Exception Table_** - series of code ranges defining what to do if an exception occurs in that range:
* C++ `std::type_info` defines:
  1. Which types of exception objects can be handled in current range
  2. An **_associated action_** that should take place.
* **_Actions_** typically pass control to a **_landing pad_**
* **_Landing pad_**:
  1. Corresponds roughly to the code found in the `catch` portion of a `try`/`catch` block.
  2. Receives:
     1. An **_exception structure_**
     2. The **_type of exception_** thrown (used to determine which `catch` should actually process the exception)

### LLVM Code generation
#### `Throw` breaks down into two steps:

1. `__cxa_allocate_exception` -> to allocate exception space for an exception structure. 
2. `__cxa_throw` -> to raise the exception, passing the exception structure as an argument.

```c++
  void* __cxa_allocate_exception(size_t thrown_size);
  void __cxa_throw(void* thrown_exception, std::type_info *tinfo, void (* dest)(void*));
```

* The type of the exception is represented using a C++ RTTI structure: `std::type_info`.

* The type of an exception object is `__cxa_exception`

### `Try/Catch`

The LLVM C++ front-end replaces the `call` with an `invoke` instruction. Unlike a call, the invoke has two potential continuation points:

1. Where to continue when the call succeeds as per normal
2. Where to continue if the call raises an exception, either by a throw or the unwinding of a throw

**_Landing pad_** - the place where an **_invoke_** continues after an exception is thrown:
* It's an alternative function entry point takes two args:
    * An exception structure reference
    * A type info index
* It saves the exception structure reference 
* Then proceeds to select the catch block that corresponds to the type info of the exception object

Finally, the entry and exit of catch code is bracketed with calls to `__cxa_begin_catch` and `__cxa_end_catch`.

1. `__cxa_begin_catch` takes an exception structure reference as an argument and returns the value of the exception object.

2. `__cxa_end_catch` takes no arguments and do:
    1. Increment's the exception's handler count.
    2. Places the exception on the stack of currently-caught exceptions if it is not already there, linking the exception to the previous top of the stack.
    3. Decrements the uncaught_exception count.
    4. Returns the adjusted pointer to the exception object.
```c++
void* __cxa_begin_catch(void *exceptionObject);
void cxa_end_catch();
```

### Cleanups
`Cleanup` is extra code which needs to be run as part of unwinding a scope:
* C++ destructors are a typical example
*  A `landing pad` may need to run arbitrary amounts of cleanup code before actually entering a `catch` block. 
 
### Asm Table Formats
#### `Exception Handling Frame (EHF)`
In LLVM `eh_frame`:
- is very similar to the unwind frame used by `DWARF` debug info. 
- contains all the information necessary to tear down the current frame and restore the state of the prior frame. 
- One `EHF` per each function in a TU (translation unit)
- One common `EHF` that defines information common to all functions in the TU unit
- The format of this call frame information `(CFI)` is often platform-dependent 

### Exception Tables
`Exception Table` contains information about what actions to take when an exception is thrown in a particular part of a function’s code. This is typically referred to as the `language-specific data area (LSDA)`. 
* The format of the LSDA table is specific to the personality function `ex. __gxx_personality_v0`
* There is one `Exception Table` per function, except:
    * leaf functions
    * functions that have calls only to non-throwing functions

[comment]: <> (## `Itanium EH` - idea of **_“successive unwinding"_**)

[comment]: <> (Throwing an exception typically involves **_allocating thread local memory_** to hold the exception, and calling into the `EH runtime` that:)

[comment]: <> (1. Identifies frames with appropriate exception handling actions)

[comment]: <> (2. Successively resets the register context of the current thread to the most recently active frame with actions to run. )

[comment]: <> (3. Execution resumes at a `landingpad` instruction, which produces register values provided by the runtime.)

[comment]: <> (4. If a function is only cleaning up allocated resources, the function is responsible for calling `_Unwind_Resume` to transition to the next most recently active frame after it is finished cleaning up.)

[comment]: <> (5. Eventually, the `frame` responsible for handling the exception calls `__cxa_end_catch` to:)

[comment]: <> (    1. Destroy the exception)

[comment]: <> (    2. Release its memory)

[comment]: <> (    3. Resume normal control flow)

[comment]: <> (## `Windows Runtime`)

[comment]: <> (* The Windows EH model **_does not use these successive register context resets._** )

[comment]: <> (* Instead, the **_active exception is typically described by a frame on the stack._**)
  
[comment]: <> (1. The exception object is allocated in stack memory and its address is passed to `__CxxThrowException`)

[comment]: <> (2. General purpose `structured exceptions &#40;SEH&#41;` are:)

[comment]: <> (   * more analogous to Linux signals)

[comment]: <> (   * dispatched by userspace DLLs provided with Windows )

[comment]: <> (3. Each frame **_on the stack_** has an `assigned EH personality routine`, which decides what actions to take to handle the exception:)

[comment]: <> (   * `EH personality routines`: )

[comment]: <> (     * C++ personality: `__CxxFrameHandler3`)

[comment]: <> (     * SEH personalities: `_except_handler3`, `_except_handler4`, and `__C_specific_handler`)

[comment]: <> (   * All of them implement cleanups by calling back into a `funclet` contained in the parent function.)

[comment]: <> (### `Funclets`:)

[comment]: <> (* Are regions of the parent function)

[comment]: <> (* Can be called as though they were a function pointer with a very special calling convention)
  
[comment]: <> (* The frame pointer of the parent frame is passed into the `funclet` either using the standard `EBP` register or as the first parameter register, depending on the architecture.)

[comment]: <> (* The `funclet` implements the EH action by accessing local variables in memory through the frame pointer, and returning some appropriate value, continuing the EH process.)

[comment]: <> (* No variables live in to or out of the `funclet` can be allocated in registers.)

[comment]: <> (#### Using funclets)

[comment]: <> (* The C++ personality also uses `funclets` to contain the code for catch blocks.)

[comment]: <> (* The runtime must use `funclets` for catch bodies because the C++ exception object is allocated in a **_child stack frame_** of the function handling the exception. If the runtime rewound the stack back to frame of the catch, the memory holding the exception would be overwritten quickly by subsequent function calls.)
  
[comment]: <> (* The use of `funclets` also allows `__CxxFrameHandler3` to implement rethrow without resorting to `TLS`. Instead, the runtime throws a special exception, and then uses `SEH &#40;__try / __except&#41;` to resume execution with new information in the child frame.)

[comment]: <> (#### NOTE)

[comment]: <> (The successive unwinding approach is incompatible with Visual C++ exceptions and general purpose Windows exception handling because:)

[comment]: <> (1. The C++ exception object lives in stack memory)

[comment]: <> (2. LLVM cannot provide a custom personality function that uses landingpads)

[comment]: <> (3. Similarly, SEH does not provide any mechanism to rethrow an exception or continue unwinding. )