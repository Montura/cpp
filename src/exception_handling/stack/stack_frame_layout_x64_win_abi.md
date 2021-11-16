# [Stack frame layout in Windows x64](https://habr.com/ru/company/aladdinrd/blog/321868/)

## 1. Function: prologue, epilogue, frame

### `Register Parameter Area`
* Is provided by the `caller` in **_each stack frame_**.
* When a function is called, the the last thing allocated on the stack before the `Return Address` is space for at least 4 registers (8 bytes each).
* This area is available for the `callee's` use without explicitly allocating it for:
  * variable argument functions 
  * debugging
  * local variables 

### Function Prologue
#### Any of the following actions can be omitted but the order is fixed:
1. To save first 4 args to `Register Parameter Area`
2. Pushes `GP` regs (`General-Purpose registers`)
3. Allocates memory on the stack for local variables
4. Sets frame pointer (`RBP/EBP`)
5. Saves `XMM` and more `GP` regs.

```asm
mov    [rsp+8], rcx  ; 1. save first 4 args  
mov    [rsp+16], rdx
mov    [rsp+24], r8
mov    [rsp+32], r9
push   rbp           ; 2. save GP regs
push   rbx 
sub    rsp, 24       ; 3. allocate memory on the stack
; 4. Setting frame pointer is omminted
movdqa [rsp], xmm6   ; 5. saves XMM and GP regs
; `GP` regs can be saved after stack memory allocation
; instead of `push   rdi` use MOV 
mov    [rsp+16], rdi ;
```

### Function Frame
#### The frame consists of:
1. Saved `GP` regs
2. Allocated memory on the stack
3. Saved `XMM` regs

![](https://habrastorage.org/r/w1560/files/4ba/beb/a53/4babeba5391f47e69a137df097d4355e.png)

#### Function may have frame pointer (`RBP/EBP`)
1. The access to function's frame via **_frame pointer_**
2. The memory can be dynamically allocated in a function's body (not only in prologue)

```asm
; 1. Saving first 4 args is ommitted  
push rpb            ; 2. save GP regs
push rbx
sub  rsp,24         ; 3. allocate stack memory
lea  [rbp],[rsp+0]  ; 4. set frame pointer
; 5. Saving `XMM` and more `GP` regs is ommitted

; function body
call get_struct_len ; returns result to RAX  
sub  rsp, rax       ; dynamic memory allocation on the stack
mov  rcx, rsp
call read_data
```

* If `prologue` allocates stack area that exceeds 1 page size (`4KB`):
  * Such allocation can occupy more than 1 virtual page
  * So `prologue` calls `_chkstk` to check the allocation
```asm
push rpb        ; save GP regs
push rbx
push rdi
mov rax, 1000h  ; check memory allocation (is it possible?)      
call _chkstk
sub  rsp,rax    ; allocate stack memory
```

### Function Epilogue
* Function has _**at least one**_ `EPILOGUE`
#### Any of the following actions can be omitted but the order is fixed:
1. Restore `XMM` and `GP` regs that were saved after memory allocation
2. Release allocated memory by `prologue`
3. Pops `GP` regs
4. Return to the caller

```asm
mov    rdi,[rsp+16] ; 1. restore XMM and GP regs 
movdqa xmm6, [rsp], 
add    rsp, 24      ; 2. free allocated memory 
pop    rbx          ; 3. pop GP regs
pop    rbp          
ret                 ; 4. Return to the caller
```
_**NOTE:**_
* `IF` function USE _**frame pointer**_ then release memory with:
```asm
lea rsp, [RPB+constant_value]
``` 
* `ELSE` release memory with:
```asm
add rsp, constant_value
```
* `Epilogue` example:
```asm
lea rsp,[rbp+24]    ; 2. free allocated memory
pop    rbx          ; 3. pop GP regs
pop    rbp          
jmp some_funciton   ; 4. USE jmp isntead of RET
```

## 2. Function types
### Frame function
1. _**Has**_:
  * `Frame` in the stack
  * `Unwind_Info`
2. _**Can**_:
  1. call other functions (_**stack must be aligned with 16 bytes**_)
  2. allocate memory on the stack
  3. use any CPU regs

### Leaf function
1. _**Doesn't have**_:
* ~~Frame in the stack~~
* ~~Unwind Info~~
2. _**Can't**_:
  1. ~~call other functions~~
  2. ~~allocate memory on the stack~~
  3. ~~use any CPU regs~~

## 3. Calling convention

#### Just a single `x64 calling convention` no more madness with:
`cdecl`/`stdcall`/`fastcall`/`thiscall`/`register`/`safecall`

### The caller
1. Use 8 bytes per arg
2. Passes first 4 args via regs
  * Allocates `Register Parameter Area
    * Callee can use `RPA` for saving args or for any another
2. Passes 5th, 6th, ... args via stack:
  * Use `Stack Parameter Area`

![](https://habrastorage.org/r/w1560/files/c6e/198/33b/c6e19833b2d3425692009717f971ddb9.png)

#### The stack bottom always contains:
1. `Return Address`
2. `Register Parameter Area` args 1, 2, 3, 4
3. `Stack Parameter Area` args 5, 6, ...
* If function calls another func then its stack must be **_16-byte-aligned_**:
  * `RPA` always starts from this _**16-bytes alignment**_

### Passing first 4 args
* Use `RCX`, `RDX`, `R8`, `R9` for `Decimal` or `POD` type
* Use `XMM0`, `XMM1`, `XMM2`, `XMM3` for `Floating-point` type
* If `sizeof(T) > 8 bytes` pass `T arg` as pointer `T*`:
  * Then `T arg` is placed in memory allocated by the caller (`RPA`?) on the _**16-bytes alignment**_.

```c++
struct st1 { 
  int a, b; // sizeof(st1) == 8
};
struct st2 {
  int a, b, c; // sizeof(st2) == 12
};

// a -> RCX
// b -> RDX
// c -> XMM2
// d -> R9
// e -> push on the stack
void f1(int a, int b, double c, st1 d, int e);

// a    -> RCX
// b    -> XMM1
// c    -> R8
// d*   -> R9 
// e, f -> push on the stack
void f2(int a, double b, int c, st2 d, st1 e, st2 f);
```

### Return value
#### Function returns value in:
* `RAX` for `Decimal` or `POD` type
* `XMM0` for `Floating-point` type
* `Temporary Area` pointer to which is passed as first arg:
  * So, passing args are _**shifted >> 1**_:
    * `RAX` is reserved for `address of area` for `Return Value`
    * 1st -> `RDX`
    * 2nd -> `R8`
    * 3nd -> `R9`
    * 4th -> `stack`

```c++
struct st1 { 
  int a, b; // sizeof(st1) == 8
};
struct st2 {
  int a, b, c; // sizeof(st2) == 12
};

// a -> RCX, return INT -> RAX
int f1(int a);

// a -> RCX, return FLOAT -> XMM0
float f2(int a);

// a -> RCX, return st1 -> RAX
st1 f3(int a);

// RAX - ptr to area where st2 is saved
// a -> RDX
// b -> R8
// c -> R9
// d -> stack
st2 f4(int a, int b, int c, int d);
```

### Registers

#### Total 9 `Callee-saved` regs
1. `RBX`
2. `RBP`
3. `RDI`
4. `RSI`
5. `RSP`
6. `R12`
7. `R13`
8. `R14`
9. `R15`

* `Callee` has to **_saved before_** usage and **_restore after_** usage

