## `Setjmp/Longjmp (SJLJ) Exception handling wtih GCC`
* These are somewhat analogous to VC's `x86 stack-based` SEH implementations
* `SjLj` is simpler to implement but has more runtime overhead, so it's not very common these days

* Similar to `x86 SEH`, a structure is **_constructed on the stack for each function_** that uses exception handling
* However, instead of using list in `FS:0`, implementation specific functions:
  1. At entry `_Unwind_SjLj_Register`
  2. At exit `_Unwind_SjLj_Unregister`

* The **_registration structure_** can vary but generally uses this layout:
```c++
struct SjLj_Function_Context {
    struct SjLj_Function_Context *prev;
    int call_site;
    _Unwind_Word data[4];
    _Unwind_Personality_Fn personality;
    void *lsda;
    int jbuf[];
};
```

## SjLj exceptions
* **_Personality routine_** points to the  personality function which _is called by the unwinding routine_ during exception processing
* **_LSDA_** points to _language-specific data area_ which encodes
  _unwinding actions_ and _catch handlers_ for `try` blocks
* **_call_site_** is:
  1. Analogous to the `state` variable of `VC's EH`
  2. Is updated by the compiler on every change which might need an corresponding  _unwinding action_
* `jbuf` used by the _unwinder_ to resume execution if the _personality routine_ signals that a _handler_ has been found
* Usually `jbuf[2]` contains the address of the **_landing pad_** for the function

### SjLj setup example

```asm
ADD R0, SP, #0xA4+_sjlj_ctx
LDR R3, [R3]                    ; ___gxx_personality_sj0
STR R3, [SP,#0xA4+_sjlj_ctx.personality]
LDR R3, =_lsda_sub_14F94
STR R7, [SP,#0xA4+_sjlj_ctx.jbuf]
STR R3, [SP,#0xA4+_sjlj_ctx.lsda]
LDR R3, =lp_sub_14F94
STR SP, [SP,#0xA4+_sjlj_ctx.jbuf+8]
STR R3, [SP,#0xA4+_sjlj_ctx.jbuf+4]
BL __Unwind_SjLj_Register
MOV R3, #2
STR R3, [SP,#0xA4+_sjlj_ctx.call_site]

_sjlj_ctx.personality = &__gxx_personality_sj0;
_sjlj_ctx.jbuf[0] = &v11; // frame pointer
_sjlj_ctx.lsda = &lsda_sub_14F94;
_sjlj_ctx.jbuf[2] = &v5; // stack pointer
_sjlj_ctx.jbuf[1] = lp_sub_14F94; // landing pad
_Unwind_SjLj_Register(&_sjlj_ctx);
_sjlj_ctx.call_site = 2;
```

### SjLj landing pad: unwinding
* The `SjLj` **_landing pad handler_** inspects the **_call_site_** member and depending on its value:
  1. Performs _unwinding actions_ (destruct local variables) 
  2. Executes _user code_ in the `catch` blocks.

### SjLj landing pad: catch blocks
* If the current state corresponds to a `try` block, then the **_landing pad handler_** checks the _**handler switch value**_ to determine which exception was matched


### Example
```c++
  #include <csetjmp>

  jmp_buf env_;
  do {
    if (!setjmp(env_) { // TRY
      printf("In TRY statement\n");
      longjmp(env_, 1); // THROW
      printf("I do not appear\n");
    } else { // CATCH
      printf("Got an exception\n");
    }
  } while (0); // ETRY
  
  ETRY
```
1. When `setjmp(env_)` is called first time it:
  1. saves the Execution Context (**EC**) in a `jmp_buf` structure
  2. returns 0
2. When `longjmp(env_, 1)` is called (_non-local goto_):
  1. **EC** saved in `jmp_buf` is copied back to the CPU and computation starts over from the point of `setjmp(env_)` function.
  2. The returned value of `setjmp(env_)` is the one passed as **SECOND** argument to `longjmp(env_, 1)` function.

##### Performance drawbacks:
```c++
struct Resource {
  Resource() {};
  ~Resource() {};
};
```
1. `setjmp(env_)` must be called at the beginning of the every `"__try"-block` and the `List<jmp_buf*>` must be maintained.
2. The `List<Resource*>` on the stack must be maintained at all times and kept a consistent state with respect to the list of `List<jmp_buf*>`.
3. All the variables are stored in the registers and that are declared outside the `"__try"-block` have to be restored to their initial values when `longjmp(env_, 1)` is invoked.

## GCC exceptions: zero-cost
* `Zero-cost` refers to no code overhead in the case of no exceptions (unlike `SjLj` which has set-up/tear-down code that is always executed)
* Uses a set of tables encoding address ranges, so does not need any state variables in the code
* Format and encoding is based on `Dwarf2/Dwarf3/Dwarf4`
* The first-level (language-independent) format is described in [Linux Standard Base Core Specification](http://refspecs.linuxbase.org/LSB_4.1.0/LSB-Core-generic/LSB-Core-generic/ehframechpt.html
  )
* Second-level (language-specific) is based on HP [Itanium implementation](https://itanium-cxx-abi.github.io/cxx-abi/exceptions.pdf) but differs from it in some details