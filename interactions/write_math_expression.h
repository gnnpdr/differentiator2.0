#ifndef _WRITE_H_
#define _WRITE_H_

#include "read_math_expression.h"
#include <math.h>

//static const size_t START_CHANGE = 17;

void write_math_expression(Node *const root, Stack *const stk, Errors *const error);

#endif //_WRITE_H_