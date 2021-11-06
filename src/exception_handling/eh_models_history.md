#[Exception handling in LLVM, from Itanium to MSVC](https://llvm.org/devmtg/2015-10/slides/KlecknerMajnemer-ExceptionHandling.pdf)

##Two implementation strategies in 1989:
 - **_Portable exception handling (`PEH or SjLj`)_**:
    - Built on linked lists and `setjmp/longjmp`
    - Ideal for `C` transliteration (`CFront`)
    - Interoperates across EH-unaware code produced by other vendors
 - _**Efficient exception handling**_:
    - Built on `PC lookup` tables that determine which `EH actions` to take
    - Requires reliable `stack unwinding mechanism`
    - Need call frame information (`CFI`) to restore non-volatile registers and locate `return addresses`


|   Method |   Based on|
|:-----------:|:------------------------------:|
|  Portable exception handling (PEH) |  _**Stack**_ |
| Efficient exception handling (EH) |  _**Table**_  |

### Borland implements C++ and SEH in 1993
   - Implementation approach similar to `PEH` described in 1989
   - `Windows toolchain` ecosystem was diverse, needed interoperability
   - `SEH` allowed:
     1. Recovering from CPU traps (integer divide by zero, etc)
     2. Resuming in the trapping context:
        - Usable for virtual memory tricks or making divide by zero produce a value
   - `Microsoft` adopted `SEH for Windows`, `fs:00` becomes TLS slot for `EH`



 - Different vendors made different choices:

|   Vendor    | Portable exception handling (PEH) | Efficient exception handling (EH) |
|:-----------:|:---------------------------------:|:---------------------------------:|
| GCC or LLVM | `setjump/longjump`                | Zero-cost (`DWARF`, `LSDA`)       |
|  MSVC       | `SEH`                             | Zero-cost (`.pdata`)              |


## Borland implements C++ and SEH in 1993
  - Implementation approach similar to `PEH` described in 1989
  - `SEH` allowed:
    1. Recovering from `CPU` traps (integer divide by zero, etc)
    2. Resuming in the trapping context:
       - Usable for virtual memory tricks or making divide by zero produce a value 
  - `Microsoft`
    - adopted `SEH for Windows`, `FS:00` becomes `TLS` slot for `EH`
  - `GCC toolchain`
    - implemented specific functions are called at entry and exit `_Unwind_SjLj_Register`/`_Unwind_SjLj_Unregister` (instead of using list in `FS:00`)

## HP landingpad model for Itanium
- `HP` had years of experience getting C++ EH right in multiple compilers:
  - Major user of CFront, eventually transitioned to aC++
- `HP` popularized the _**landingpad model**_ through the `Itanium C++ ABI`
- Uses **_successive unwinding_**: restores the register context of each frame on
the stack with cleanups until the right catch is reached: 
  - Major departure from ‘89 models, which both pinned objects with destructors in memory (linked list of **_allocated objects_** is supported with the list of `jmb_buf`)
- **_Language-specific data area_** (`LSDA`) contains two tables:
  - **_Call site table_**: map from `PC` range to **_landingpad_** label plus action table index
  - **_Action table_**: array of type information references and next action chains
  - At most one **_landingpad_** label per call
- `GCC` and `LLVM` use `Itanium C++ ABI`

## Windows exception handling model (doesn't use landingpads)

- Tables map from program state number to `funclet` pointers:
- State number tracked through `PC tables` and explicitly in memory
- Each `funclet` shares the parent frame via `EBP`/`RBP`
  - Runtime provides the **_establishing frame pointer_** via regparm 
  - `Funclet` assumes `SP` has dynamically changed, similar to dynamic alloca
- `Funclet`s implement three major actions:
  1. **_SEH filter_**: Should this exception be caught, retried, or propagated outwards
  2. **_Cleanup_**: Cleanup code, like C++ destructor calls or finally blocks
  3. **_Catch_**: User code from the catch block body


## Itanium
### Itanium exception handling phases:
1. Call `__cxa_allocate_exception` to **_allocate thread local memory_** to hold the **_exception object_** and the exception header `__cxa_exception`
2. Jump to `__cxa_throw`, set the `__cxa_exception` fields and then jump to `_Unwind_RaiseException`
3. In `_Unwind_RaiseException` execute the `SEARCH_PASE`:
   - Call **_personality routine_** to find matching try catch (`std::type_info` matching)
4. In `_Unwind_RaiseException` execute the `CLEANUP_PHASE`:
   - Call **_personality routine_** to find stack frames containing out-of-scope variables
   - For each stack frame, jump to its landing pad to execute the constructors:
        - The landing pad uses `_Unwind_Resume` to resume the `CLEANUP_PHASE`
5. The `CLEANUP_PHASE` executed by `_Unwind_RaiseException` jumps to the **_landing pad_** corresponding to the matching `try/catch`
6. The landing pad:
   1. Calls `__cxa_begin_catch`
   2. Executes the `catch - block`
   3. Calls `__cxa_end_catch` 
7. `__cxa_end_catch destroys`:
    1. Destroy the `exception object`
    2. Release its memory
    3. Resume normal control flow

## Windows
### Windows exception handling phases
1. Exception is raised to OS
2. Walk stack, **_call each personality**_ until the exception is claimed
   - The `SEH` and `CLR` personalities call **_active filter funclets_** during this phase
3. Call each **_personality again_** to run **_cleanups_**
   - **_Personality_** controls what happens if cleanups raise an exception
4. **_Personality_** of catching frame handles the exception
   - C++ **_personality_** calls **_catch funclet_**, uses `SEH` to detect C++ rethrow
5. **_Personality_** resets register context to the parent frame

### Windows exception handling implications
- Contrast to successive unwinding: **_only one register context reset_**
- All `EH` occurs with the exceptional frame **_on the stack_**!
  - The **_C++ exception object lives in the frame of the throw_**
  - `Stack pointer` is reset **_at the closing curly of the catch block_**
- **_Successively unwinding_** to landingpads **_cannot be compatible with_** `MSVC EH`
  - Mingw will never have MSVC-compatible exception handling