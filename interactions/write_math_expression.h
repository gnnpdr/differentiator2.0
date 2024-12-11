#ifndef _WRITE_H_
#define _WRITE_H_

#include "latex_dump.h"
#include <math.h>

Node* write_math_expression(char *const text, Node* root, Stack *const stk, Err_param *const error);

#endif //_WRITE_H_