#include <check.h>

#line 1
#include <math.h>
#include <stdio.h>

#include "s21_decimal.h"

int file_length(char* fileName) {
  int fileLength = 0;
  FILE* fp = fopen(fileName, "r");
  if (fp) {
    fseek(fp, 0, SEEK_END);
    fileLength = ftell(fp);
    fclose(fp);
  }
  return fileLength;
}

void read_compare_part(FILE* fp, s21_decimal* first, s21_decimal* second,
                       int* isLess, int* isGreater) {
  fread(first, sizeof(s21_decimal), 1, fp);
  fread(second, sizeof(s21_decimal), 1, fp);
  char c = 0;
  fread(&c, sizeof(char), 1, fp);
  *isLess = (int)c;
  fread(&c, sizeof(char), 1, fp);
  *isGreater = (int)c;
  return;
}

START_TEST(compare_module) {
#line 29
  // signature = decimal, decimal, <(int), >(int)
  char* fileName = "test/FILE/compare_test";
  int dateLen = 2 * (sizeof(s21_decimal) + sizeof(int));
  int fileLength = file_length(fileName) / dateLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; (i < fileLength); ++i) {
      s21_decimal first = {{0, 0, 0, 0}};
      s21_decimal second = {{0, 0, 0, 0}};
      int o_isLess = 0;
      int o_isGreater = 0;
      int isLess = 0;
      int isGreater = 0;
      read_compare_part(fp, &first, &second, &o_isLess, &o_isGreater);
      isLess = s21_is_less(first, second);
      isGreater = s21_is_greater(first, second);
      ck_assert_int_eq(isLess, o_isLess);
      ck_assert_int_eq(isGreater, o_isGreater);
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(int_to_decimal_module) {
#line 54
  // signature = int, decimal
  char* fileName = "test/FILE/int_to_decimal_test";
  int dateLen = sizeof(int) + sizeof(s21_decimal);
  int fileLength = file_length(fileName) / dateLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; i < fileLength; ++i) {
      int source = 0;
      s21_decimal result = {{0, 0, 0, 0}};
      s21_decimal o_result = result;
      fread(&source, sizeof(int), 1, fp);
      fread(&o_result, sizeof(s21_decimal), 1, fp);
      s21_from_int_to_decimal(source, &result);
      ck_assert_int_eq(1, s21_is_equal(result, o_result));
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(decimal_to_int_module) {
#line 74
  // signature = decimal, int.
  char* fileName = "test/FILE/decimal_to_int_test";
  int dateLen = sizeof(int) + sizeof(s21_decimal);
  int fileLength = file_length(fileName) / dateLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; i < fileLength; ++i) {
      s21_decimal source = {{0, 0, 0, 0}};
      int result = 0;
      int o_result = result;
      fread(&source, sizeof(s21_decimal), 1, fp);
      fread(&o_result, sizeof(int), 1, fp);
      s21_from_decimal_to_int(source, &result);
      ck_assert_int_eq(result, o_result);
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(float_to_decimal_module) {
#line 95
  // signature = float, decimal.
  char* fileName = "test/FILE/float_to_decimal_test";
  int dateLen = sizeof(float) + sizeof(s21_decimal);
  int fileLength = file_length(fileName) / dateLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; i < fileLength; ++i) {
      s21_decimal result = {{0, 0, 0, 0}};
      s21_decimal o_result = result;
      float date = 0;
      fread(&date, sizeof(float), 1, fp);
      fread(&o_result, sizeof(s21_decimal), 1, fp);
      s21_from_float_to_decimal(date, &result);
      ck_assert_int_eq(0, (s21_is_not_equal(result, o_result)));
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(decimal_to_float_module) {
#line 116
  // signature = float, decimal
  char* fileName = "test/FILE/decimal_to_float_test";
  int dateLen = sizeof(float) + sizeof(s21_decimal);
  int fileLength = file_length(fileName) / dateLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; i < fileLength; ++i) {
      s21_decimal source = {{0, 0, 0, 0}};
      float o_result = 0.0;
      float result = o_result;
      fread(&source, sizeof(s21_decimal), 1, fp);
      fread(&o_result, sizeof(float), 1, fp);
      s21_from_decimal_to_float(source, &result);
      ck_assert_float_eq_tol(result, o_result, 1e-6);
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(round_test_module) {
#line 137
  // signature = decimal, decimal(floor), decimal(round), decimal(truncate)
  char* fileName = "test/FILE/round_test";
  int dateLen = 4 * sizeof(s21_decimal);
  int fileLength = file_length(fileName) / dateLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; i < fileLength; ++i) {
      s21_decimal source = {{0, 0, 0, 0}};
      s21_decimal floor = source;
      s21_decimal o_floor = source;
      s21_decimal round = source;
      s21_decimal o_round = source;
      s21_decimal trunc = source;
      s21_decimal o_trunc = source;

      fread(&source, sizeof(s21_decimal), 1, fp);
      fread(&o_floor, sizeof(s21_decimal), 1, fp);
      fread(&o_round, sizeof(s21_decimal), 1, fp);
      fread(&o_trunc, sizeof(s21_decimal), 1, fp);

      s21_floor(source, &floor);
      s21_round(source, &round);
      s21_truncate(source, &trunc);

      ck_assert_int_eq(1, s21_is_equal(floor, o_floor));
      ck_assert_int_eq(1, s21_is_equal(round, o_round));
      ck_assert_int_eq(1, s21_is_equal(trunc, o_trunc));
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(add_test_module) {
#line 171
  // signature = decimal, decimal, decimal(result)
  char* fileName = "test/FILE/add_test";
  int dataLen = 3 * sizeof(s21_decimal);
  int fileLength = file_length(fileName) / dataLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; i < fileLength; ++i) {
      s21_decimal first = {{0, 0, 0, 0}};
      s21_decimal second = first;
      s21_decimal result = first;
      s21_decimal o_result = first;

      fread(&first, sizeof(s21_decimal), 1, fp);
      fread(&second, sizeof(s21_decimal), 1, fp);
      fread(&o_result, sizeof(s21_decimal), 1, fp);

      s21_add(first, second, &result);
      ck_assert_int_eq(1, s21_is_equal(result, o_result));
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(sub_test_module) {
#line 198
  // signature = decimal, decimal, decimal(result)
  char* fileName = "test/FILE/sub_test";
  int dataLen = 3 * sizeof(s21_decimal);
  int fileLength = file_length(fileName) / dataLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; i < fileLength; ++i) {
      s21_decimal first = {{0, 0, 0, 0}};
      s21_decimal second = first;
      s21_decimal result = first;
      s21_decimal o_result = first;

      fread(&first, sizeof(s21_decimal), 1, fp);
      fread(&second, sizeof(s21_decimal), 1, fp);
      fread(&o_result, sizeof(s21_decimal), 1, fp);

      s21_sub(first, second, &result);

      ck_assert_int_eq(1, s21_is_equal(result, o_result));
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(mul_test_module) {
#line 225
  // signature = decimal, decimal, decimal(result)
  char* fileName = "test/FILE/mul_test";
  int dataLen = 3 * sizeof(s21_decimal);
  int fileLength = file_length(fileName) / dataLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; i < fileLength; ++i) {
      s21_decimal first = {{0, 0, 0, 0}};
      s21_decimal second = first;
      s21_decimal result = first;
      s21_decimal o_result = first;

      fread(&first, sizeof(s21_decimal), 1, fp);
      fread(&second, sizeof(s21_decimal), 1, fp);
      fread(&o_result, sizeof(s21_decimal), 1, fp);

      s21_mul(first, second, &result);
      ck_assert_int_eq(1, s21_is_equal(result, o_result));
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(div_test_module) {
#line 250
  // signature = decimal, decimal, decimal(result)
  char* fileName = "test/FILE/div_test";
  int dataLen = 3 * sizeof(s21_decimal);
  int fileLength = file_length(fileName) / dataLen;

  FILE* fp = fopen(fileName, "rb");
  if (fp) {
    for (int i = 0; i < fileLength; ++i) {
      s21_decimal first = {{0, 0, 0, 0}};
      s21_decimal second = first;
      s21_decimal result = first;
      s21_decimal o_result = first;

      fread(&first, sizeof(s21_decimal), 1, fp);
      fread(&second, sizeof(s21_decimal), 1, fp);
      fread(&o_result, sizeof(s21_decimal), 1, fp);

      s21_div(first, second, &result);
      ck_assert_int_eq(1, s21_is_equal(result, o_result));
    }
    fclose(fp);
  }
}
END_TEST

START_TEST(is_greater_or_equal_test_module) {
#line 299
  s21_decimal zero = {0};
  s21_decimal max = {0};
  s21_decimal min = {0};

  set_zero(&zero);
  set_max_value(&max);
  set_min_value(&min);

  ck_assert_int_eq(1, s21_is_greater_or_equal(zero, zero));
  ck_assert_int_eq(1, s21_is_greater_or_equal(max, max));
  ck_assert_int_eq(1, s21_is_greater_or_equal(max, zero));
  ck_assert_int_eq(1, s21_is_greater_or_equal(max, min));
  ck_assert_int_eq(1, s21_is_greater_or_equal(zero, min));

  s21_decimal first = max;
  s21_decimal second = max;
  set_exponent(28, &second);

  ck_assert_int_eq(1, s21_is_greater_or_equal(first, second));
  ck_assert_int_eq(0, s21_is_greater_or_equal(second, first));
}
END_TEST

START_TEST(cast_errors_test_module) {
#line 321

  s21_decimal maxValue = {0};
  s21_decimal trashValue = {0};
  set_max_value(&maxValue);

  ck_assert_int_eq(1, s21_from_int_to_decimal(0, NULL));
  ck_assert_int_eq(1, s21_from_float_to_decimal(0.0, NULL));
  ck_assert_int_eq(1, s21_from_float_to_decimal(1e220, &trashValue));

  ck_assert_int_eq(1, s21_from_decimal_to_int(trashValue, NULL));
  ck_assert_int_eq(
      1, s21_from_decimal_to_int(maxValue, (int*)(&trashValue.bits[0])));

  ck_assert_int_eq(1, s21_from_decimal_to_float(trashValue, NULL));
}
END_TEST

START_TEST(add_and_sub_error_test_module) {
#line 336

  s21_decimal maxValue = {0};
  s21_decimal minValue = maxValue;
  s21_decimal tmp = maxValue;

  set_max_value(&maxValue);
  set_min_value(&minValue);

  ck_assert_int_eq(1, s21_add(maxValue, maxValue, NULL));
  ck_assert_int_eq(1, s21_sub(maxValue, maxValue, NULL));
  ck_assert_int_eq(1, s21_add(maxValue, maxValue, &tmp));
  ck_assert_int_eq(1, s21_sub(maxValue, minValue, &tmp));
  ck_assert_int_eq(2, s21_add(minValue, minValue, &tmp));
  ck_assert_int_eq(2, s21_sub(minValue, maxValue, &tmp));
}
END_TEST

START_TEST(round_null_test_module) {
#line 352

  s21_decimal number = {0};

  ck_assert_int_eq(1, s21_floor(number, NULL));
  ck_assert_int_eq(1, s21_round(number, NULL));
  ck_assert_int_eq(1, s21_truncate(number, NULL));
  ck_assert_int_eq(1, s21_negate(number, NULL));
}
END_TEST

START_TEST(else_test_module) {
#line 361

  s21_decimal maxValue = {0};
  set_max_value(&maxValue);

  s21_decimal tmp = maxValue;

  ck_assert_int_eq(1, s21_negate(maxValue, NULL));

  ck_assert_int_eq(1, mantis_increment(&tmp));

  set_zero(&tmp);
  tmp.bits[2] = 1;
  maxValue.bits[2] = 0;

  ck_assert_int_eq(0, mantis_increment(&maxValue));
  ck_assert_int_eq(1, s21_is_equal(tmp, maxValue));
}
END_TEST

int main(void) {
  Suite* s1 = suite_create("Core");
  TCase* tc1_1 = tcase_create("Core");
  SRunner* sr = srunner_create(s1);
  srunner_set_fork_status(sr, CK_NOFORK);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, compare_module);
  tcase_add_test(tc1_1, int_to_decimal_module);
  tcase_add_test(tc1_1, decimal_to_int_module);
  tcase_add_test(tc1_1, float_to_decimal_module);
  tcase_add_test(tc1_1, decimal_to_float_module);
  tcase_add_test(tc1_1, round_test_module);
  tcase_add_test(tc1_1, add_test_module);
  tcase_add_test(tc1_1, sub_test_module);
  tcase_add_test(tc1_1, mul_test_module);
  tcase_add_test(tc1_1, div_test_module);
  tcase_add_test(tc1_1, is_greater_or_equal_test_module);
  tcase_add_test(tc1_1, cast_errors_test_module);
  tcase_add_test(tc1_1, add_and_sub_error_test_module);
  tcase_add_test(tc1_1, round_null_test_module);
  tcase_add_test(tc1_1, else_test_module);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
