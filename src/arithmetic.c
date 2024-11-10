#include <stdio.h>

#include "s21_decimal.h"

int s21_divn_10(s21_decimal* value, int tenPower) {
  int isreminder = 0;
  while (tenPower) {
    uint32_t localreminder = s21_div_10(value);
    if (localreminder) isreminder = 1;
    --tenPower;
  }
  return isreminder;
}

int s21_div_10(s21_decimal* value) {
  uint64_t divider = 10;
  uint64_t reminder = 0;
  int shiftNumber = 32;

  for (int i = 4 - 2; i >= 0; --i) {
    uint64_t current = (uint64_t)(value->bits[i]) + reminder;
    value->bits[i] = (int)(current / divider);
    reminder = (current % divider) << shiftNumber;
  }
  return (int)(reminder >> shiftNumber);
}

int s21_mul_10(s21_decimal* value) {
  int error_status = 0;
  int shift_number = 32;
  uint64_t down_mask = 0xffffffff;
  uint64_t tmp = 0;

  for (int i = 0; i <= 4 - 2; ++i) {
    tmp += (10 * (uint64_t)(value->bits[i]));
    value->bits[i] = (uint32_t)(tmp & down_mask);
    tmp >>= shift_number;
  }
  if (tmp != 0) error_status = 1;
  return error_status;
}

int mantis_increment(s21_decimal* value) {
  int error_status = 0;
  if ((++(value->bits[0])) == 0) {
    if ((++(value->bits[1])) == 0) {
      if ((++(value->bits[2])) == 0) {
        error_status = 1;
      }
    }
  }
  return error_status;
}

void decimal_to_bcd(s21_decimal bin, unsigned char* bcd) {
  unsigned char tmp[28] = {0};
  int b = bin.bits[0];
  bin.bits[0] = bin.bits[2];
  bin.bits[2] = b;
  for (int i = 0; i < 12; i++) {
    int first = 8 * (i % 4);
    tmp[19 + 4 * (i / 4) - (i % 4)] =
        ((bin.bits[i / 4]) & ((unsigned)(0x000000FF) << first)) >> first;
  }
  for (int i = 0; i < 96; i++) {
    for (int j = 0; j < 28; j++) {
      tmp[j] <<= 1;
      if ((j < 27) && (tmp[j + 1] & 0x80)) tmp[j] |= 1;
    }
    for (int j = 0; ((i != 95) && (j < 16)); j++)
      tmp[j] += 0x30 * (((tmp[j] >> 4) & 0x0f) > 4) + 3 * ((tmp[j] & 0x0f) > 4);
  }
  for (int i = 0; i < 32; i++)
    bcd[i] = (i & 1) ? (tmp[i >> 1]) & 0x0f : ((tmp[i >> 1] >> 4) & 0x0f);
  bcd[0] = (get_sign(&bin) != 0);
  bcd[1] = bin.bits[3] >> 16;
}

int bcd_to_decimal(s21_decimal* bin, unsigned char* bcd) {
  unsigned char tmp[28] = {0};
  for (int i = 0; i < 4; i++) bin->bits[i] = 0;
  bin->bits[3] = (bcd[0] << 31) + (bcd[1] << 16);
  for (int i = 0; i < 3; i++) bcd[i] = 0;
  for (int i = 0; i < 16; i++) tmp[i] = (bcd[2 * i] << 4) | bcd[2 * i + 1];
  for (int i = 0; i < 96; i++) {
    for (int j = 27; j >= 0; j--) {
      tmp[j] = (tmp[j] >> 1) & 0x7F;
      if (j && (tmp[j - 1] & 1)) tmp[j] |= 0x80;
    }
    for (int j = 0; ((i != 95) && (j < 16)); j++)
      tmp[j] -= 0x30 * (((tmp[j] >> 4) & 0x0f) > 7) + 3 * ((tmp[j] & 0x0f) > 7);
  }
  for (int j = 0; j < 16; j++)
    if (tmp[j] != 0) return 1;
  for (register int i = 0; i < 3; i++) {
    int first = tmp[16 + i * 4];
    for (int j = 17 + i * 4; j < 20 + i * 4; j++) first = (first << 8) | tmp[j];
    bin->bits[i] = first;
  }
  int first = bin->bits[0];
  bin->bits[0] = bin->bits[2];
  bin->bits[2] = first;
  return 0;
}

void bcd_shift_right(unsigned char* a) {
  for (; (a[1] && !a[31]); a[2] = 0, a[1]--)
    for (int i = 31; i > 1; i--) a[i] = a[i - 1];
}
void bcd_shift_left(unsigned char* a, int n) {
  int ans = 0;
  for (int i = 2; i < n + 1; i++) ans += a[i];
  if (ans == 0) a[1] = 28;
  for (; ((!a[3]) && (!a[2]) && (ans)); a[n] = 0, a[1]++)
    for (int i = 3; i < n; i++) a[i] = a[i + 1];
}
void bcd_norm(unsigned char* bcd, int n) {
  for (int i = n; i > 2; i--) {
    if (bcd[i] > 127) {
      bcd[i] += 10;
      bcd[i - 1] -= 1;
    }
    bcd[i - 1] += bcd[i] / 10;
    bcd[i] %= 10;
  }
}
void bank(unsigned char* bcd) {
  if (!bcd[2] || bcd[1]) {
    if (bcd[31 + (bcd[2] == 0)] != 5)
      bcd[30 + (bcd[2] == 0)] += (bcd[31 + (bcd[2] == 0)] > 5);
    else
      bcd[30 + (bcd[2] == 0)] += bcd[30 + (bcd[2] == 0)] & 1;
    bcd[31 + (bcd[2] == 0)] = 0;
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int errorStatus = 0;
  if (result) {
    unsigned char bcd_1[33] = {0}, bcd_2[33] = {0}, bsd_result[33] = {0};
    unsigned char sign = (get_sign(&value_1) != 0);
    unsigned char s = 1;
    decimal_to_bcd(value_1, bcd_1);
    decimal_to_bcd(value_2, bcd_2);
    bsd_result[0] = sign;
    if (sign != (get_sign(&value_2) != 0)) {
      s = -1;
      value_1.bits[3] &= 0x0FFFFFFF;
      value_2.bits[3] &= 0x0FFFFFFF;
      bsd_result[0] ^= (s21_is_less(value_1, value_2));
    }
    bcd_shift_left(bcd_1, 31);
    bcd_shift_left(bcd_2, 31);
    bsd_result[1] = (bcd_1[1] > bcd_2[1]) ? bcd_2[1] : bcd_1[1];
    for (; bcd_1[1] > bsd_result[1]; bcd_1[1]--, bcd_1[3] = 0)
      for (int i = 32; i > 3; i--) bcd_1[i] = bcd_1[i - 1];
    for (; bcd_2[1] > bsd_result[1]; bcd_2[1]--, bcd_2[3] = 0)
      for (int i = 32; i > 3; i--) bcd_2[i] = bcd_2[i - 1];
    for (int i = 32; i > 2; i--)
      bsd_result[i] = (s21_is_greater(value_1, value_2))
                          ? bcd_1[i] + s * bcd_2[i]
                          : bcd_2[i] + s * bcd_1[i];
    bcd_norm(bsd_result, 32);
    bcd_shift_left(bsd_result, 32);
    for (int i = 0; i < 29 && (bsd_result[i + 3] < dec_max[i] - '0'); i++) {
      if (bsd_result[i + 3] > dec_max[i] - '0') {
        if (bsd_result[1]) {
          for (int j = 32; j > 2; j--) bsd_result[j] = bsd_result[j - 1];
          bsd_result[1]--;
        }
      }
    }
    bank(bsd_result);
    bcd_shift_right(bsd_result);
    bcd_1[0] = (bsd_result[0] + 1) *
               (bsd_result[2] | bcd_to_decimal(result, bsd_result));
    for (int i = 0; (i < 4) & bcd_1[0]; i++) result->bits[i] = 0;
    if (s21_is_zero(*result)) result->bits[3] = 0;
    errorStatus = bcd_1[0];
  } else {
    errorStatus = 1;
  }
  return errorStatus;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  s21_negate(value_2, &value_2);
  return s21_add(value_1, value_2, result);
}

int get_power(s21_decimal value) {
  float val_float;
  s21_from_decimal_to_float(value, &val_float);
  val_float = fabs(val_float);
  int power;

  if (val_float >= 1)
    power = (int)log10f(val_float);
  else
    power = -(int)log10f(1 / val_float);
  return power;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  //  0 — OK;
  //  1 — число слишком велико или равно бесконечности;
  //  2 — число слишком мало или равно отрицательной бесконечности;
  //  3 — деление на 0.
  //  4 — result не инециализировн.
  if ((get_power(value_1) + get_power(value_2)) > 28) {
    if (get_sign(&value_1) == get_sign(&value_2))
      return 1;
    else
      return 2;
  }
  if ((get_power(value_1) + get_power(value_2)) < -28) {
    return 2;
  }
  if (!result) return 4;
  unsigned char bcd_1[32], bcd_2[32], bsd_result[33] = {0}, ans[58] = {0};
  for (int i = 0; i < 4; i++) result->bits[i] = 0;
  if (s21_is_zero(value_1) || s21_is_zero(value_2)) return 0;
  decimal_to_bcd(value_1, bcd_1);
  decimal_to_bcd(value_2, bcd_2);
  bsd_result[0] = bcd_1[0] ^ bcd_2[0];
  bsd_result[1] = bcd_1[1] + bcd_2[1];
  for (int i = 0; i < 29; i++) {
    for (int j = 0; j < 29; j++) {
      ans[57 - i - j] += bcd_1[31 - j] * bcd_2[31 - i];
      for (int k = 57; k > 0; ans[k--] %= 10) ans[k - 1] += ans[k] / 10;
    }
  }
  for (; (!ans[0] && (bsd_result[1] < 57)); ans[57] = 0, bsd_result[1]++)
    for (int i = 0; i < 57; i++) ans[i] = ans[i + 1];
  if (bsd_result[1] >= 29) {
    bsd_result[1] -= 29;
    for (int i = 3, j = (bsd_result[1] > 28) * (bsd_result[1] - 28); i < 33;
         i++)
      bsd_result[i + j] = ans[i - 3];
    if (bsd_result[1] > 28) bsd_result[1] -= 28;
    bank(bsd_result);
    bcd_shift_right(bsd_result);
    bcd_to_decimal(result, bsd_result);
    if (s21_is_zero(*result)) result->bits[3] = 0;
  }
  return 0;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  //  0 — OK;
  //  1 — число слишком велико или равно бесконечности;
  //  2 — число слишком мало или равно отрицательной бесконечности;
  //  3 — деление на 0.
  //  4 — result не инециализировн.
  if (s21_is_zero(value_2)) return 3;
  if ((get_power(value_1) - get_power(value_2)) > 28) {
    if (get_sign(&value_1) == get_sign(&value_2))
      return 1;
    else
      return 2;
  }
  if ((get_power(value_1) - get_power(value_2)) < -28) {
    return 2;
  }
  if (!result) return 4;
  unsigned char bcd[33] = {0}, k;
  for (int i = 0; i < 4; i++) result->bits[i] = 0;

  bcd[1] = 28;
  bcd[0] = (get_sign(&value_1) ^ get_sign(&value_2));
  value_1.bits[3] &= 0x0FFFFFFF;
  value_2.bits[3] &= 0x0FFFFFFF;
  s21_decimal tmp = value_2;
  while (s21_is_greater(value_1, tmp) && s21_is_greater_or_equal(DEC01, tmp)) {
    bcd[1]--;
    s21_mul(value_2 = tmp, DEC10, &tmp);
  }
  if (bcd[1] < 28) bcd[1]++;
  for (int i = 3; i < 33; i++) {
    for (k = 0; s21_is_greater_or_equal(value_1, value_2); k++)
      s21_sub(value_1, value_2, &value_1);
    s21_mul(value_1, DEC10, &value_1);
    bcd[i] = k;
  }
  bcd_norm(bcd, 32);
  bank(bcd);
  bcd_shift_right(bcd);
  bcd_to_decimal(result, bcd);
  for (int i = 0; (i < 4) && ((char)bcd[1] < 0); i++) result->bits[i] = 0;
  if (s21_is_zero(*result)) result->bits[3] = 0;
  return 0;
}
