#include <stdio.h>

#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal* result) {
  int errorStatus = 1;
  if (result) {
    errorStatus = 0;
    int exp = get_exponent(&value);
    int isNotZero = !s21_is_zero(value);
    errorStatus = s21_truncate(value, result);
    if ((!errorStatus) && exp && isNotZero && (get_sign(result))) {
      errorStatus = mantis_increment(result);
    }
  }
  return errorStatus;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  int errorStatus = 1;
  if (result) {
    errorStatus = 0;
    int exp = get_exponent(&value);
    uint32_t upreminder = 0;
    uint32_t criticalreminderValue = 5;
    while (exp > 1) {
      s21_div_10(&value);
      --exp;
    }
    if (exp) upreminder = s21_div_10(&value);
    set_exponent(0, &value);
    *result = value;
    if (upreminder > criticalreminderValue ||
        (upreminder == criticalreminderValue && (result->bits[0] & 1))) {
      errorStatus = mantis_increment(result);
    }
  }
  return errorStatus;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  int errorStatus = 1;
  if (result) {
    errorStatus = 0;
    int exp = get_exponent(&value);
    while (exp) {
      s21_div_10(&value);
      --exp;
    }
    *result = value;
    set_exponent(0, result);
  }
  return errorStatus;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  int errorStatus = 1;
  if (result) {
    errorStatus = 0;
    *result = value;
    set_sign((0 == get_sign(&value)), result);
  }
  return errorStatus;
}