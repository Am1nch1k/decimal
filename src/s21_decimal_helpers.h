#ifndef C5_S21_DECIMAL_2_S21_DECIMAL_HELPERS_H
#define C5_S21_DECIMAL_2_S21_DECIMAL_HELPERS_H

#define dec_max "79228162514264337593543950335"
#define MAX 0xFFFFFFFF
#define DEC10       \
  (s21_decimal) {   \
    { 10, 0, 0, 0 } \
  }

#define DEC01                  \
  (s21_decimal) {              \
    { MAX, MAX, MAX, 1 << 16 } \
  }

#define D_SIGN_POS 127
#define D_LAST_BIT_POS 95
#define D_EXP_MASK 0x00FF0000
#define D_EXP_START_BIT 112
#define D_MAX_EXP 28
#define D_STR_LEN 32
#define S21_MAX_UINT 4294967295

#define MAX_FLOAT_IN_DECIMAL 7.922816e28
#define MIN_FLOAT_IN_DECIMAL 0.5e-28
#define MAX_INT 2147483647
#define MIN_INT -2147483648

int s21_divn_10(s21_decimal* value, int ten_power);
int s21_div_10(s21_decimal* value);
int s21_mul_10(s21_decimal* value);

int mantis_increment(s21_decimal* value);

void set_zero(s21_decimal* number);
void normalize(s21_decimal* a, s21_decimal* b);
char check_allowed(const s21_decimal* number);

#endif  // C5_S21_DECIMAL_2_S21_DECIMAL_HELPERS_H
