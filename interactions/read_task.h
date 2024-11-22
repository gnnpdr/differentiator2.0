#ifndef _INTERACTIONS_H_
#define _INTERACTIONS_H_

#include "..\draw\draw_tree.h"
#include "..\tree_lib\get_database.h"
#include "..\stack_lib\stk.h"

static const char ARITHM_OPEN_BRACE  = '(';
static const char ARITHM_CLOSE_BRACE = ')';
static const int ERROR_VALUE = -13;

static const double EXP = 2.72;

void handle_task (Input* base_text, Tree *const the_tree);
void arithm_read_tree_node (Node* node, char** start_text, Tree* the_tree);

#endif //_INTERACTIONS_H_