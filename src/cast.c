//
// Created by Andrey on 03.09.2024.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  set_zero(dst);
  char sign = 0;
  if (src < 0) sign = 1;
  long long int source = llabs(src);
  set_sign(sign, dst);
  for (int i = 0; i <= D_LAST_BIT_POS && source > 0; ++i, source >>= 1)
    set_bit(dst, i, source % 2);
  return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  set_zero(dst);
  if ((src != src) || (fabs(src) < MIN_FLOAT_IN_DECIMAL) ||
      (fabs(src) > MAX_FLOAT_IN_DECIMAL)) {
    return 1;
  }
  char sign = 0;
  if (src < 0) sign = 1;
  src = fabs(src);
  int exp = 0;
  while (src < 10000000) {
    exp++;
    src *= 10;
  }
  int depth = 0;
  while (src > 99999999) {
    depth++;
    src /= 10;
  }

  int source = (int)round(src);
  s21_from_int_to_decimal(source, dst);
  set_sign(sign, dst);
  set_exponent(exp, dst);
  for (int i = 0; i < depth; i++) s21_mul_10(dst);
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  // проверка условия
  int flag = 0;
  if (!check_allowed(&src)) flag = 1;
  s21_truncate(src, &src);  // отбрасываем дробную часть
  *dst = 0;
  for (int i = 32; i <= D_LAST_BIT_POS; ++i) {  // проверяем, влезет ли в int
    flag += get_bit(&src, i) == 1;  // все биты с 31 должны быть 0
  }
  if (!flag) {
    for (int i = 0; i < 31; ++i) {
      int bit = get_bit(&src, i);
      *dst += bit << i;
    }
    if (get_sign(&src) == 1) *dst *= (-1);
    if (get_bit(&src, 31) == 1) {
      if (*dst == 0 && get_sign(&src) == 1)
        *dst = MIN_INT;
      else {
        *dst = 0;
        flag = 1;
      }
    }
  } else
    flag = 1;
  return flag;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  *dst = 0;
  int result = 1;
  long double temp = 0;
  if (check_allowed(&src)) {
    for (int i = 0; i <= D_LAST_BIT_POS; ++i) {
      long double delta = get_bit(&src, i) * powl(2, i);
      temp += delta;
    }
    for (int i = 0; i < get_exponent(&src); ++i) temp /= 10;
    if (get_sign(&src)) temp *= (-1);
    *dst = (float)temp;
    result = 0;
  }
  return result;
}