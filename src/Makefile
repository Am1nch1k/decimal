CC = gcc 
FLAGS = -Werror -Wextra -Wall -std=c11 -pedantic -g
GCOVFLAGS =  --coverage
SUITE_CASES_C = test/suite_*.c
S21_DEC_C = arithmetic.c bit_and_sign.c cast.c comparsion.c rounding.c
S21_DEC_O = arithmetic.o bit_and_sign.o cast.o comparsion.o rounding.o
OS = $(shell uname -s)
LINUX = -lcheck -lm -lrt -lpthread -lsubunit

all : clean s21_decimal.a

s21_decimal.o:  s21_decimal.h
		$(CC) $(FLAGS) -c $(S21_DEC_C)

s21_decimal.cov:  s21_decimal.h
		$(CC) $(FLAGS) -c $(S21_DEC_C) $(GCOVFLAGS)

s21_decimal.a: s21_decimal.o
		ar rcs s21_decimal.a $(S21_DEC_O)

s21_decimal.a_cov: s21_decimal.cov
		ar rcs s21_decimal.a $(S21_DEC_O) 

test: s21_decimal.a_cov test/s21_test.c
ifeq ($(OS), Darwin)
		$(CC)   -Werror -Wextra -Wall -std=c11 $(SUITE_CASES_C)  test/s21_test.c  s21_decimal.a -lcheck -o s21_test $(GCOVFLAGS)
else
		$(CC)  -Werror -Wextra -Wall -std=c11 $(SUITE_CASES_C) test/s21_test.c  -L. -l:s21_decimal.a -o s21_test  $(GCOVFLAGS) $(LINUX)

endif
		./s21_test

gcov_report: test
#gcovr -r . --html --html-details -o coverage_report.html
		lcov -t "gcov_report" -o s21_gcov_report.info -c -d .
		genhtml -o s21_report s21_gcov_report.info
		open ./s21_report/index.html


clean : 
		rm -rf s21_decimal.a *.o *.a *.gcno  *.gcda *.info report.out *.gcov  s21_test s21_report

style:
	cp ../materials/linters/.clang-format .
	cp ../materials/linters/.clang-format test
	clang-format -n *.c *.h
	clang-format -n test/*.c test/*.h
	clang-format -i *.c *.h
	clang-format -i test/*.c test/*.h
	clang-format -n *.c *.h
	clang-format -n test/*.c test/*.h
	rm .clang-format
	rm test/.clang-format
