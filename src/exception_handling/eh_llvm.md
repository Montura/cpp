# Exception Handling
1. [Itanium ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html) `Zero-cost` Exception Handling
2. [`Setjmp/Longjmp`](eh_sj_lj.md) Exception Handling
3. `Windows Runtime` Exception Handling

### [Overview LLVM](https://llvm.org/docs/ExceptionHandling.html#introduction)
When an exception is thrown in LLVM code, the runtime:

1. Has to find in the current compile unit an `Exception Frame` corresponding to the function where the exception was thrown.
2. `Exception Frame` contains a reference to an `Exception Table`.
   
  The exception handling ABI is defined by `Personality Function` (`__gxx_personality_v0`) 
```c++
_Unwind_Reason_Code __gxx_personality_v0 (int version,
    _Unwind_Action actions,
    uint64_t exceptionClass,
    _Unwind_Exception* unwind_exception, // an exception structure containing the exception object type and value
    _Unwind_Context* context // the context of the exception + reference to the exception table for the current function
)
```
`Exception Table` - series of code ranges defining what to do if an exception occurs in that range:
* C++ std::type_info defines which types of exception objects can be handled in current range, and an `associated action` that should take place. `Actions` typically pass control to a `landing pad`
* `Landing pad` corresponds roughly to the code found in the `catch` portion of a `try/catch` block. It receives an `exception structure` and the `type of exception` thrown that used to determine which `catch` should actually process the exception.

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

`Landing pad` - the place where an `invoke` continues after an exception is thrown:
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

## `Itanium EH` - idea of **_“successive unwinding"_**

Throwing an exception typically involves **_allocating thread local memory_** to hold the exception, and calling into the `EH runtime` that:
1. Identifies frames with appropriate exception handling actions
2. Successively resets the register context of the current thread to the most recently active frame with actions to run. 
3. Execution resumes at a `landingpad` instruction, which produces register values provided by the runtime.
4. If a function is only cleaning up allocated resources, the function is responsible for calling `_Unwind_Resume` to transition to the next most recently active frame after it is finished cleaning up.
5. Eventually, the `frame` responsible for handling the exception calls `__cxa_end_catch` to:
    1. Destroy the exception
    2. Release its memory
    3. Resume normal control flow

## `Windows Runtime`

* The Windows EH model **_does not use these successive register context resets._** 
* Instead, the **_active exception is typically described by a frame on the stack._**
  
1. The exception object is allocated in stack memory and its address is passed to `__CxxThrowException`
2. General purpose `structured exceptions (SEH)` are:
   * more analogous to Linux signals
   * dispatched by userspace DLLs provided with Windows 
3. Each frame **_on the stack_** has an `assigned EH personality routine`, which decides what actions to take to handle the exception:
   * `EH personality routines`: 
     * C++ personality: `__CxxFrameHandler3`
     * SEH personalities: `_except_handler3`, `_except_handler4`, and `__C_specific_handler`
   * All of them implement cleanups by calling back into a `funclet` contained in the parent function.

### `Funclets`:
* Are regions of the parent function
* Can be called as though they were a function pointer with a very special calling convention
  
* The frame pointer of the parent frame is passed into the `funclet` either using the standard `EBP` register or as the first parameter register, depending on the architecture.

* The `funclet` implements the EH action by accessing local variables in memory through the frame pointer, and returning some appropriate value, continuing the EH process.

* No variables live in to or out of the `funclet` can be allocated in registers.

#### Using funclets

* The C++ personality also uses `funclets` to contain the code for catch blocks.

* The runtime must use `funclets` for catch bodies because the C++ exception object is allocated in a **_child stack frame_** of the function handling the exception. If the runtime rewound the stack back to frame of the catch, the memory holding the exception would be overwritten quickly by subsequent function calls.
  
* The use of `funclets` also allows `__CxxFrameHandler3` to implement rethrow without resorting to `TLS`. Instead, the runtime throws a special exception, and then uses `SEH (__try / __except)` to resume execution with new information in the child frame.

#### NOTE
The successive unwinding approach is incompatible with Visual C++ exceptions and general purpose Windows exception handling because:
1. The C++ exception object lives in stack memory
2. LLVM cannot provide a custom personality function that uses landingpads
3. Similarly, SEH does not provide any mechanism to rethrow an exception or continue unwinding. 