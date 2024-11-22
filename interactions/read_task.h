#ifndef _INTERACTIONS_H_
#define _INTERACTIONS_H_

#include "..\draw\draw_tree.h"
#include "..\tree_lib\get_database.h"  //попытка была, но ничего не вышло, пишет, что нераспознаны заголовки в мэйне
#include "..\stack_lib\stk.h"

static const char ARITHM_OPEN_BRACE  = '(';
static const char ARITHM_CLOSE_BRACE = ')';
static const int ERROR_VALUE = -13;

/*static const char* EMPTY_STR = "";

static const size_t MAX_STRS_AMT  =  50;

static const size_t MAX_FILE_SIZE = 3000;

struct Input 
{
    char* name;
    char* text;
    size_t size;

    //char* strs[MAX_STRS_AMT];
    size_t node_amt;
};

Errors input_ctor (Input *const base_text);
Errors get_database_name(Input *const base_text, char **const argv);
Errors get_database_text (Input *const base_text);*/
void handle_task (Input* base_text, Tree *const the_tree);
void arithm_read_tree_node (Node* node, char** start_text, Tree* the_tree);

//void input_dtor(Input* base_text);

#endif //_INTERACTIONS_H_