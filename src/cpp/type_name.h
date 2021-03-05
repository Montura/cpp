#pragma once

#include <type_traits>
#include <string>

template <class T>
std::string type_name() {
  typedef typename std::remove_reference_t<T> DereferencedType;

  std::string r = typeid(DereferencedType).name();
  if (std::is_const_v<DereferencedType>) {
    r += " const";
  }
  if (std::is_volatile_v<DereferencedType>) {
    r += " volatile";
  }
  if (std::is_lvalue_reference_v<T>) {
    r += "&";
  } else if (std::is_rvalue_reference_v<T>) {
    r += "&&";
  } else if (std::is_pointer_v<T>) {
    r += "*";
  }
  return r;
}