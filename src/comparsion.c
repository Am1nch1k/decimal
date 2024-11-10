#include "s21_decimal.h"

int s21_is_zero(s21_decimal a) {
  return (a.bits[0] == 0 && a.bits[1] == 0 && a.bits[2] == 0) ? 1 : 0;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  normalize(&a, &b);
  char sign_a = get_sign(&a);
  char sign_b = get_sign(&b);
  int result = (sign_a == sign_b);
  if (result) {  // если знаки равные
    result = -1;
    for (int i = 2; i >= 0 && result == -1; i--) {
      if (a.bits[i] != b.bits[i]) {
        if ((a.bits[i] > b.bits[i] && sign_a == 0) ||
            (a.bits[i] < b.bits[i] && sign_a == 1))
          result = 1;
        else
          result = 0;
      }
    }
    if (result == -1) result = 0;
  } else {
    result = (sign_a < sign_b) ? 1 : 0;
  }
  return result;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  normalize(&a, &b);
  uint8_t sign_a = (a.bits[3] >> 31) & 1;
  uint8_t sign_b = (b.bits[3] >> 31) & 1;
  int result = (sign_a == sign_b);
  if (result) {
    for (int i = 0; i < 3 && result == 1; i++) {
      if (a.bits[i] != b.bits[i]) result = 0;
    }
  } else {
    if (s21_is_zero(a) && s21_is_zero(b)) result = 1;
  }
  return result;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_greater(a, b) || s21_is_equal(a, b) ? 1 : 0;
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  return !s21_is_greater(a, b) && !s21_is_equal(a, b) ? 1 : 0;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_greater(a, b) ? 1 : 0;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_equal(a, b) ? 1 : 0;
}