#ifndef _CTOR_H_
#define _CTOR_H_

#include <stdlib.h>
#include <string.h>

#include "..\draw\draw_tree.h"
//#include "..\tree_lib\get_database.h"

typedef  int stack_element_t;
static const stack_element_t POISON = -13;

static const size_t DELTA          = 2;
static const size_t DOUBLE_DELTA   = 4;
static const size_t START_STK_SIZE = 6;

static const size_t MIN_STK_SIZE = 3;

struct Stack
{
    #ifdef DEBUG
    unsigned long long left_canary;
    const char* origin_file;
    int origin_str;
    const char* origin_func;
    unsigned long long hash;
    unsigned long long right_canary;
    #endif

    stack_element_t* data;
    size_t size;
    size_t capacity = START_STK_SIZE;
};

Errors stk_ctor(Stack *const stk);

Errors stk_push(Stack *const stk, stack_element_t element);
Errors stk_pop(Stack *const stk, stack_element_t* elem);

void stk_dtor(Stack *const stk);

#endif //_CTOR_H_