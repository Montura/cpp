## MSVC Compiler keys

`/EHa`
1. Enables standard C++ stack unwinding.
2. Catches both **SEH (asynchronous)** and standard **C++ (synchronous)** exceptions when you use `catch(...)` syntax.
3. Overrides `/EHs` and `/EHc`.

`/EHs (doesn't catch SEH exceptions)`
1. Enables standard C++ stack unwinding.
2. Catches only standard C++ exceptions when you use `catch(...)` syntax.
3. Unless `/EHc` is also specified, the compiler assumes that functions declared as extern "C" may throw a C++ exception.

`/EHc`
1. When used with `/EHs`, the compiler assumes that functions declared as `extern "C"` never throw a C++ exception.
2. It has no effect when used with `/EHa` (that is, `/EHca` is equivalent to /EHa).
3. `/EHc` is ignored if `/EHs` or `/EHa` aren't specified.

`/EHr`
1. Tells the compiler to always generate runtime termination checks for all noexcept functions.
2. By default, runtime checks for noexcept may be optimized away if the compiler determines the function calls only non-throwing functions.
3. This option gives strict C++ conformance at the cost of some extra code. `/EHr` is ignored if `/EHs` or `/EHa` aren't specified.