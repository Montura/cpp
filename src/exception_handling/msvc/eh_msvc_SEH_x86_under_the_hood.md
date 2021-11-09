# [How a C++ compiler implements exception handling](https://www.codeproject.com/Articles/2126/How-a-C-compiler-implements-exception-handling)
`EXCEPTION_REGISTRATION` structure is used to register the **_exception callback_** with the OS that it calls when
exception occurs.
```c++
struct EXCEPTION_REGISTRATION {
   EXCEPTION_REGISTRATION *prev;
   DWORD handler; // EXCEPTION HANDLER (EHr)
   int   id;
   DWORD ebp;
};
```
* VC++ creates `EXCEPTION_REGISTRATION`  for every function as its **_local variable_**.
* The last field of the structure overlaps the location where frame pointer `EBP` points.
* The **_prologue_** creates this structure on its stack frame and registers it with the OS.
* The **_epilogue_** restores the `EXCEPTION_REGISTRATION` of the `caller`.


#### When VC++ compiles a function, it generates two set of data for the function:
1. **_Exception callback_** function.
2. `FuncInfo` structure that contains info about:
1. The catch blocks and their addresses
2. The type of exception they are interested in catching
3. etc

### How things look like at runtime when considering exception handling

* Widget's **_exception callback_** is at the head of the exception chain pointed to by `FS:[00]`
* Since every thread has its own stack, this means that every thread has its own list of `EXCEPTION_REGISTRATION` structures pointed to by `FS:[00]`


* `EHer (Widget_exc_handler)` passes `widget's FuncInfo` structure's address to `__CxxFrameHandler`
  function, which inspects `FuncInfo` to see if there is any `catch block` in the function interested in catching the current exception.
* If it does not find any, it returns `ExceptionContinueSearch` value back to the OS.
* OS gets the next node off the **_EH list_** and calls its `EHer` (which is the handler for the `caller` of the current function).

![](https://www.codeproject.com/KB/cpp/exceptionhandler/figure4.gif)

* This continues until the `EHer` finds the `catch block` interested in catching the exception in which case it does not return back to the OS.

### If `catch block` is found in `FuncInfo`
1. But before `EHer` calls the catch block, it must perform _stack unwinding_: **_cleaning up the stack frames_** of the functions below this function's frame.

#### IDEA: cleaning up the stack frames
The `EHer`:
1. Must find all the local objects of the function alive on the frame at the time of the exception and call their destructors.
2. Delegates the task of cleaning the frame to the `EHer` associated with that frame.

#### IMPL: cleaning up the stack frames
1. It starts from the beginning of the `EH list` pointed to by `FS:[00]` and calls the `EHer` at each node, telling it that the stack is being unwound.
2. In response, the handler calls destructor for all the local objects on the frame and returns. This continues until it arrives at the node that corresponds to itself.

#### A moment before `EHer` calls the `catch block`
Since `catch block` is part of a function, it uses the stack frame of the function to which it belongs.

1. The `EHer` needs to **_activate_** its stack frame before calling the `catch block`.
2. The `catch block` accepts one argument: the type of exception it is willing to catch.
3. `EHer` **_should copy the exception object or its reference to catch block's frame_**.
4. After copying the exception and activating the frame, `EHer` calls the `catch block`.

#### `EHer` calls the `catch block`
* `Catch block` returns to `EHer` the **_Return Address_** where control should be transferred in the function after `try-catch block`.
* **At this moment**:
  1. The *stack unwinding has occurred*
  2. The *frames have been cleaned up*
* **But** the frames **have not been removed and they still physically occupy the space on the stack**.  This is because the `EHer` is still executing and using the stack for its local objects, its frame present below the last function's frame from where the exception originated.

#### When `catch block` returns
* It needs to destroy the exception.
* After this point the `EHer`:
  1. Removes all the frames including its own by setting the `ESP` at the end of the function's frame (to which it has to transfer the control)
  2. Transfers control at the end of `try-catch block`.
* How does the `EHer` know where the end of function's frame is?
  * The compiler saves it (through function's prologue) on function's stack frame for the `EHer` to find it.
  * See figure 4. It is sixteen bytes below the stack frame pointer EBP.

#### Catch Block may itself throw a new exception or rethrow the same exception
* `EHer` has to watch for the next situations and take appropriate action:
  1. If `catch block` throws a **_new exception_**, the `EHer` has **_to destroy the old exception_**.
  2. If `catch block` specifies a **_rethrow_**, then the `EHer` has **_to propagate the old exception_**.

### `FuncInfo` structure
![](https://www.codeproject.com/KB/cpp/exceptionhandler/figure5.gif)

When `EHer` has to search for a `catch block` in a function, the first thing it has to determine is whether the point from where the exception originated from within a function has an enclosing `try block` or not. 

* If it does not find any `try block`, then it returns back.
* Otherwise, it searches the list of `catch blocks` associated with the enclosing `try block`.

#### `TryBlock` structure
1. At compile time, the compiler assigns each `try block` a **_start id_** and **_end id_**.
2. The compiler generates `TryBlock` data structure (_tryblock 2_) for each `try block` with in the function.
3. At the time of exception, the `EHer`:
   1. Reads `EXCEPTION_REGISTRATION::id` from the frame to **_id_**
   2. Checks the `TryBlock` structure to see if the **_id_** is equal to or falls in **_[start id, end id]_**.
* If it does, then the exception originated from with in this `try block`.
* Otherwise, it looks at the next `TryBlock` structure in `TryBlockTable`.

#### `CatchBlock` structure
Once exception handler finds the `try block`, it can traverse the `catchblock` table associated with the `try block` to see if any `catch block` is interested in catching the exception. 

NOTE: 
* In case of `nested try blocks`, the exception that originated from `inner try block` also originated from `outer try block`.
* In `TryBlockTable` VC++ puts `inner try block` before the `outer try block`.

The `EHer` should first look for the `catch blocks` for the `inner try block`:
* If none is found, then it looks for the `catch blocks` of the `outer try block`.

### `Catch block` arg and the current exception

The catch block catches the exception if **_H_** and **_E_** are the exact same type.
```c++
void foo() {
   try {
      throw E(); // compiler generates excpt_info structure for E.
   } catch(H) { /* ... */ }
}
```
![](https://www.codeproject.com/KB/cpp/exceptionhandler/figure6.gif)

#### The `EHer`:
1. Has to compare the types at runtime (`RTTI`, `std::type_info`).
2. Knows about the type of the `catch block's` parameter from `FuncInfo` structure. 

#### The compiler:
1. Generates `excpt_info` structure for the exception thrown
2. Exception's `std::type_info` is available through `excpt_info` structure
3. Makes available to the `EHer`:
   1. Exception's destructor (destroy after the catch block is invoked)
   2. A table of `extented type_info` (for exception types hierarchy):
      1. `std::type_info`
      2. Copy constructor (before invoking the catch block)
      3. Size

* If the `catch block's` parameter type is a _**base class**_ and the exception is its **_derived class_**:
  1. These `typeinfo's` are not the same. 
  2. Anyway the `EHer` should still invoke that `catch block`. 

* So the `EHer` compares the `type_info` of the `catch block's` parameter with all the `type_info's` available through the `excpt_info` structure.

### `Throw` statement
VC++ translates `throw` statement into something like:

```c++
/* throw Exception();*/
/* -> 1. Compiler generates `excpt_info` structure for Exception. */
/* -> 2. */ Exception e = Exception();   // create exception on the stack
/* -> 3. */ _CxxThrowException(&e, E_EXCPT_INFO_ADDR);

extern "C"
void __stdcall _CxxThrowException(
    void* pExceptionObject,
    _ThrowInfo* pThrowInfo
);
```

1. `_CxxThrowException` passes control to the OS (through function `RaiseException`) passing it both of its parameters.
```c++
void RaiseException(
  [in] DWORD           dwExceptionCode,
  [in] DWORD           dwExceptionFlags,
  [in] DWORD           nNumberOfArguments,
  [in] const ULONG_PTR *lpArguments
);
```
2. The OS:
   1. Packages these two parameters in `_EXCEPTION_RECORD` structure.
   2. Looks at the TIB (`FS:[00]`) of the faulting thread and retrieves a PTR to an `EXCEPTION_REGISTRATION` structure. 
   3. Every function creates its own `EXCEPTION_REGISTRATION` on its stack frame and registers it.
   4. Calls the `ExceptionHandler` with the `EXCEPTION_REGISTRATION` argument:
      1. `EXCEPTION_REGISTRATION::id` is used by `EHer` to finding the `catch block`.
      2. `EHer` aware of:
         1. The function's stack frame (useful for cleaning the stack frame).
         2. The position of the `EXCEPTION_REGISTRATION` node on the exception list (useful for stack unwinding).

![](https://bytepointer.com/resources/pietrek_crash_course_depths_of_win32_seh_files/pietrek2.jpg)


[ExceptionHandler(_CxxFrameHandler3) implementation](https://github.com/ojdkbuild/tools_toolchain_vs2017bt_1416/blob/master/VC/Tools/MSVC/14.16.27023/crt/src/vcruntime/risctrnsctrl.cpp#L255)
```c++
// __CxxFrameHandler3 - Real entry point to the runtime
EXCEPTION_DISPOSITION (*__CxxFrameHandler3)(
    _EXCEPTION_RECORD *ExcRecord,  // Information for this exception
    void * EstablisherFrame,       // Dynamic information for this frame
    _CONTEXT *ContextRecord,       // Context info
    void * DispatcherContext     // More dynamic info for this frame
);
```

* If the `EHer` cannot find `catch block`, it returns `ExceptionContinueSearch` value back to the OS. 
* `_EXCEPTION_RECORD` structure is defined in `WINNT.H` as:
```c++
struct _EXCEPTION_RECORD {
  DWORD ExceptionCode; 
  DWORD ExceptionFlags; 
  _EXCEPTION_RECORD *ExcRecord;
  PVOID   ExceptionAddress; 
  DWORD NumberParameters;
  DWORD ExceptionInformation[15]; 
} EXCEPTION_RECORD;
```
* If (`ExceptionCode` == `0xe06d7363`) then:
  1. The exception occurs due to `throw`
  2. The exception represents a C++ exception
  3. The `ExceptionInformation array` contains pointer to the exception and the `excpt_info` structure.

* `EHer` looks at the `ExceptionFlags` to determine what action to take:
  * If the value is `EH_UNWINDING`, that is an indication to the `EHer` that the stack is being unwound and it should clean its stack frame and return.
  * Otherwise, `EHer` has to search for the `catch block` in the function and invoke it if it is found.

```c++
/* throw Exception();*/
// 1. Compiler generates `excpt_info` structure for Exception.
// 2. Exception e = Exception(); // create exception on the stack
// 3. _CxxThrowException(&e, E_EXCPT_INFO_ADDR);
// 4. -> RaiseException()
// 5.  -> OS creates `_EXCEPTION_RECORD record {&e, E_EXCPT_INFO_ADDR}
// 6.   -> OS call ExceptionCallback (it starts from FS:[00])
// 7.     -> FS:[00] points to the first node of `EXCEPTION_REGISTRATION` list
// 8.     -> Calls ExceptionHandler() for the first list node
```