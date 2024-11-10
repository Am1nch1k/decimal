#include "tests_suitcases.h"

START_TEST(div_test_1) {
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_decimal res_dec = {0};
  int out = s21_div(a, b, &res_dec);
  ck_assert_int_eq(out, 3);
}
END_TEST

START_TEST(div_test_2) {
  s21_decimal a, b, res_dec;
  s21_from_int_to_decimal(8, &a);
  s21_from_int_to_decimal(4, &b);
  int out = s21_div(a, b, &res_dec);
  int res_int;
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, 2);
  ck_assert_int_eq(out, 0);
}
END_TEST

START_TEST(div_test_3) {
  s21_decimal a, b, res_dec;
  s21_from_int_to_decimal(8, &a);
  s21_from_int_to_decimal(3, &b);
  int out = s21_div(a, b, &res_dec);
  float res_float;
  s21_from_decimal_to_float(res_dec, &res_float);
  ck_assert(fabs(res_float - 8 / 3.0) < 1e5);
  ck_assert_int_eq(out, 0);
}
END_TEST

START_TEST(div_test_4) {
  s21_decimal a, b, res_dec;
  s21_from_int_to_decimal(-8, &a);
  s21_from_int_to_decimal(3, &b);
  int out = s21_div(a, b, &res_dec);
  float res_float;
  s21_from_decimal_to_float(res_dec, &res_float);
  ck_assert(fabs(res_float - (-8) / 3.0) < 1e5);
  ck_assert_int_eq(out, 0);
}
END_TEST

START_TEST(div_test_5) {
  s21_decimal a, b, res_dec;
  s21_from_float_to_decimal(1e20, &a);
  s21_from_float_to_decimal(1e-20, &b);
  int out = s21_div(a, b, &res_dec);
  ck_assert_int_eq(out, 1);
}
END_TEST

START_TEST(div_test_6) {
  s21_decimal a, b, res_dec;
  s21_from_float_to_decimal(-1e20, &a);
  s21_from_float_to_decimal(1e-20, &b);
  int out = s21_div(a, b, &res_dec);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(div_test_7) {
  s21_decimal a, b, res_dec;
  s21_from_float_to_decimal(1e-20, &a);
  s21_from_float_to_decimal(1e20, &b);
  int out = s21_div(a, b, &res_dec);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(div_test_8) {
  s21_decimal a, b, res_dec;
  s21_from_float_to_decimal(-1e-20, &a);
  s21_from_float_to_decimal(1e20, &b);
  int out = s21_div(a, b, &res_dec);
  ck_assert_int_eq(out, 2);
}
END_TEST

Suite *suite_div(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_div");
  tc = tcase_create("case_div");

  tcase_add_test(tc, div_test_1);
  tcase_add_test(tc, div_test_2);
  tcase_add_test(tc, div_test_3);
  tcase_add_test(tc, div_test_4);
  tcase_add_test(tc, div_test_5);
  tcase_add_test(tc, div_test_6);
  tcase_add_test(tc, div_test_7);
  tcase_add_test(tc, div_test_8);
  suite_add_tcase(s, tc);
  return s;
}
