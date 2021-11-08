# [Compiler Internals: Exceptions and RTTI](https://www.hexblog.com/wp-content/uploads/2012/06/Recon-2012-Skochinsky-Compiler-Internals.pdf)
## Visual C++ SEH x86

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

* Keywords `__try`, `__except` and `__finally` can be used for compiler-level `SEH` support
* The compiler uses:
    1. **_A single exception handler per all functions_** with `SEH`
    2. But different supporting structures (**_scope tables_**) per function
* The `SEH` **_handler registration frame is placed on the stack_**

### Visual C++: SEH handler registration frame

* `FS:00` points to the `Next` member
* **_ExceptionHandler_** points to `__except_handler3` (`SEH3`) or `__except_handler4` (`SEH4`)
* The frame set-up is often delegated to **_compiler helper_** (`__SEH_prolog`/`__SEH_prolog4/etc`)
* **_ScopeTable_** points to a table of entries describing all `__try` blocks in the function
* **_TryLevel_** points to a current `try` block number in the **_exception registration frame_**
*  **_xpointers_** filled in by the **_exception handler_** and used by it to retrieve information about the current exception `GetExceptionInformation`/`GetExceptionCode`
```c++
// -8 DWORD _esp;
// -4 PEXCEPTION_POINTERS xpointers;
struct _EH3_EXCEPTION_REGISTRATION {
    struct _EH3_EXCEPTION_REGISTRATION *Next;
    PVOID ExceptionHandler;
    PSCOPETABLE_ENTRY ScopeTable;
    DWORD TryLevel;
};
```
### Visual C++: Scope Table
* One **_scope table_** entry is generated per `__try` block
* **_EnclosingLevel_** points to the block which contains the current one (first table entry is number 0)
* **_Top level_** (function) is -1 for `SEH3` and -2 for `SEH4`
* `SEH4` has additional fields for cookie checks

<table> 
<tr>
    <th> SEH3 </th>
    <th> SEH4 </th>
</tr>
<tr>
<td>

```c++
struct _SCOPETABLE_ENTRY {   
    DWORD EnclosingLevel;
    void* FilterFunc;
    void* HandlerFunc;
}
```

</td>
<td>

```c++
struct _EH4_SCOPETABLE {
    DWORD GSCookieOffset;
    DWORD GSCookieXOROffset;
    DWORD EHCookieOffset;
    DWORD EHCookieXOROffset;
    _EH4_SCOPETABLE_RECORD ScopeRecord[];
};
```

</td>
</tr>
</table>

### Visual C++: mapping tables to code
* **_FilterFunc_** points to the **_exception filter_** (expression in the `__except` operator)
* **_HandlerFunc_** points to the `__except` block body
* If **_FilterFunc_** is `NULL`, **_HandlerFunc_** is the `__finally` block body

```asm
; Entering __try block 0
mov [ebp+ms_exc.registration.TryLevel], 0
; Entering __try block 1
mov [ebp+ms_exc.registration.TryLevel], 1
[...]
; Entering __try block 0 again
mov [ebp+ms_exc.registration.TryLevel], 0
```