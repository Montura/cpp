# [Exceptions in Windows x64. Part 2](https://habr.com/ru/company/aladdinrd/blog/322956/)

[`Part 1. Stack frame layout`](../stack/stack_frame_layout_x64_win_abi.md)

## 1. `.pdata` section of PE image ([Win32 PE Format](https://docs.microsoft.com/en-us/windows/win32/debug/pe-format))
![](https://habrastorage.org/files/8eb/3b6/f2c/8eb3b6f2c9bf456c8d0a86f2782b48a3.png)

It contains:
1. `Function table` for PE image
2. Info about stack frame unwinding
3. OS uses `Function Table` during `EHer` lookup. 

## 2. `Function Table` 
* Contains an **_array of_** `RUNTIME_FUNCTION` **_structures_**
* Elements are sorted in **_ascending order_** by the `BeginAddress`.
* All addresses are **_RVAs_** (relative virtual addresses to `PE`)
* **_Chained entries_** (bit 0 set in `UnwindInfo`) point to the parent entry (see later)
* Can describe a `chunck` of function (see later)
```c++
typedef struct _RUNTIME_FUNCTION {
  DWORD BeginAddress;      // address of the first byte of function's begin 
  DWORD EndAddress;        // address of the first byte after function's end
  DWORD UnwindInfoAddress; // address of `UNWIND_INFO` structure
} RUNTIME_FUNCTION, *PRUNTIME_FUNCTION;
```

## 2. `UNWIND_INFO`
* Describes a non-leaf function in terms of:
  * its prologue size
  * frame register usage
```c++
typedef struct _UNWIND_INFO {
  unsigned char Version : 3;  // version number
  unsigned char Flags   : 5;  // Flags
  
  unsigned char SizeOfProlog; // size of prolog in bytes
  unsigned char CountOfCodes; 
  
  unsigned FrameRegister : 4; // register number used for stack frame pointer
  unsigned FrameOffset   : 4; 
  
  UNWIND_CODE UnwindCode[1];   
} UNWIND_INFO, *PUNWIND_INFO;
```

#### Frame offset
* `FrameOffset` is a number of 16-bytes blocks
* When pointer to function stack frame is set to `RSP` + `FrameOffset` * `16`
* if `FrameRegister` is 0, then pointer to function stack frame is `RSP`

#### _Array of `UNWIND_CODE`_
* Describes the way in which the stack is set up when the prologue is executed.
* This is provided through an array of the UnwindCode array.
* `CountOfCodes` is the length of the array.
* The array is composed of `UNWIND_CODE` structures which are defined as[8]:
* Elements are sorted in **_descending order_**:
  * First element - is a **_last_** `prologue` _**action**_

#### Flags
* `UNW_FLAG_EHANDLER`
  * function has `Exception Handler` and it must be executed after `handler` lookup
* `UNW_FLAG_UHANDLER`
  * function has `Unwinding(Termination) Handler` and it must be executed during unwinding
* `UNW_FLAG_CHAININFO`
  * current `UNWIND_INFO` is `chained` with the previous `UNWIND_INFO` structure

### Structure layout
#### When `UNWIND_INFO::Flag` is `UNW_FLAG_EHANDLER` or `UNW_FLAG_UHANDLER`
* _**Next structure**_ is:
```c++
#include <winnt.h> // see definitions here
typedef struct _EXCEPTION_HANDLER {
  DWORD ExceptionHandlerAddress; // address of `ExceptionRoutine`
  BYTE LanguageSpecificData[0]; 
} EXCEPTION_HANDLER, *PEXCEPTION_HANDLER ;

typedef enum EXCEPTION_DISPOSITION;

EXCEPTION_DISPOSITION ExceptionRoutine(
    PEXCEPTION_RECORD   ExceptionRecord,  // exception reason info
    PVOID               EstablisherFrame, // pointer to stack frame
    PCONTEXT            ContextRecord,    // CPU context
    PDISPATCHER_CONTEXT DispatcherContext // current context 
);
```
* `ContextRecord` points to structure `CONTEXT`:
  * Initially, `CONTEXT` is a `CPU context` when the exception has occurred
  * `CONTEXT` will be changed by handlers during:
    * **_handler lookup_**
    * _**stack unwinding**_
  * The final `CONTEXT` state is a target `CPU context` 
  * Target `CPU context` will be applied when app continues execution
* `DispatcherContext`
  * contains current `CONTEXT` for:
    * **_handler lookup_**
    * _**stack unwinding**_

![](https://habrastorage.org/r/w1560/files/82a/f62/1fa/82af621fa71a412e92486c78720c29c8.png)


#### When `UNWIND_INFO::Flag` is `UNW_FLAG_CHAININFO`: 
  * This `UNWIND_INFO` is _**chained**_
  * **_Next structure_** is `RUNTIME_FUNCTION`
  * `RUNTIME_FUNCTION::UnwindInfoAddress` points to the previous `UNWIND_INFO`
  * Look again at the `UNWIND_INFO::Flag`
  * The number of **_chained_** structures is up to 32.
  
### Chained structures optimizations
1. To avoid saving some `GP` regs in `prologue` and do it in function body:
   * Replace `push` to the stack with `mov` in memory
   * We have to provide additional **_chunked_** `RUNTIME_FUNCTION`:
     * It points to the proper **_chained_** `UNWIND_INFO` that saves `GP` regs 
2. **_Chunked_** `RUNTIME_FUNCTION` and **_chained_** `UNWIND_INFO`
   * allow avoiding duplicates in `UNWIND_INFO::UnwindCodes`

![](https://habrastorage.org/r/w1560/files/e2d/050/372/e2d05037284a46d1914e8c835df0d19e.png)


## 3. `Unwind codes`

Look at the structure `UNWIND_CODE` that are used in an array inside the `UNWIND_INFO` to describe `prologue` actions 

```c++
typedef union _UNWIND_CODE {
  struct { // prologue
    BYTE CodeOffset;
    BYTE OpCode : 4;
    BYTE OpInfo : 4;
  };
  struct { // epilogue
    BYTE CodeOffset;
    BYTE OpCode : 4;
    BYTE OpInfo : 4;
  };
} UNWIND_CODE;
```

* First struct is used for `prologue`:
  * `CodeOffset` has an offset to the next instruction. 
  * Поле `OpCode` содержит код выполняемого действия.
  * Поле `OpInfo` зависит от значения поля OpCode. 
  
* Second struct is used for `epilogue`:
  * `OffsetLowOrSize` is 12-bits offset `RUNTIME_FUNCTION::BeginAddress`
  * `OffsetHighOrFlags` is 12-bits offset `RUNTIME_FUNCTION::EndAddress`

# [Exceptions in Windows x64. Part 3](https://habr.com/ru/company/aladdinrd/blog/324376/)

## 1. Exceptions and exception handling

### 1.1 Helper functions
#### Windows OS:
* `ntoskrnl.exe` -> exports to `KernelSpace`:
* `ntdll.dll` -> exports to `UserSpace`:
  * `RtlLookupFunctionEntry`
  * `RtlVirtualUnwind`

### RtlLookupFunctionEntry
```c++
PRUNTIME_FUNCTION RtlLookupFunctionEntry(
  DWORD64               ControlPc;    // address of code
  PDWORD64              ImageBase;    // output address of PE image
  PUWIND_HISTORY_TABLE  HistoryTable; // used for caching lookup
);
```
* `RtlLookupFunctionEntry` return values:
  * `PRUNTIME_FUNCTION`: valid structure for `Frame function`
  * `NULL`:
    * Can't find `PE` image by provided `ControlPc`
    * Or it is a `leaf function` - it has no frame

### RtlVirtualUnwind
[Example](https://github.com/microsoft/Xbox-ATG-Samples/blob/9654b6efa4e1db52365fc2ae80386e0e27b9658d/XDKSamples/Tools/SymbolProxyClient/CallStack.cpp#L45)

* Do **_virtual unwinding_** for the `PRUNTIME_FUNCTION`:
  1. Don't change physical state of `CPU context`
  2. Changes only temporary structure `PCONTEXT ContextRecord`
  3. `CPU context` after unwinding saves to `PCONTEXT ContextRecord`
```c++
PEXCEPTION_ROUTINE RtlVirtualUnwind(
  DWORD                          HandlerType;
  DWORD64                        ImageBase; // PE image address
  DWORD64                        ControlPc; // 
  PRUNTIME_FUNCTION              FunctionEntry;
  PCONTEXT                       ContextRecord,
  PVOID                          *HandlerData,
  PDWORD64                       EstablisherFrame,
  PKNONVOLATILE_CONTEXT_POINTERS ContextPointers
);
```


####_**Args**_:
* `HandlerType` when -> `UNW_FLAG_EHANDLER` or `UNW_FLAG_UHANDLER`
* `ImageBase` -> address of executable `PE` image
* `ControlPc` -> instruction address where the execution is interrupted
* `FunctionEntry` -> address of `RUNTIME_FUNCTION`
* `ContextRecord` -> **_output_** parameter for `CPU context`
* `HandlerData` -> **_output_** parameter for `handler data`
* `EstablisherFrame` -> **_output_** parameter for `function frame pointer`
* `ContextPointers` -> optional **_output_** for `GP` and `XMM` regs after unwinding


####**_Return Values_** depends on `HandlerType`:
  * `UNW_FLAG_EHANDLER`:
    * Returns address to `EHer` as `PEXCEPTION_ROUTINE`
    * Writes address of `EHer` data to `HandlerData` (`NULL` if no `EHer`)
  * `UNW_FLAG_UHANDLER`:
    * Returns address to `UWHer` as `PEXCEPTION_ROUTINE`
    * Writes address of `UWHer` data to `HandlerData` (`NULL` if no `EHer`)
  * Else returns `NULL`

![](https://habrastorage.org/r/w1560/files/393/e3f/3e6/393e3f3e6f034873828f913e6b9f3f44.png)

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
