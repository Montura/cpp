#include <iostream>

void cppref();

void printable_ASCII() {
  std::cout << "Printable ASCII [32..126]:\n";
  for (char i = ' '; i <= '~'; ++i) {
    std::cout << i << ((i % 16 == 15) ? '\n' : ' ');
  }
  std::cout << '\n';
}

#include "basic_concepts/identifierts.h"
#include "basic_concepts/decltype.h"
#include "basic_concepts/type_classification.h"
#include "basic_concepts/type_naming.h"
#include "basic_concepts/reference_declaration.h"
#include "basic_concepts/fundamental_types.h"
#include "basic_concepts/object.h"
#include "basic_concepts/sizeof.h"
#include "basic_concepts/scope.h"

void cppref() {
  printable_ASCII();

  Identifiers::test_identifiers();
  Decltype::decltype_test();
  TypeClassification::test();
  TypeNaming::test();
  ReferenceDeclaration::test();
  FundamentalTypes::test();
  Object::test();
  Sizeof::test();
  Scope::test();
}