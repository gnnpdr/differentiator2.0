#ifndef _DIFF_H_
#define _DIFF_H_

#include "read_math_expression.h"

static const double SEC = 2; 

Node* diff_node (Node *const node, Errors *const error);
Node* copy_node (Node *const node, Errors *const error);

#endif //_DIFF_H_