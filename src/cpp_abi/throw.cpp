#include "throw.h"
#include <cstdio>

// Another exception type
struct Fake_Exception {};

void raise() {
  throw Exception();
}

// Анализируем, что произойдет, если исключение не отлавливается в catch-блоке
void try_but_dont_catch() {
  try {
    printf("Running a try which will never throw.\n");
  } catch (Fake_Exception&) {
    printf("Exception caught... with the wrong catch!\n");
  }

  try {
    raise();
  } catch (Fake_Exception&) {
    printf("Caught a Fake_Exception... with the wrong catch!\n");
  }

  printf("try_but_dont_catch handled the exception\n");
}

// И что произойдет, если отлавилвается
void catchit() {
  try {
    try_but_dont_catch();
  } catch (Exception&) {
    printf("Caught an Exception with the right catch!\n");
  } catch (Fake_Exception&) {
    printf("Caught an Fake_Exception with the right catch!\n");
  }

  printf("catchit handled the exception\n");
}

extern "C" {
  void seppuku() {
    catchit();
  }

  // C++ exceptions под капотом:
  //  1. throw-объявление будет транслировано компилятором в два вызова: __cxa_allocate_exception и __cxa_throw
  //    (see throw.s:14-19).
  //  2. __cxa_allocate_exception и __cxa_throw "живут" в libstdc++.
  //  3. __cxa_allocate_exception выделяет память для нового исключения.
  //  4. __cxa_throw выполняет подготовку и отдает исключение в _Unwind, в набор функций,
  //    которые живут в libstdc и производит реальное разворачивание стэка (ABI определяет интерфейс этих функций).
}