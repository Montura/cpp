#include "extern_template_foo.h"

// "extern template" ->
//    to force the compiler to not instantiate a template when you know that it will be instantiated somewhere else.
extern template struct Foo<int, char>;

void testExternTemplateInstantiation() {
  Foo<int, char> f;
  f.test(1, '\n');
}

struct S {
  int y;
};