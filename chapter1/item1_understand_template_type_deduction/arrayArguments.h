#pragma once

#include <array>
#include <utilFunc.h>

// Array Arguments
const char name[] = "J. P. Briggs"; // const char[13]
const char* prtToName = name; // because of the array-to-pointer decay rule, the code compiles.

void foo() {
  fVal(name); // name is array, T - const char*
  fRef(name); // name is array, T - const char [13]
  // IMPORTANT: Type of Reference to array: const char (&) [13]
}

// the same function declaration
void myFunc(int param[]);
void myFunc(int* param);

// return size of an array as a compile-time constant.
// (The array parameter has no name, because we care only about the number of elements it contains.)

template<typename T, std::size_t N>
static constexpr std::size_t arraySize(T (&)[N]) noexcept {
  return N;
}

int keyValues[] = { 1, 3, 7, 9, 11, 22, 35 };
int mappedVals [arraySize(keyValues)] ; //
std::array<int, arraySize(keyValues)> mappedValues;