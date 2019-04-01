void test_zero();
void test_nullptr();
void nullptr_and_templates();
void nullptr_as_template_arg();

void item8() {
  test_zero();
  test_nullptr();
  nullptr_and_templates();
  nullptr_as_template_arg();
}

/// Things to Remember:
///   - Prefer nullptr to 0 and NULL.
///   - Avoid overloading on integral and pointer types.