#include <chapter1/item1_understand_template_type_deduction/arrayArguments.h>
#include <chapter1/item1_understand_template_type_deduction/functionArguments.h>
#include <chapter1/item1_understand_template_type_deduction/case1.h>
#include <chapter1/item1_understand_template_type_deduction/case2.h>
#include <chapter1/item1_understand_template_type_deduction/case3.h>
#include <chapter1/item1_understand_template_type_deduction/functionArguments.h>
#include <chapter1/item2_undrestand_auto_type_deducing/autoTypeDeducing.h>
#include <chapter1/item3_understand_decltype/decltype.h>
#include <chapter2/item5_prefer_auto_to_explicit_type_declarations/autoVsExpilicitTypes.h>
#include <chapter2/item6_explicitly_typed_initializer_idiom/explicitlyTypedInitializer.h>

int main() {
  deduceTemplatOne();
  deduceTemplateTwo();
  deduceTemplateThree();
  checkTypes();
  conceptualCalls();
  autoDeducing();
  autoSpecialDeducingRule();
  decltypeForRValueParams();
  return 0;
}