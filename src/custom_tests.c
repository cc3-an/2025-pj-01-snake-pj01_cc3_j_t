#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"

// Necesario por las funciones static en state.c
#include "state.c"

/* Vean el archivo asserts.c, para conocer algunas funciones de assert */

int greater_than_forty_two(int x) {
  return x > 42;
}

bool is_vowel(char c) {
  char* vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++) {
    if (c == vowels[i]) {
      return true;
    }
  }
  return false;
}

/**
 * Ejemplo 1: Retorna true si todos los tests pasan. false de lo contrario.
 *
 * La funcion greater_than_forty_two(int x) va a retornar true si x > 42. false de lo contrario.
 * Nota: Este test no cubre todo al 100%.
 */
bool test_greater_than_forty_two() {
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1)) {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2)) {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is a,e,i,o,u)
    and returns false otherwise
    Note: This test is NOT comprehensive
*/
/**
 * Ejemplo 2: Retorna true si todos los tests pasan. false de lo contrario.
 *
 * La funcion is_vowel(char c) va a retornar true si c es una vocal (es decir, c es a,e,i,o,u)
 * y retorna false en el caso contrario
 * Nota: Este test no cubre todo al 100%.
 */
bool test_is_vowel() {
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1)) {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2)) {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4)) {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5)) {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6)) {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail()
{
  // TODO: Implementar esta funcion
  if (assert_true("is_tail('w')", is_tail('w'))) return true;
  if (assert_true("is_tail('a')", is_tail('a'))) return true;
  if (assert_true("is_tail('s')", is_tail('s'))) return true;
  if (assert_true("is_tail('d')", is_tail('d'))) return true;
  if (assert_true("is_tail('x')", is_tail('x'))) return true;
  if (assert_true("is_tail('A')", is_tail('A'))) return true;

  return false;
}

bool test_is_head()
{
  // TODO: Implementar esta funcion
  if (assert_equals_char("body_to_tail('^')", 'w', body_to_tail('^'))) return true;
  if (assert_equals_char("body_to_tail('<')", 'a', body_to_tail('<'))) return true;
  if (assert_equals_char("body_to_tail('v')", 's', body_to_tail('v'))) return true;
  if (assert_equals_char("body_to_tail('>')", 'd', body_to_tail('>'))) return true;
  if (assert_equals_char("body_to_tail('?')", '?', body_to_tail('?'))) return true;

  return false;
}

bool test_is_snake()
{
  // TODO: Implementar esta funcion
  char* snake_chars = "wasd^<v>WASDx";
  for (int i = 0; i < strlen(snake_chars); i++)
  {
    if (assert_true("is_snake()", is_snake(snake_chars[i]))) return true;
  }

  if (assert_false("is_snake('#')", is_snake('#'))) return true;
  if (assert_false("is_snake('*')", is_snake('*'))) return true;

  return false;
}

bool test_body_to_tail()
{
  // TODO: Implementar esta funcion
  if (assert_equals_char("body_to_tail('^')", 'w', body_to_tail('^'))) return true;
  if (assert_equals_char("body_to_tail('<')", 'a', body_to_tail('<'))) return true;
  if (assert_equals_char("body_to_tail('v')", 's', body_to_tail('v'))) return true;
  if (assert_equals_char("body_to_tail('>')", 'd', body_to_tail('>'))) return true;
  if (assert_equals_char("body_to_tail('?')", '?', body_to_tail('?'))) return true;

  return false;
}

bool test_head_to_body()
{
  // TODO: Implement this function.
  if (assert_equals_char("head_to_body('W')", '^', head_to_body('W'))) return true;
  if (assert_equals_char("head_to_body('A')", '<', head_to_body('A'))) return true;
  if (assert_equals_char("head_to_body('S')", 'v', head_to_body('S'))) return true;
  if (assert_equals_char("head_to_body('D')", '>', head_to_body('D'))) return true;
  if (assert_equals_char("head_to_body('?')", '?', head_to_body('?'))) return true;

  return false;
}

bool test_get_next_row()
{
  // TODO: Implement this function.
  if (assert_equals_unsigned_int("get_next_row('v')", 6, get_next_row(5, 'v'))) return true;
  if (assert_equals_unsigned_int("get_next_row('s')", 6, get_next_row(5, 's'))) return true;
  if (assert_equals_unsigned_int("get_next_row('S')", 6, get_next_row(5, 'S'))) return true;

  if (assert_equals_unsigned_int("get_next_row('^')", 4, get_next_row(5, '^'))) return true;
  if (assert_equals_unsigned_int("get_next_row('w')", 4, get_next_row(5, 'w'))) return true;
  if (assert_equals_unsigned_int("get_next_row('W')", 4, get_next_row(5, 'W'))) return true;

  if (assert_equals_unsigned_int("get_next_row('>')", 5, get_next_row(5, '>'))) return true;

  return false;
}

bool test_get_next_col()
{
  // TODO: Implementar esta funcion
  if (assert_equals_unsigned_int("get_next_col('>')", 6, get_next_col(5, '>'))) return true;
  if (assert_equals_unsigned_int("get_next_col('d')", 6, get_next_col(5, 'd'))) return true;
  if (assert_equals_unsigned_int("get_next_col('D')", 6, get_next_col(5, 'D'))) return true;

  if (assert_equals_unsigned_int("get_next_col('<')", 4, get_next_col(5, '<'))) return true;
  if (assert_equals_unsigned_int("get_next_col('a')", 4, get_next_col(5, 'a'))) return true;
  if (assert_equals_unsigned_int("get_next_col('A')", 4, get_next_col(5, 'A'))) return true;

  if (assert_equals_unsigned_int("get_next_col('^')", 5, get_next_col(5, '^'))) return true;

  return false;
}

bool test_customs() {
  if (!test_greater_than_forty_two()) {
    printf("%s\n", "test_greater_than_forty_two failed.");
    return false;
  }
  if (!test_is_vowel()) {
    printf("%s\n", "test_is_vowel failed.");
    return false;
  }
  if (!test_is_tail()) {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_head()) {
    printf("%s\n", "test_is_head failed");
    return false;
  }
  if (!test_is_snake()) {
    printf("%s\n", "test_is_snake failed");
    return false;
  }
  if (!test_body_to_tail()) {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_head_to_body()) {
    printf("%s\n", "test_head_to_body failed");
    return false;
  }
  if (!test_get_next_row()) {
    printf("%s\n", "test_get_next_row failed");
    return false;
  }
  if (!test_get_next_col()) {
    printf("%s\n", "test_get_next_col failed");
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}
