//
//  test.c
//  test
//
//  Created by mcbk on 19.01.2023.
//

#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "../calc_core.h"
#include "../stack.h"
#include "../string_utils.h"
#define EPS 1e-8

START_TEST(parent_test_1) {
  char buff[20] = " ())(";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, PARENTHESIS_ERROR);
  char buff1[20] = " ((1 + 2))";
  err = Parser(buff1, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) StackFree(&stack);
  }
}
END_TEST

START_TEST(parent_test_2) {
  char buff[20] = " (())()";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) StackFree(&stack);
}
END_TEST

START_TEST(func_test_1) {
  char buff[30] = "sincostg(atan)";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, FUNCTION_ERROR);
  if (!err) StackFree(&stack);
}
END_TEST

START_TEST(func_test_2) {
  char buff[30] = "sincostan(atan)";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, PARSE_ERROR);
    if (!err) StackFree(&stack);
  }
  char buff1[100] =
      "sin(cos(tan(atan(40)))) - acos(asin(sqrt(20))) + ln(log(3))";
  err = Parser(buff1, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) StackFree(&stack);
  }
}
END_TEST

START_TEST(parser_test_1) {
  char buff[30] = "+1+-2";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, PARSE_ERROR);
    if (!err) StackFree(&stack);
  }
}
END_TEST

START_TEST(parser_test_2) {
  char buff[30] = "+1+3-2";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) StackFree(&stack);
  }
}
END_TEST

START_TEST(parser_test_3) {
  char buff[30] = "(+1+3-2-)+2";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, PARSE_ERROR);
    if (!err) StackFree(&stack);
  }
}
END_TEST

START_TEST(parser_test_4) {
  char buff[30] = "(+1+3-2)+2-";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, PARSE_ERROR);
    if (!err) StackFree(&stack);
  }
}
END_TEST

START_TEST(parser_test_5) {
  char buff[50] = "(+1+3-2)(+2)cos3";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, PARSE_ERROR);
    if (!err) StackFree(&stack);
  }
}
END_TEST
START_TEST(parser_test_6) {
  char buff[50] = "(+1+3-2)*(+2)cos3";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, PARSE_ERROR);
    if (!err) StackFree(&stack);
  }
}
END_TEST

START_TEST(parser_test_7) {
  char buff[50] = "*(+1+3-2)*(+2)cos3";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, PARSE_ERROR);
    if (!err) StackFree(&stack);
  }
}
END_TEST

START_TEST(parser_test_8) {
  char buff[50] = "(*1+3-2)*(+2)cos3";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, PARSE_ERROR);
    if (!err) StackFree(&stack);
  }
}
END_TEST

START_TEST(parser_test_9) {
  char buff[50] = "5mof4";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, OPERATION_ERROR);
}
END_TEST

START_TEST(parser_test_10) {
  char buff[50] = "wiom";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, PARSE_ERROR);
}
END_TEST

Suite* parser_err_suite(void) {
  Suite* s;
  TCase* tc_core;
  s = suite_create("parser_err_test");
  tc_core = tcase_create("core");

  tcase_add_test(tc_core, parent_test_1);
  tcase_add_test(tc_core, parent_test_2);
  tcase_add_test(tc_core, func_test_1);
  tcase_add_test(tc_core, func_test_2);
  tcase_add_test(tc_core, parser_test_1);
  tcase_add_test(tc_core, parser_test_2);
  tcase_add_test(tc_core, parser_test_3);
  tcase_add_test(tc_core, parser_test_4);
  tcase_add_test(tc_core, parser_test_5);
  tcase_add_test(tc_core, parser_test_6);
  tcase_add_test(tc_core, parser_test_7);
  tcase_add_test(tc_core, parser_test_8);
  tcase_add_test(tc_core, parser_test_9);
  tcase_add_test(tc_core, parser_test_10);

  suite_add_tcase(s, tc_core);
  return s;
}

START_TEST(calc_test_1) {
  char* buff = "(1+3-2)*(+2)*cos(3)";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, 0);
      ck_assert_double_eq_tol(result, -3.95996998, EPS);
    }
  }
}
END_TEST

START_TEST(calc_test_2) {
  char* buff = "(1+3-2)*(+2)*cos(3)^2";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, 0);
      ck_assert_double_eq_tol(result, 3.92034057, EPS);
    }
  }
}
END_TEST

START_TEST(calc_test_3) {
  char* buff = "ln((1+3-2)*(+2))*cos(3)^2";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, 0);
      ck_assert_double_eq_tol(result, 1.3586865, EPS);
    }
  }
}
END_TEST

START_TEST(calc_test_4) {
  char* buff = "cos(3)^2 + (acos(0.5))";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, 0);
      ck_assert_double_eq_tol(result, 2.02728269, EPS);
    }
  }
}
END_TEST

START_TEST(calc_test_5) {
  char* buff = "tan(0.1) + atan(0.8)";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, 0);
      ck_assert_double_eq_tol(result, 0.775075614, EPS);
    }
  }
}
END_TEST

START_TEST(calc_test_6) {
  char* buff = "asin(1) + sqrt(4)*ln(0.1)";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, 0);
      ck_assert_double_eq_tol(result, -3.034373859, EPS);
    }
  }
}
END_TEST

START_TEST(calc_test_7) {
  char* buff = "5 mod 2 + log(10) + 4^(-1)";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, 0);
      ck_assert_double_eq_tol(result, 2.25, EPS);
    }
  }
}
END_TEST

START_TEST(calc_test_8) {
  char* buff = "1 / 0";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, CALC_ERROR);
    }
  }
}
END_TEST

START_TEST(calc_test_9) {
  char* buff = "-1 / 0";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, CALC_ERROR);
    }
  }
}
END_TEST

START_TEST(calc_test_10) {
  char* buff = "sqrt(-2)";
  Stack stack;
  int err = Parser(buff, &stack);
  ck_assert_int_eq(err, 0);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    ck_assert_int_eq(err, 0);
    if (!err) {
      Stack polish;
      MakePolish(&stack, &polish);
      double result = 0.0;
      err = CalculatePolish(&polish, &result);
      StackFree(&stack);
      StackFree(&polish);
      ck_assert_int_eq(err, CALC_ERROR);
    }
  }
}
END_TEST

START_TEST(calc_test_11) {
  char* buff = "sin(3.14)";
  double val = 0;
  int err = Calculate(buff, &val);
  ck_assert_int_eq(err, 0);
  ck_assert_double_eq_tol(0.0015926, val, 1e-7);
}
END_TEST

Suite* calc_suite(void) {
  Suite* s;
  TCase* tc_core;
  s = suite_create("parser_err_test");
  tc_core = tcase_create("core");
  tcase_add_test(tc_core, calc_test_1);
  tcase_add_test(tc_core, calc_test_2);
  tcase_add_test(tc_core, calc_test_3);
  tcase_add_test(tc_core, calc_test_4);
  tcase_add_test(tc_core, calc_test_5);
  tcase_add_test(tc_core, calc_test_6);
  tcase_add_test(tc_core, calc_test_7);
  tcase_add_test(tc_core, calc_test_8);
  tcase_add_test(tc_core, calc_test_9);
  tcase_add_test(tc_core, calc_test_10);
  tcase_add_test(tc_core, calc_test_11);
  suite_add_tcase(s, tc_core);
  return s;
}
int main() {
  Suite* tests[] = {parser_err_suite(), calc_suite(), NULL};
  int no_failed = 0;
  for (int i = 0; tests[i]; ++i) {
    SRunner* runner;
    runner = srunner_create(tests[i]);
    srunner_set_fork_status(runner, CK_NOFORK);
    srunner_run_all(runner, CK_NORMAL);
    no_failed += srunner_ntests_failed(runner);
    srunner_free(runner);
  }
  if (no_failed == 0) printf("TEST OK\n");
  //    double res;
  //    char* buf = "sin(1)";
  //    int err = Calculate(buf, &res);
  //    if (!err)
  //        printf("err = %d  res = %lf", err, res);
  //    else
  //        printf ("lol\n");
  //    char* buff = "cos(5)";
  //    Stack stack;
  //    int err = Parser(buff, &stack);
  //    if (!err) {
  //        MakeUnary(&stack);
  //        err = CheckStack(&stack);
  //
  //        if (!err) {
  //            Stack polish;
  //            MakePolish(&stack, &polish);
  //            double result = 0.0;
  //            err = CalculatePolish(&polish, &result);
  //            printf("err = %d  res = %.9lf\n", err, result);
  //            StackFree(&stack);
  //            StackFree(&polish);
  //
  //        }
  //        printf("err = %d\n", err);
  //    }

  //    Stack stack;
  //    char buff1[100] = " ((1 + 2))";
  //    int err = Parser(buff1, &stack);
  //
  //    if (!err) {
  //        MakeUnary(&stack);
  //        err = CheckStack(&stack);
  //        if (!err)
  //            StackFree(&stack);
  //    }
  //    FILE* file =
  //    fopen("/Users/a1234/Desktop/21/c/C7_SmartCalc_v1.0-0/src/model/test/testcases",
  //    "r"); if (file) {
  //        printf("hi");
  //        size_t len1 = 200, len2 = 200;
  //        char* buff1 = calloc(200, sizeof(char));
  //        char* buff2 = calloc(200, sizeof(char));
  //        size_t end = 1;
  //        int counter = 0;
  //        while (end != -1) {
  //            end = getline(&buff1, &len1, file);
  //            ++counter;
  //            if (end != -1) {
  //                buff1[end - 1] = '\0';
  //                end = getline(&buff2, &len2, file);
  //                int err;
  //                sscanf(buff2, "%d", &err);
  //                printf ("N%d %d - %s\n",counter, err, buff1);
  //            }
  //        }
  //        free(buff1);
  //        free(buff2);
  //        fclose(file);
  //    }

  //    FILE* file =
  //    fopen("/Users/a1234/Desktop/21/c/C7_SmartCalc_v1.0-0/src/model/test/testcases",
  //    "r"); if (file) {
  //        size_t len1 = 200, len2 = 200;
  //        char* buff1 = calloc(200, sizeof(char));
  //        char* buff2 = calloc(200, sizeof(char));
  //        size_t end = 1;
  //        int counter = 0;
  //        while (end != -1) {
  //            end = getline(&buff1, &len1, file);
  //            ++counter;
  //            if (end != -1) {
  //                buff1[end - 1] = '\0';
  //                end = getline(&buff2, &len2, file);
  //                int true_err;
  //                sscanf(buff2, "%d", &true_err);
  //                //if (!str_e)
  //                Stack stack;
  //                int err = Parser(buff1, &stack);
  //                if (err != true_err)
  //                    printf("err in counter = %d\n", counter);
  //                if (!err) {
  //                    MakeUnary(&stack);
  //                    err = CheckStack(&stack);
  //                    if (err != true_err)
  //                        printf("err in counter = %d\n", counter);
  //                    if (!err)
  //                        StackFree(&stack);
  //                }
  //            }
  //        }
  //        free(buff1);
  //        free(buff2);
  //        fclose(file);
  //    }
  return 0;
}
