# [Compiler Internals: Exceptions and RTTI](https://www.hexblog.com/wp-content/uploads/2012/06/Recon-2012-Skochinsky-Compiler-Internals.pdf)
## Visual C++ EH x86

* `EH` is present if:
  1. Function uses `try`/`catch` statements
  2. Automatic objects with non-trivial destructors are present 
* Implemented on top of `SEH` 
* Uses a **_distinct handler per function_**, but they all call a common one
`_CxxFrameHandler`/`_CxxFrameHandler3`
* Compiler-generated **_unwind funclets_** are used to perform **_unwinding actions_** (calling destructors etc) during exception processing
* A special structure (`FuncInfo`) is generated for the function and contains info about **_unwinding actions_** and `try`/`catch` blocks

### x86: Registration and FuncInfo structure
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
    int maxState;                                    // Number of entries
    const struct _s_UnwindMapEntry * pUnwindMap;     // A pointer to unwind map
    unsigned int nTryBlocks;                         // Number of entries
    const struct _s_TryBlockMapEntry * pTryBlockMap; // A pointer to try{} block map
    unsigned int nIPMapEntries;                      // Number of entries
    void * pIPtoStateMap;                            // A ptr to IP-to-state map (unused on x86)
    const struct _s_ESTypeList * pESTypeList;        // List of exceptions in the throw specification (undocumented feature)
    int EHFlags;                                     // FI_EHS_FLAG=1: function was compiled /EHs
} FuncInfo;
```
### x86: Unwind map

* Similar to `SEH`'s **_scope table_**, but **_without exception filters_**
* All necessary actions (**_unwind funclet's_**) are executed unconditionally
* **_Action_** can be `NULL` to indicate no-action state transition
* Typical **_funclet_** destroys a constructed object on the stack
* **_Top-level_** state is -1

```c++
typedef const struct _s_UnwindMapEntry {
     int toState;
     void *action;
} UnwindMapEntry;
```

### Changes for x64

* `SEH` changes completely
* Instead of **_stack-based frame registration_**, next items are stored in `.pdata` section:
   1. **_Pointers to handlers_**
   2. **_Unwind Info_**
* Only limited set of instructions are supposed to be used in **_prolog_** and **_epilog_**:
  - It makes _stack walking_ and _unwinding_ **easier**
* **_Language-specific handlers_** are used to implement compiler-level `SEH` and `C++ EH`
* However, the supporting `SEH/EH` structures (**_scope table_**, `FuncInfo` ) are very similar

### x64: `.pdata` section
* Contains an **_array of_** `RUNTIME_FUNCTION` **_structures_**
* Each structure describes **_a contiguous range of instructions_** belonging to a function
* **_Chained entries_** (bit 0 set in `UnwindInfo`) point to the parent entry
* All addresses are **_RVAs_** (relative virtual addresses [~~relative to image base~~?])

```c++
typedef struct _RUNTIME_FUNCTION {
    DWORD BeginAddress;
    DWORD EndAddress;
    DWORD UnwindInfoAddress;
} RUNTIME_FUNCTION;
```

### x64: Unwind Info

```c++
typedef struct _UNWIND_INFO {
    unsigned char Version : 3;  // Version Number
    unsigned char Flags : 5;    // Flags
    unsigned char SizeOfProlog;
    unsigned char CountOfCodes; // A number of "unwind codes
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

### x64: VC C++ EH FuncInfo
* Pretty much the same as `x86`, except RVAs instead of addresses and `IP-to-state` map is used

```c++
typedef struct _s_FuncInfo {
   int magicNumber;            // 0x19930522
   int maxState;               // number of states in unwind map
   int dispUnwindMap;          // RVA of the unwind map
   unsigned int nTryBlocks;    // count of try blocks
   int dispTryBlockMap;        // RVA of the try block array
   unsigned int nIPMapEntries; // count of IP-to-state entries
   int dispIPtoStateMap;       // RVA of the IP-to-state array
   int dispUwindHelp;          // rsp offset of the state var (initialized to -2; used during unwinding)
   int dispESTypeList;         // list of exception spec types
   int EHFlags;                // flags
} FuncInfo;

```

### x64: IP-to-state map
* Instead of an explicit state variable on the stack (as in `x86`), this  map is used to find out the current state from the execution address

```c++
typedef struct IptoStateMapEntry {
   __int32 Ip; // Image relative offset of IP
   __ehstate_t State;
} IptoStateMapEntry;
```

### x64: C++ Exception Record
* Since exceptions can be caught in a different module and the `ThrowInfo` RVAs might need to be resolved, the imagebase of the throw originator is added to the structure

```c++
typedef struct EHExceptionRecord {
    DWORD ExceptionCode;                       // (= EH_EXCEPTION_NUMBER)
    DWORD ExceptionFlags;                      // Flags determined by NT
    struct _EXCEPTION_RECORD *ExceptionRecord; // extra record (not used)
    void * ExceptionAddress;                   // Address at which exception occurred
    DWORD NumberParameters;                    // Number of extended parameters. (=4)
    struct EHParameters {
        DWORD magicNumber;       // = EH_MAGIC_NUMBER1
        void * pExceptionObject; // Pointer to the actual object thrown
        ThrowInfo *pThrowInfo;   // Description of thrown object
        void *pThrowImageBase;   // Image base of thrown object
    } params;
} EHExceptionRecord;

```
