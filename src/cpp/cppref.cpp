#include <iostream>

void cppref();

void printable_ASCII() {
  std::cout << "Printable ASCII [32..126]:\n";
  for (char i = ' '; i <= '~'; ++i) {
    std::cout << i << ((i % 16 == 15) ? '\n' : ' ');
  }
  std::cout << '\n';
}

#include "identifierts.h"
#include "decltype.h"
#include "type_classification.h"
#include "type_naming.h"
#include "reference_declaration.h"

void cppref() {
  printable_ASCII();

  Identifiers::test_identifiers();
  Decltype::decltype_test();
  TypeClassification::test();
  TypeNaming::test();
  ReferenceDeclaration::test();
}