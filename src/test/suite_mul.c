#include "tests_suitcases.h"

START_TEST(mul_test_1) {
  int num1 = -10;
  int num2 = -10;
  int prod_int = 100;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, prod_int);
}
END_TEST

START_TEST(mul_test_2) {
  int num1 = 10;
  int num2 = 20;
  int prod_int = 200;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, prod_int);
}
END_TEST

START_TEST(mul_test_3) {
  int num1 = -10;
  int num2 = 20;
  int prod_int = -200;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, prod_int);
}
END_TEST

START_TEST(mul_test_4) {
  int num1 = 9403;
  int num2 = 202;
  int res_origin = 1899406;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, res_origin);
}
END_TEST

START_TEST(mul_test_5) {
  int num1 = -32768;
  int num2 = 2;
  int res_origin = -65536;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, res_origin);
}
END_TEST

START_TEST(mul_test_6) {
  int num1 = -32768;
  int num2 = 32768;
  int res_origin = -1073741824;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, res_origin);
}
END_TEST

START_TEST(mul_test_7) {
  float num1 = 94030;
  int num2 = 202;
  float res_origin = 18994060;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_float_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  float res_float = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_float(res_dec, &res_float);
  ck_assert_float_eq(res_float, res_origin);
}
END_TEST

START_TEST(mul_test_8) {
  float num1 = 94030;
  float num2 = 94030;
  float res_origin = 8841640900;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_float_to_decimal(num1, &a);
  s21_from_float_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  float res_float = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_float(res_dec, &res_float);
  ck_assert_float_eq(res_float, res_origin);
}
END_TEST

START_TEST(s21_mul_1) {
  s21_decimal dec1 = {{4, 0, 0, 0}};
  s21_decimal dec2 = {{10, 0, 0, 0}};

  s21_decimal d22;
  s21_mul(dec1, dec2, &d22);

  int qqqq = 0;
  s21_from_decimal_to_int(d22, &qqqq);
  ck_assert_int_eq(qqqq, 40);
}
END_TEST

START_TEST(s21_mul_2) {
  s21_decimal a, b, res_dec;
  s21_from_float_to_decimal(1e20, &a);
  s21_from_float_to_decimal(1e20, &b);
  int out = s21_mul(a, b, &res_dec);
  ck_assert_int_eq(out, 1);
}
END_TEST

START_TEST(s21_mul_3) {
  s21_decimal a, b, res_dec;
  s21_from_float_to_decimal(-1e20, &a);
  s21_from_float_to_decimal(1e20, &b);
  int out = s21_mul(a, b, &res_dec);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(s21_mul_4) {
  s21_decimal a, b, res_dec;
  s21_from_float_to_decimal(1e-20, &a);
  s21_from_float_to_decimal(1e-20, &b);
  int out = s21_mul(a, b, &res_dec);
  ck_assert_int_eq(out, 2);
}
END_TEST

START_TEST(s21_mul_5) {
  s21_decimal a, b, res_dec;
  s21_from_float_to_decimal(-1e-20, &a);
  s21_from_float_to_decimal(1e-20, &b);
  int out = s21_mul(a, b, &res_dec);
  ck_assert_int_eq(out, 2);
}
END_TEST

Suite *suite_mul(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_mul");
  tc = tcase_create("case_mul");

  tcase_add_test(tc, mul_test_1);
  tcase_add_test(tc, mul_test_2);
  tcase_add_test(tc, mul_test_3);
  tcase_add_test(tc, mul_test_4);
  tcase_add_test(tc, mul_test_5);
  tcase_add_test(tc, mul_test_6);
  tcase_add_test(tc, mul_test_7);
  tcase_add_test(tc, mul_test_8);
  tcase_add_test(tc, s21_mul_1);
  tcase_add_test(tc, s21_mul_2);
  tcase_add_test(tc, s21_mul_3);
  tcase_add_test(tc, s21_mul_4);
  tcase_add_test(tc, s21_mul_5);

  suite_add_tcase(s, tc);
  return s;
}
