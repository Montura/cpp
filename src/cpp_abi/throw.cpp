#include "throw.h"

extern "C" {
  void seppuku() {
    throw Exception();
  }

  // C++ exceptions под капотом:
  //  1. throw-объявление будет транслировано компилятором в два вызова: __cxa_allocate_exception и __cxa_throw
  //    (see throw.s:14-19).
  //  2. __cxa_allocate_exception и __cxa_throw "живут" в libstdc++.
  //  3. __cxa_allocate_exception выделяет память для нового исключения.
  //  4. __cxa_throw выполняет подготовку и отдает исключение в _Unwind, в набор функций,
  //    которые живут в libstdc и производит реальное разворачивание стэка (ABI определяет интерфейс этих функций).
}