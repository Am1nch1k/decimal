#include "tests_suitcases.h"

START_TEST(s21_from_float_to_decimal_1) {
  s21_decimal val;
  float fl1 = 3;
  float fl2 = 127.1234;
  float fl1_res = 0;
  float fl2_res = 0;
  s21_from_float_to_decimal(fl1, &val);
  s21_from_decimal_to_float(val, &fl1_res);
  ck_assert_double_eq_tol(fl1, fl1_res, 1e-28);
  s21_from_float_to_decimal(fl2, &val);
  s21_from_decimal_to_float(val, &fl2_res);
  ck_assert_double_eq_tol(fl2, fl2_res, 1e-28);
}
END_TEST

START_TEST(s21_from_float_to_decimal_5) {
  s21_decimal val;
  s21_from_float_to_decimal(1.02E+09, &val);
  ck_assert_int_eq(val.bits[0], 1020000000);
  ck_assert_int_eq(val.bits[1], 0);
  ck_assert_int_eq(val.bits[2], 0);
  ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_8) {
  s21_decimal val;
  float a = -1.0 / 0.0;
  int b = s21_from_float_to_decimal(a, &val);
  ck_assert_int_eq(b, 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal_11) {
  s21_decimal dec;
  float tmp = 0;
  float tmp1 = 0.03;
  s21_from_float_to_decimal(tmp1, &dec);
  s21_from_decimal_to_float(dec, &tmp);
  ck_assert_float_eq_tol(tmp, tmp1, 1e-06);
}
END_TEST

START_TEST(s21_from_float_to_decimal_12) {
  s21_decimal dec;
  s21_from_float_to_decimal(-128.023, &dec);
  float tmp = 0;
  s21_from_decimal_to_float(dec, &tmp);
  ck_assert_float_eq_tol(tmp, -128.023, 1e-06);
}
END_TEST

START_TEST(s21_from_float_to_decimal_15) {
  s21_decimal dec;
  s21_from_float_to_decimal(1.02E+08, &dec);
  float tmp = 0;
  s21_from_decimal_to_float(dec, &tmp);
  ck_assert_float_eq_tol(tmp, 1.02E+08, 1e-06);
}
END_TEST

START_TEST(s21_from_float_to_decimal_16) {
  s21_decimal dec;
  s21_from_float_to_decimal(-333.2222, &dec);
  float tmp = 0;
  s21_from_decimal_to_float(dec, &tmp);
  ck_assert_float_eq_tol(tmp, -333.2222, 1e-06);
}
END_TEST

START_TEST(s21_from_float_to_decimal_17) {
  s21_decimal dec;
  float a = 1.0 / 0.0;
  int ret = s21_from_float_to_decimal(a, &dec);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal_18) {
  s21_decimal dec;
  float a = -1.0 / 0.0;
  int b = s21_from_float_to_decimal(a, &dec);
  ck_assert_int_eq(b, 1);
}
END_TEST

Suite *suite_from_float_to_decimal(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_from_float_to_decimal");
  tc = tcase_create("case_from_float_to_decimal");
  tcase_add_test(tc, s21_from_float_to_decimal_1);
  tcase_add_test(tc, s21_from_float_to_decimal_5);
  tcase_add_test(tc, s21_from_float_to_decimal_8);
  tcase_add_test(tc, s21_from_float_to_decimal_11);
  tcase_add_test(tc, s21_from_float_to_decimal_12);
  tcase_add_test(tc, s21_from_float_to_decimal_15);
  tcase_add_test(tc, s21_from_float_to_decimal_16);
  tcase_add_test(tc, s21_from_float_to_decimal_17);
  tcase_add_test(tc, s21_from_float_to_decimal_18);

  suite_add_tcase(s, tc);
  return s;
}