#include "Args.h"
#include "ArgsAsArrays.h"
#include "ArgsAsFuncs.h"
#include "AutoTypeDeducing.h"

int main() {
  deduceTemplatOne();
  deduceTemplateTwo();
  deduceTemplateThree();
  checkTypes();
  conceptualCalls();
  autoDeducing();
  initializerListDeducing();
  return 0;

}