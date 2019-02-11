#include "Args.h"
#include "ArgsAsArrays.h"
#include "ArgsAsFuncs.h"
#include "AutoTypeDeducing.h"
#include "Decltype.h"

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