#include "throw.h"
#include <cstdio>

// Добавляем второй тип исключений
struct Fake_Exception {};

void raise() {
  throw Exception();
}

// Анализируем, что произойдет, если исключение не отлавливается в catch-блоке
void try_but_dont_catch() {
  try {
    raise();
  } catch (Fake_Exception&) {
    printf("Running try_but_dont_catch::catch(Fake_Exception)\n");
  }

  printf("try_but_dont_catch handled an exception and resumed execution\n");
}

// И что произойдет, если отлавилвается
void catchit() {
  try {
    try_but_dont_catch();
  } catch (Exception&) {
    printf("Running try_but_dont_catch::catch(Exception)\n");
  } catch (Fake_Exception&) {
    printf("Running try_but_dont_catch::catch(Fake_Exception)\n");
  }

  printf("catchit handled an exception and resumed execution\n");
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