#pragma once

#include <array>
#include <func.h>

// Array Arguments
const char name[] = "J. P. Briggs"; // const char[13]
const char* prtToName = name; // because of the array-to-pointer decay rule, the code compiles.

void foo() {
  f(name);    // name is array, T - const char*
  fRef(name); // name is array, T - const char [13]
  // IMPORTANT: Type of Reference to array: const char (&) [13]
}

// the same function declaration
void myFunc(int param[]);
void myFunc(int* param);

int keyValues[] = { 1, 3, 7, 9, 11, 22, 35 };
int mappedVals [arraySize(keyValues)] ; //
