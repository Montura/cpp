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