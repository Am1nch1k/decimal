#ifndef S21_DECIMAL_H_
#define S21_DECIMAL_H_

#include <math.h>
#include <stdint.h>

typedef struct {
  uint32_t bits[4];
} s21_decimal;

// BIT AND SIGN FUNCTIONS//

char set_bit(s21_decimal* target, int bit_pos, char bit);
char get_bit(const s21_decimal* source, int bit_pos);

char set_sign(char sign, s21_decimal* number);
char get_sign(const s21_decimal* number);

char set_exponent(int exp, s21_decimal* number);
int get_exponent(const s21_decimal* number);

// CAST FUNCTIONS//

int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_float_to_decimal(float src, s21_decimal* dst);

int s21_from_decimal_to_int(s21_decimal src, int* dst);
int s21_from_decimal_to_float(s21_decimal src, float* dst);

// BASED ARITHMETIC FUNCTIONS//

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// COMPARSION FUNCTIONS//

int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);
int s21_is_zero(s21_decimal a);

// ROUNDING FUNCTIONS//

int s21_floor(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal* result);

#include "s21_decimal_helpers.h"
#endif  //  S21_DECIMAL_H_
