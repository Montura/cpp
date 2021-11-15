# Zoom up the stack

## [Functions and Stack](https://www.codeproject.com/Articles/2126/How-a-C-compiler-implements-exception-handling)

### `pop EAX`

1. Read `4 bytes` from the location where `ESP` is pointing
2. `ESP += 4/8` (in 32/64-bit CPU)

### `push EBP`

1. `ESP -= 4/8` (in 32/64-bit CPU)
2. Write the contents of `EBP` register at the location where `ESP` is pointing.

### Function Prologue

```asm
push EBP      ; save current frame pointer on stack
mov EBP, ESP  ; Activate the new frame
sub ESP, 10   ; Subtract. Set ESP at the end of the frame
```

### Function Epilogue

```asm
mov ESP, EBP  
pop EBP       ; activate caller's frame
ret           ; return to the caller
```

### The `caller` use `call` instruction

1. It **_pushes_** _callee's_ arguments on the stack.
2. It **_pushes_** the address of the next instruction where control should be returned.
3. It **_jumps_** to the beginning of the _callee_.

### The `callee` use `ret` instruction

1. It **_pops_** off the return address from the stack
2. **_Transfer's_** control at that address

### Clean up function parameters (calling conventions)

1. The `caller` **_removes_** the `callee's` arguments from the stack by adding the size of the arguments to the `ESP`
   which is known at compile time:

```asm
add ESP, args_size
```

2. The `callee` **_removes_** the parameters from the stack by specifying the total parameter size in `return`
   instruction which again is known at compile time:

```asm
; Assume the total parameter size is 24 bytes
ret 24 ; remove bytes from the stack before returning to the caller 
```

Only one of these schemes is used at a time depending upon callee's calling convention. Please note that every thread in
a process has its own associated stack.    
![](https://www.codeproject.com/KB/cpp/exceptionhandler/figure3.gif)

