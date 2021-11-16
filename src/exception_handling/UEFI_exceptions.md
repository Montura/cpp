#[Exceptions in UEFI app](https://habr.com/ru/company/aladdinrd/blog/320948/)

- Intel x64 CPU
- MSVC compiler (`try/except` blocks)
- `IDT` table (Interrupt Descriptor Table)

![](https://habrastorage.org/files/04e/7d9/5ed/04e7d95ed9014dd687c03f2faae9c5df.png)

  - Total _**256**_ exceptions 
  - First **_32_** are reserved for **_exceptions_**
  - Other **_224_** are used for **_interruptions_**

Exception:
1. CPU can't execute the current instruction
2. Exception is happened
3. CPU calls `Interruption Handler` from `IDT`
   * `IH` entry point is in `IDT`

When the compiler encounters `try\except` block:
1. It generates **_meta information_** that describes for each block:
   1. Block's place in a `PE` image
   2. Address of `handler` for this block:
      * `except {...}` or `finally {...}` are handlers
2. **_Meta information_** is inside `.pdata` section of `PE` image

![](https://habrastorage.org/files/8eb/3b6/f2c/8eb3b6f2c9bf456c8d0a86f2782b48a3.png)

### `try/except` blocks
* Are processed by the compiler
* Are part of programming language
* _**Require OS support**_:
  * One part of exception processing is held by _**OS**_
  * Another part of exception processing is held by **_APP_**
* Require `__c_specific_handler` implementation:
  * Or will get a link error about omitted implementation
  * Handler are part of Windows `CRT` libraries:
    * `_CxxFrameHandler` \ `_CxxFrameHandler3`
    * `SEH3: __except_handler3` \ `SEH4: __except_handler4`

### Exception processing
![](https://habrastorage.org/r/w1560/files/222/e3d/d3b/222e3dd3b7224f9f9029044461509937.png)
1. Exception is happened
2. OS calls `Interruption Handler` from `IDT`:
   * `IHers` entry point are in `IDT`
   * `IHers` are set by `OS` during its launch
3. `IHers` save:
   * All necessary info needed for exception processing
   * All specific info for current exception type
4. `IHers` calls function to scan and find proper handler.
5. It scans `.pdata` section to find `except/finally {...}` handlers
6. If handler was found then OS pass control flow to it, else app is terminated with an error.
