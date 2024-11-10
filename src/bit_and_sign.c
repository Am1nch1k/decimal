//
// Created by Andrey on 26.08.2024.
//
#include "s21_decimal.h"

char set_bit(s21_decimal* target, int bit_pos, char bit) {
  if (bit_pos >= 0 && bit_pos < 128 && (bit == 0 || bit == 1)) {
    unsigned int mask = 1 << (bit_pos % 32);
    if (bit == 1)
      target->bits[bit_pos / 32] |= mask;
    else {
      mask = ~mask;
      target->bits[bit_pos / 32] &= mask;
    }
    return 0;
  }
  return -1;
}

char get_bit(const s21_decimal* source, int bit_pos) {
  if (bit_pos >= 0 && bit_pos < 128) {
    unsigned int mask = 1 << (bit_pos % 32);
    return (source->bits[bit_pos / 32] & mask) != 0;
  }
  return -1;
}

char set_sign(char sign, s21_decimal* number) {
  return set_bit(number, D_SIGN_POS, sign);
}

char get_sign(const s21_decimal* number) { return get_bit(number, D_SIGN_POS); }

char set_exponent(int exp, s21_decimal* number) {
  if (exp >= 0 && exp <= D_MAX_EXP) {
    for (int i = 0; i < 8; ++i, exp >>= 1)
      set_bit(number, D_EXP_START_BIT + i, exp % 2);
    return 0;
  }
  return -1;
}

int get_exponent(const s21_decimal* number) {
  int exp = 0;
  for (int i = 0; i < 8; ++i) {
    exp += get_bit(number, D_EXP_START_BIT + i) << i;
  }
  return exp;
}

void set_zero(s21_decimal* number) {
  for (int i = 0; i < 4; ++i) {
    number->bits[i] = 0;
  }
}

char check_allowed(const s21_decimal* number) {
  int result = 1;
  for (int i = 96 + 0; i <= 96 + 15 && result; ++i) {
    if (get_bit(number, i)) result = 0;
  }
  for (int i = 96 + 24; i <= 96 + 30 && result; ++i) {
    if (get_bit(number, i)) result = 0;
  }
  if (result == 1 && (get_exponent(number) < 0 || get_exponent(number) > 28)) {
    result = 0;
  }
  return result;
}

void normalize(s21_decimal* a, s21_decimal* b) {
  s21_decimal *lower_exp, *greater_exp;
  if (get_exponent(a) > get_exponent(b)) {
    greater_exp = a;
    lower_exp = b;
  } else {
    greater_exp = b;
    lower_exp = a;
  }
  int delta_exp = get_exponent(greater_exp) - get_exponent(lower_exp);
  for (int i = 0; i < delta_exp; ++i) s21_mul_10(lower_exp);
  set_exponent(get_exponent(greater_exp), lower_exp);
}