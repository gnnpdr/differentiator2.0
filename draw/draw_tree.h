#ifndef _DRAW_H_
#define _DRAW_H_

#include "..\structure\tree_structure.h"

#include <string.h>

static const size_t INPUT_FILE_SIZE = 10000;

static const size_t MAX_STR_LEN = 100;

//void print(const Node* const node);
Errors graph_dump(Node* const node, Node* const definite_node);

#endif //_DRAW_H_