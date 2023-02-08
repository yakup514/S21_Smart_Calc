//
//  calc_core.h
//  test
//
//  Created by mcbk on 19.01.2023.
//

#ifndef calc_core_h
#define calc_core_h
#ifdef __cplusplus
extern "C" {
#endif
#include <math.h>
#include <stdio.h>

#include "stack.h"
#include "string_utils.h"

void MakePolish(Stack* stack, Stack* result);
double Evaluate(double val1, double val2, char* operation);
int CalculatePolish(Stack* stack, double* result);
int Calculate(char* data, double* result);
#ifdef __cplusplus
}
#endif
#endif /* calc_core_h */
