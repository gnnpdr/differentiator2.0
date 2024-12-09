#ifndef _INTERACTIONS_H_
#define _INTERACTIONS_H_

#include "..\draw\draw_tree.h"
#include "..\tree_lib\get_database.h"
#include "..\stack_lib\stk.h"

static const char ARITHM_OPEN_BRACE  = '(';
static const char ARITHM_CLOSE_BRACE = ')';
//static const int ERROR_VALUE = -13;

static const double EXP = 2.72;

void text_to_tree_convert (Input *const base_text, Tree *const the_tree, Errors *const error);
void arithm_read_tree_node (Node *const node, char **const start_text, Tree *const the_tree, Errors *const error);
int get_var_ind(char* str, bool* is_var);
int get_op_ind(char* str);
#endif //_INTERACTIONS_H_