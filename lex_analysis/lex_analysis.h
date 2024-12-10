#ifndef _LEX_H_
#define _LEX_H_

#include "..\draw\draw_tree.h"
#include "..\tree_lib\get_database.h"
#include "..\stack_lib\stk.h"

static const size_t TOKEN_AMT = 10;
static const size_t ID_AMT = 10;
static const size_t ERROR_VALUE_SIZE_T = 888;
static const int ERROR_VALUE = -13;

struct Token
{
    Type type;
    int value;
};

#include <ctype.h>
#include <math.h>

struct Id
{
    char* start_address;
    size_t len;
    //доп инфа
};

Token* tokens_ctor(Err_param *const error);
Id* id_ctor(Err_param *const error);

void tokens_dtor(Token *const tokens);
void ids_dtor(Id *const ids);

void lex_analysis(Token *const tokens, Id *const ids, Input *const base_text, Err_param* error);

#endif //_LEX_H_