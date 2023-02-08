//
//  stack.c
//  test2
//
//  Created by mcbk on 16.01.2023.
//

#include "stack.h"

void StackInit(Stack* stack) {
  stack->data = calloc(STACK_SIZE, sizeof(Node));
  stack->pos = -1;
  stack->size = STACK_SIZE;
}
void StackFree(Stack* stack) {
  free(stack->data);
  stack->pos = -1;
  stack->size = 0;
}

// for future development
// void StackResize(Stack* stack) {
//     if (stack) {
//         stack->size += STACK_SIZE;
//         stack->data = realloc(stack->data, stack->size * sizeof(Node));
//     }
// }

Node StackPeek(const Stack* stack) {
  Node node;
  if (stack->pos >= 0)
    node = stack->data[stack->pos];
  else
    node.err = 1;
  return node;
}
Node StackPop(Stack* stack) {
  Node node;
  if (stack->pos >= 0)
    node = stack->data[stack->pos--];
  else
    node.err = 1;
  return node;
}

// printing functions
// void StackPrint(const Stack* stack) {
//     for (int i = 0; i <= stack->pos; ++i) {
//         if (stack->data[i].type == 0)
//             printf("val = %lf\n", stack->data[i].val);
//         else if (stack->data[i].type == 1)
//             printf("oper = %s\n", stack->data[i].oper);
//         else if (stack->data[i].type == 2)
//             printf("func = %s\n", stack->data[i].oper);
//         else if (stack->data[i].type == 3)
//             printf("val = x\n");
//
//     }
// }
//
// void StackPrintLine(const Stack* stack) {
//     for (int i = 0; i <= stack->pos; ++i) {
//         if (stack->data[i].type == 0)
//             printf("%lf ", stack->data[i].val);
//         else if (stack->data[i].type == 1)
//             printf("%s ", stack->data[i].oper);
//         else if (stack->data[i].type == 2)
//             printf("%s ", stack->data[i].oper);
//     }
//     printf("\n");
// }

void MakeUnary(Stack* stack) {
  for (int i = 0; i <= stack->pos; ++i) {
    if (stack->data[i].type == 1) {
      if (stack->data[i].oper[0] == '+' &&
          (i == 0 || stack->data[i - 1].oper[0] == '(')) {
        stack->data[i].oper[0] = 'u';
        stack->data[i].oper[1] = '+';
        stack->data[i].oper[2] = '\0';
        stack->data[i].prior = 5;
      } else if (stack->data[i].oper[0] == '-' &&
                 (i == 0 || stack->data[i - 1].oper[0] == '(')) {
        stack->data[i].oper[0] = 'u';
        stack->data[i].oper[1] = '-';
        stack->data[i].oper[2] = '\0';
        stack->data[i].prior = 5;
      }
    }
  }
}

int CheckStack(Stack* stack) {
  int err = 0;
  if (StackPeek(stack).type != 0 && StackPeek(stack).oper[0] != ')')
    err = PARSE_ERROR;

  for (int i = 0; i <= stack->pos && err == 0; ++i) {
    if (i == 0 && stack->data[i].type == 1 && stack->data[i].oper[0] != 'u' &&
        stack->data[i].oper[0] != '(') {
      err = PARSE_ERROR;
    } else if (i > 0 && stack->data[i].type == 1 &&
               stack->data[i - 1].type == 1) {
      if (stack->data[i - 1].oper[0] == '(' &&
          (stack->data[i].oper[0] != '(' && stack->data[i].oper[0] != 'u')) {
        err = PARSE_ERROR;
      } else if (stack->data[i - 1].oper[0] != '(' &&
                 stack->data[i - 1].oper[0] != ')' &&
                 stack->data[i].oper[0] != '(') {
        err = PARSE_ERROR;
      } else if (stack->data[i - 1].oper[0] == ')' &&
                 stack->data[i].oper[0] == '(') {
        err = PARSE_ERROR;
      }
    } else if (i > 0 && stack->data[i - 1].type == 2 &&
               ((stack->data[i].type == 1 && stack->data[i].oper[0] != '(') ||
                stack->data[i].type == 0 || stack->data[i].type == 3)) {
      err = PARSE_ERROR;
    } else if (i > 0 && stack->data[i - 1].type == 1 &&
               stack->data[i].type == 2 && stack->data[i - 1].oper[0] == ')') {
      err = PARSE_ERROR;
    }
  }
  if (err) StackFree(stack);

  return err;
}
