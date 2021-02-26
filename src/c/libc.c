//
// The C Programming Language. Brian W. Kernighan Dennis M. Ritchie

#include <stdio.h>

#include "common.h"

void fahrenheit_celsius_table();

// 1.5 Character Input and Output
void test_input_output();

// 1.7 Functions.
// 1.8 Args -> call by value
// 1.9 Characters array
void test_functions_and_characters_array();

// 1.10 External Variables and Scope
// Declare extern variables for extern_keyword.c
char extern_longest_line[MAXLENGTH];
char extern_line[MAXLENGTH];
int extern_max_len = 0;

// 2.2 Data types
void test_data_types();

// 2.3 Constants
void test_constants_and_enums();

// 2.4 Declarations
//  1. If the variable in question is not automatic, the initialization is done once only;
//  2. An explicitly initialized automatic variable is initialized each time the function or block it is in is entered;
//  3. External and static variables are initialized to zero by default.
//  4. Automatic variables for which is no explicit initializer have undefined (i.e., garbage) values.

// 2.7 Type Conversions
void test_type_conversions();

// 2.8 Increment and Decrement Operators
void test_increment_decrement();

// 2.9 Bitwise Operators
void test_bitwise_operations();

// 2.10 Assignment operator
void tests_for_bitcount();

// 3.3 If-Else + bin_search
void tests_for_bis_search();

void test_libc() {
  printf("---------------------- Start testing libc functions ---------------------- \n");
  printf("hello, world\n");
  fahrenheit_celsius_table();
  test_input_output();
  test_functions_and_characters_array();
  test_data_types();
  test_constants_and_enums();
  test_type_conversions();
  test_increment_decrement();
  test_bitwise_operations();
  tests_for_bitcount();
  tests_for_bis_search();
  printf("---------------------- End testing libc functions ---------------------- \n");
}
