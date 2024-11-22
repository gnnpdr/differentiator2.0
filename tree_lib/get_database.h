#ifndef _BASE_H_
#define _BASE_H_ 

#include "..\draw\draw_tree.h"

#include <sys/stat.h>

static const char QUOT_MARK   = '"';
static const char OPEN_BRACE  = '{';
static const char CLOSE_BRACE = '}';

static const char* EMPTY_STR = "";

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
Errors get_database_text (Input *const base_text);
//void handle_text (Input* base_text, Tree *const the_tree);;
void input_dtor(Input* base_text);

#endif //_BASE_H_