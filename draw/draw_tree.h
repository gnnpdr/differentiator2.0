#ifndef _DRAW_H_
#define _DRAW_H_

#include "..\structure\tree_structure.h"

#include <string.h>

static const char* OP_DEF = "operation";
static const char* NUM_DEF = "number";
static const char* VAR_DEF = "variable";

static const size_t INPUT_FILE_SIZE = 10000;

static const size_t MAX_STR_LEN = 100;

void graph_dump(Node* const node, Node* const definite_node, Errors *const error);

#endif //_DRAW_H_