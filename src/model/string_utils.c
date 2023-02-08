//
//  string_utils.c
//  test
//
//  Created by mcbk on 16.01.2023.
//

#include "string_utils.h"
static const char kFunctions[9][5] = {"sin",  "cos",  "tan", "acos", "asin",
                                      "atan", "sqrt", "ln",  "log"};

int CheckFunction(char* data) {
  int err = FUNCTION_ERROR;
  for (int i = 0; i < 9; ++i) {
    if (!strcmp(kFunctions[i], data)) {
      err = 0;
      break;
    }
  }
  return err;
}
int CheckParenthesis(char* data) {
  int err = 0;
  int ind = 0;
  char ch = ' ';
  int p_opened = 0;
  while (ch != '\0' && err == 0) {
    ch = data[ind++];
    if (ch == '(')
      ++p_opened;
    else if (ch == ')' && p_opened > 0)
      --p_opened;
    else if (ch == ')' && p_opened == 0) {
      err = 3;
    }
  }
  err = (p_opened == 0 && err == 0) ? 0 : PARENTHESIS_ERROR;
  return err;
}

int GetFunc(char** ch, char* buff) {
  int err = 0;
  int counter = 0;
  memset(buff, '\0', 5);
  while (**ch != '(' && **ch != '\0' && counter < 4 && CheckFunction(buff)) {
    buff[counter++] = *((*ch)++);
  }
  buff[counter] = '\0';
  err = CheckFunction(buff);
  return err;
}

int IsFunc(char ch) {
  if (ch == 's' || ch == 'c' || ch == 't' || ch == 'a' || ch == 'l')
    return 1;
  else
    return 0;
}

int IsDigit(char ch) {
  int result = 0;
  if ((ch >= '0' && ch <= '9')) result = 1;
  return result;
}

double GetNumber(char** ch) {
  double result = 0;
  int counter = 0;
  sscanf(*ch, "%lf%n", &result, &counter);
  for (int i = 0; i < counter; ++i) {
    ++(*ch);
  }
  return result;
}

int IsOperation(char ch) {
  int result = 0;
  if (ch == '*' || ch == '/' || ch == '-' || ch == '+' || ch == 'm' ||
      ch == '(' || ch == ')' || ch == '^')
    result = 1;
  return result;
}

int GetOperation(char** ch, char* buff) {
  int err = 0;
  if (IsOperation(**ch)) {
    if (**ch != 'm') {
      *(buff++) = *(*ch)++;
      *buff = '\0';
    } else if (*(++(*ch)) == 'o' && *(++(*ch)) == 'd') {
      *(buff++) = 'm';
      *(buff++) = 'o';
      *(buff++) = 'd';
      *buff = '\0';
      ++(*ch);
    } else {
      err = OPERATION_ERROR;
    }
  }
  return err;
}

int GetPriority(char* data) {
  int result = 0;
  if (!CheckFunction(data))
    result = 5;
  else if (data[0] == '^')
    result = 4;
  else if (data[0] == '*' || data[0] == '/' || !strcmp("mod", data))
    result = 3;
  else if (data[0] == '+' || data[0] == '-')
    result = 1;
  else if (data[0] == '(' || data[0] == ')')
    result = 0;
  return result;
}

int Parser(char* data, Stack* stack) {
  int err = CheckParenthesis(data);
  if (err != 0) return err;

  StackInit(stack);
  while (*data != '\0' && err == 0) {
    if (IsDigit(*data)) {
      stack->data[++stack->pos].val = GetNumber(&data);
      stack->data[stack->pos].type = 0;
    } else if (IsFunc(*data)) {
      err = GetFunc(&data, stack->data[++stack->pos].oper);
      stack->data[stack->pos].prior = GetPriority(stack->data[stack->pos].oper);
      stack->data[stack->pos].type = 2;

    } else if (IsOperation(*data)) {
      err = GetOperation(&data, stack->data[++stack->pos].oper);
      stack->data[stack->pos].type = 1;
      stack->data[stack->pos].prior = GetPriority(stack->data[stack->pos].oper);
    } else if (*data == ' ') {
      ++data;
      // this one for x
      //        } else if (*data == 'x') {
      //            stack->data[++stack->pos].type = 3;
    } else {
      err = PARSE_ERROR;
    }
  }
  if (err != 0) StackFree(stack);
  return err;
}

// if need to calculate with x in C
// int AddXToString(char* x, char* data, int qua, char* result) {
//     int err = 0;
//     size_t data_len = strlen(data);
//     if (strlen(x) * qua + data_len > 256) {
//         err = LENGTH_ERROR;
//     } else {
////        char* ptr_end = data + data_len;
////        char* ptr_x = NULL;
//        int counter = 0;
//        while (*data != '\0') {
//            ++counter;
//            if (*data == 'x') {
//               // printf("data_len - counter = %d, counter = %d", data_len
//               -counter, counter);
//                memmove(result, x, strlen(x));
//                result += strlen(x) - 1;
//            } else {
//                *result = *data;
//            }
//          //  printf("%c", *data);
//            ++data;
//            ++result;
//        }
//    }
//    return err;
//}
