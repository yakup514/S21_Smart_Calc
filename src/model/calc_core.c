//
//  calc_core.c
//  test
//
//  Created by mcbk on 19.01.2023.
//

#include "calc_core.h"

void MakePolish(Stack* stack, Stack* result) {
  if (stack->pos >= 0) {
    StackInit(result);
    Stack operation_stack;
    StackInit(&operation_stack);
    for (int i = 0; i <= stack->pos; ++i) {
      if (stack->data[i].type == 0) {
        result->data[++result->pos] = stack->data[i];
      } else if (stack->data[i].type == 2) {
        operation_stack.data[++operation_stack.pos] = stack->data[i];
      } else if (stack->data[i].type == 1) {
        if (operation_stack.pos < 0 || stack->data[i].oper[0] == '(') {
          operation_stack.data[++operation_stack.pos] = stack->data[i];
        } else if (StackPeek(&operation_stack).prior < stack->data[i].prior) {
          operation_stack.data[++operation_stack.pos] = stack->data[i];
        } else {
          int get_par = 0;
          while (operation_stack.pos >= 0 &&
                 StackPeek(&operation_stack).prior >= stack->data[i].prior &&
                 !get_par) {
            if (StackPeek(&operation_stack).oper[0] != '(') {
              result->data[++result->pos] = StackPop(&operation_stack);
            } else {
              StackPop(&operation_stack);
              get_par = 1;
            }
          }
          if (stack->data[i].oper[0] != ')')
            operation_stack.data[++operation_stack.pos] = stack->data[i];
        }
      }
      //            StackPrintLine(result);
      //            StackPrintLine(&operation_stack);
    }
    while (operation_stack.pos >= 0) {
      result->data[++result->pos] = StackPop(&operation_stack);
      //  StackPrintLine(result);
    }
    StackFree(&operation_stack);
  }
}
double Evaluate(double val1, double val2, char* operation) {
  double result = 0;
  if (operation[0] == '+')
    result = val1 + val2;
  else if (operation[0] == '-')
    result = val2 - val1;
  else if (!strcmp(operation, "u-"))
    result = 0 - val1;
  else if (!strcmp(operation, "u+"))
    result = val1;
  else if (operation[0] == '*')
    result = val1 * val2;
  else if (operation[0] == '/')
    result = val2 / val1;
  else if (operation[0] == '^')
    result = pow(val2, val1);
  else if (!strcmp(operation, "mod"))
    result = fmod(val2, val1);
  else if (!strcmp(operation, "sin"))
    result = sin(val1);
  else if (!strcmp(operation, "cos"))
    result = cos(val1);
  else if (!strcmp(operation, "tan"))
    result = tan(val1);
  else if (!strcmp(operation, "acos"))
    result = acos(val1);
  else if (!strcmp(operation, "asin"))
    result = asin(val1);
  else if (!strcmp(operation, "atan"))
    result = atan(val1);
  else if (!strcmp(operation, "sqrt"))
    result = sqrt(val1);
  else if (!strcmp(operation, "ln"))
    result = log(val1);
  else if (!strcmp(operation, "log"))
    result = log10(val1);
  return result;
}

int CalculatePolish(Stack* stack, double* result) {
  int err = 0;
  Stack result_stack;
  StackInit(&result_stack);
  for (int i = 0; i <= stack->pos && err == 0; ++i) {
    if (stack->data[i].type == 0) {
      result_stack.data[++result_stack.pos] = stack->data[i];
    } else if (stack->data[i].type == 1 && stack->data[i].oper[0] != 'u') {
      double val1 = StackPop(&result_stack).val;
      double val2 = StackPop(&result_stack).val;
      double temp = Evaluate(val1, val2, stack->data[i].oper);
      if (temp != temp || temp == INFINITY || temp == -INFINITY)
        err = CALC_ERROR;
      else
        result_stack.data[++result_stack.pos].val = temp;
    } else if (stack->data[i].type == 2 || stack->data[i].oper[0] == 'u') {
      double val1 = StackPop(&result_stack).val;
      double temp = Evaluate(val1, 0, stack->data[i].oper);
      if (temp != temp || temp == INFINITY || temp == -INFINITY)
        err = CALC_ERROR;
      else
        result_stack.data[++result_stack.pos].val = temp;
    }
    //  StackPrintLine(&result_stack);
  }
  if (err == 0) *result = result_stack.data[result_stack.pos].val;
  StackFree(&result_stack);
  return err;
}

int Calculate(char* data, double* result) {
  int err = 0;
  Stack stack;
  // StackInit(&stack);
  err = Parser(data, &stack);
  if (!err) {
    MakeUnary(&stack);
    err = CheckStack(&stack);
    if (!err) {
      Stack polish_stack;
      MakePolish(&stack, &polish_stack);
      StackFree(&stack);
      err = CalculatePolish(&polish_stack, result);
      StackFree(&polish_stack);
    }
  }

  return err;
}
