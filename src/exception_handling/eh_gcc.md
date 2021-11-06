# GCC: exceptions
* Two kinds of implementing exceptions are commonly used by GCC:
    1. `SjLj` (setjump-longjump)
    2. `zero-cost` (table-based)
* These are somewhat analogous to VC's `x86 stack-based` and `x64 table-based` SEH implementations
* `SjLj` is simpler to implement but has more runtime overhead, so it's not very common these days


* Similar to `x86 SEH`, a structure is constructed on the stack for each function that uses exception handling
* However, instead of using list in `FS:0`, implementation specific functions are called at entry and exit (`_Unwind_SjLj_Register`/`_Unwind_SjLj_Unregister`)

## GCC: SjLj exceptions
* `personality` points to the so-called "personality function" which is called by the unwinding routine during exception processing
* `lsda` points to "language-specific data area" which contains info in the format specific to the personality routine
* `call_site` is analogous to the `stat`e variable of `VC's EH` and is updated by the compiler on every change which might need an corresponding unwinding action
* `jbuf` contains architecture-specific info used by the unwinder to resume execution if the personality routine signals that a handler has been found
* However, usually `jbuf[2]` contains the address of the **_landing pad_** for the function


### SjLj landing pad: unwinding
* The SjLj landing pad handler inspects the `call_site` member and depending on its value performs unwinding actions (destruct local variables) or executes user code in the catch blocks.

### SjLj landing pad: catch blocks
* If the current state corresponds to a try block, then the landing pad handler checks the handler switch value to
  determine which exception was matched


## GCC exceptions: zero-cost
* `Zero-cost` refers to no code overhead in the case of no exceptions (unlike `SjLj` which has set-up/tear-down code that is always executed)
* Uses a set of tables encoding address ranges, so does not need any state variables in the code
* Format and encoding is based on `Dwarf2/Dwarf3/Dwarf4`
* The first-level (language-independent) format is described in [Linux Standard Base Core Specification](http://refspecs.linuxbase.org/LSB_4.1.0/LSB-Core-generic/LSB-Core-generic/ehframechpt.html
  )
* Second-level (language-specific) is based on HP [Itanium implementation](https://itanium-cxx-abi.github.io/cxx-abi/exceptions.pdf) but differs from it in some details
