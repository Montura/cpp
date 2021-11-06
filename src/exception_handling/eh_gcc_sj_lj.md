## `Setjmp/Longjmp (SJLJ) Exception handling`
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

