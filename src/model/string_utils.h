//
//  string_utils.h
//  test
//
//  Created by mcbk on 16.01.2023.
//

#ifndef string_utils_h
#define string_utils_h

#include <stdio.h>
#include <string.h>

#include "stack.h"
// returns:
// 0 - OK
// 1 - operation error
// 2 - func error
// 3 - parenthesis error
// 4 - calculation error

int CheckFunction(char* data);
int CheckParenthesis(char* data);
int GetFunc(char** ch, char* buff);
int IsFunc(char ch);
int IsDigit(char ch);
int IsOperation(char ch);
int GetOperation(char** ch, char* buff);
double GetNumber(char** ch);

int GetPriority(char* data);
int Parser(char* data, Stack* stack);
int CheckForX(char* data);
int AddXToString(char* x, char* data, int qua, char* result);
#endif /* string_utils_h */
