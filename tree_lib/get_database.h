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
    
    size_t node_amt;
};

void input_ctor (Input *const base_text, Errors *const error);
void get_database_name(Input *const base_text, char **const argv, Errors *const error);
void get_database_text (Input *const base_text, Errors *const error);
void input_dtor(Input* base_text);

#endif //_BASE_H_