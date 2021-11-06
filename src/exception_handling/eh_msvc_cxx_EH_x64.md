## Visual C++ EH

* `EH` is present if:
  1. Function uses `try`/`catch` statements
  2. Automatic objects with non-trivial destructors are present 
* Implemented on top of `SEH` 
* Uses a **_distinct handler per function_**, but they all eventually call a common one
(`_CxxFrameHandler`/`_CxxFrameHandler3`)
* Compiler-generated **_unwind funclets_** are used to perform **_unwinding actions_** (calling destructors etc) during exception processing
* A special structure (`FuncInfo`) is generated for the function and contains info about **_unwinding actions_** and `try`/`catch` blocks

```c++
struct EHRegistrationNode {
    // -4 void *_esp;
    EHRegistrationNode *pNext;
    void *frameHandler;
    int state;
};

typedef const struct _s_FuncInfo {
    unsigned int magicNumber:29;
    unsigned int bbtFlags:3;
    int maxState;
    const struct _s_UnwindMapEntry * pUnwindMap;
    unsigned int nTryBlocks;
    const struct _s_TryBlockMapEntry * pTryBlockMap;
    unsigned int nIPMapEntries;
    void * pIPtoStateMap;
    const struct _s_ESTypeList * pESTypeList;
    int EHFlags;
} FuncInfo;

```
### Visual C++ EH: Unwind map

* Similar to `SEH`'s **_scope table_**, but **_without exception filters_**
* All necessary actions (_**unwind funclet`s**_) are executed unconditionally
* **_Action_** can be `NULL` to indicate no-action state transition
* Typical **_funclet_** destroys a constructed object on the stack
* Top-level state is -1

```c++
typedef const struct _s_UnwindMapEntry {
     int toState;
     void *action;
} UnwindMapEntry;
```

### Visual C++: changes for x64

* `SEH` changes completely
* Instead of **_stack-based frame registration_**, **`pointers to handlers`** and **`unwind info`** are stored in `.pdata` section
* Only limited set of instructions are supposed to be used in **_prolog_** and **_epilog_**, which makes stack walking and unwinding easier
* **_Language-specific handlers_** are used to implement compiler-level `SEH` and `C++ EH`
* However, the supporting `SEH/EH` structures (`scope table`, `FuncInfo` etc) are very similar


### x64: .pdata section
* Contains an array of `RUNTIME_FUNCTION` structures
* Each structure describes a contiguous range of instructions belonging to a function
* Chained entries (bit 0 set in `UnwindInfo`) point to the parent entry
* All addresses are RVAs (relative virtual addresses [~~relative to image base~~?])

```c++
typedef struct _RUNTIME_FUNCTION {
    DWORD BeginAddress;
    DWORD EndAddress;
    DWORD UnwindInfoAddress;
} RUNTIME_FUNCTION;
```

### x64: Unwind Info
* Starts with a header, then a number of `unwind codes`, then an optional handler and any additional data for it
* Handler is present if `Flags` have `UNW_FLAG_EHANDLER` or `UNW_FLAG_UHANDLER`

```c++
typedef struct _UNWIND_INFO {
    unsigned char Version : 3; // Version Number
    unsigned char Flags : 5; // Flags
    unsigned char SizeOfProlog;
    unsigned char CountOfCodes;
    unsigned FrameRegister : 4;
    unsigned FrameOffset : 4;
    UNWIND_CODE UnwindCode[1];
    /* UNWIND_CODE MoreUnwindCode[((CountOfCodes+1)&~1)-1];
    * union {
    * OPTIONAL ULONG ExceptionHandler;
    * OPTIONAL ULONG FunctionEntry;
    * };
    * OPTIONAL ULONG ExceptionData[];
    */
} UNWIND_INFO, *PUNWIND_INFO;
```

### x64: C++ Exception Record
* Since exceptions can be caught in a different module and the `ThrowInfo` RVAs might need to be resolved, the imagebase of the throw originator is added to the structure

```c++
typedef struct EHExceptionRecord {
    DWORD ExceptionCode; // (= EH_EXCEPTION_NUMBER)
    DWORD ExceptionFlags; Flags determined by NT
    struct _EXCEPTION_RECORD *ExceptionRecord; // extra record (not used)
    void * ExceptionAddress; // Address at which exception occurred
    DWORD NumberParameters; // Number of extended parameters. (=4)
    struct EHParameters {
        DWORD magicNumber; // = EH_MAGIC_NUMBER1
        void * pExceptionObject; // Pointer to the actual object thrown
        ThrowInfo *pThrowInfo; // Description of thrown object
        void *pThrowImageBase; // Image base of thrown object
    } params;
} EHExceptionRecord;

```
