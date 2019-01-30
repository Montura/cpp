#pragma once

#include <iostream>

//template<typename T>
//void f(ParamType param);

// f(expr) // deduce T and ParamType from expr

template<typename T>
void fRef(T & param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

template<typename T>
void fCRef(T const& param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

template<typename T>
void fPointer(T * param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

template <typename T>
void fUniRef(T && param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

template <typename T>
void f(T param) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
};

template<typename T, std::size_t N>
static constexpr std::size_t arraySize(T (&)[N]) noexcept {
  return N;
}