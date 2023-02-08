//
//  stack.h
//  test2
//
//  Created by mcbk on 16.01.2023.
//

#ifndef stack_h
#define stack_h
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 255
#define OPERATION_ERROR 1
#define FUNCTION_ERROR 2
#define PARENTHESIS_ERROR 3
#define CALC_ERROR 4
#define PARSE_ERROR 5
#define LENGTH_ERROR 6

// oper - keeps operation (*,/,+,-,sin ...
// prior - priority of operation
// val - value
// err - if you have node err
// type = 0 if contains val
//        1 if contains operation
//        2 if contains function
//        3 if contains x
//
typedef struct {
  int type;
  char oper[5];
  int prior;
  double val;
  int err;
} Node;
// size - max size
// pos - current last Node position
typedef struct {
  Node* data;
  int size;
  int pos;
} Stack;

void StackInit(Stack* stack);
void StackFree(Stack* stack);
void StackResize(Stack* stack);
Node StackPeek(const Stack* stack);
Node StackPop(Stack* stack);
void StackPrint(const Stack* stack);
void StackPrintLine(const Stack* stack);
void MakeUnary(Stack* stack);
int CheckStack(Stack* stack);

#endif /* stack_h */
