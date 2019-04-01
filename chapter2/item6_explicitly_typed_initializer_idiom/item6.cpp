#include <unordered_map>
#include <vector>
#include <functional>
#include <iostream>
#include "Widget.h"
#include "explicitlyTypedInitializer.h"

void item6() {
  autoDeduceUndesiredTypes();
  explicitlyTypedInitializerIdiom();
}

/// Things to Remember
/// - “Invisible” proxy types can cause auto to deduce the “wrong” type for an initializing expression.
/// - The explicitly typed initializer idiom forces auto to deduce the type you want it to have.