# [Stack frame layout on x86-64](https://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64)

* System V AMD64 ABI

## 1. Function: prologue, epilogue, frame

### Function Prologue
```asm
push rbp      ; save current frame pointer on stack
mov  rbp, rsp ; activate the new frame
sub  rsp, 10  ; allocate memory 
```

### Function frame
#### The base pointer `EBP/RBP`:
1. Is an **_"anchor"_** to the beginning of the stack frame throughout the execution of a function
2. Is very convenient for manual assembly coding and for debugging.

3. However, **_compiler-generated code doesn't really need it_**:
* The compiler can easily keep track of offsets from `RSP`
* The DWARF  format provides means (`CFI`) to access stack frames without the base pointer

#### Some compiler omitting the base pointer for aggressive optimizations

1. To short the function `prologue` and `epilogue`
2. To provide an additional register `RBP` for `GP` regs usage.

`GCC`:
1. Keeps the base pointer by default on x86
2. Optimization managed by compilation flags:
* `-fomit-frame-pointer`
* `-fno-omit-frame-pointer`

### Function Epilogue
```asm
mov rsp, rbp  
pop ebp       ; activate caller's frame
ret           ; return to the caller
```


## 2. Calling convention

### Passing first 6 args
* Use `RDI`, `RSI`, `RCX`, `RDX`, `R8`, `R9`

#### Frame Function
```c++
long myfunc(long a, long b, long c, long d,
            long e, long f, long g, long h) {
  long xx = a * b * c * d * e * f * g * h;
  long yy = a + b + c + d + e + f + g + h;
  long zz = utilfunc(xx, yy, xx % yy);
  return zz + 20;
}
```
![](https://eli.thegreenplace.net/images/2011/08/x64_frame_nonleaf.png)
####NOTE:
* So the first 6 arguments are passed via registers.
* It looks the same as for `x86`, except this strange `red zone`

### Red Zone
* It's usually most useful in `leaf functions`.
* The `red zone` is an optimization:
  * **_decrementing_** and **_restoring_** `RSP` can be omitted.
* Code can assume that the `128 bytes` below `RSP` will not be asynchronously clobbered by signals or interrupt handlers.
* So, it can use it for scratch data, without explicitly moving the stack pointer.

#### This is indeed a leaf function's stack frame when compiled with `GCC`
```c++
long utilfunc(long a, long b, long c) {
  long xx = a + 2;
  long yy = b + 3;
  long zz = c + 4;
  long sum = xx + yy + zz;

  return xx * yy * zz + sum;
}
```

![](https://eli.thegreenplace.net/images/2011/08/x64_frame_leaf.png)

#### NOTE:
* Since `utilfunc` only has 3 arguments, calling it requires no stack usage since all the arguments fit into registers.
* In addition, since it's a `leaf function`, gcc chooses to use the `red zone` for all its local variables.
* Thus, `RSP` needs not be decremented (and later restored) to allocate space for this data.

### Registers

#### Total 7 `Callee-saved` regs
1. `RBX`
2. `RBP`
3. `RSP`
4. `R12`
5. `R13`
6. `R14`
7. `R15`

* `Callee` has to **_saved before_** usage and **_restore after_** usage
  