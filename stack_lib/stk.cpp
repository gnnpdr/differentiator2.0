#include <stdio.h>

#include "stk.h"

static void change_capacity(Stack *const stk, Errors *const error);

//---------------------CTOR-DTOR-----------------------------------------------------------------------

void stk_ctor(Stack *const stk, Errors *const error)
{
    assert(error);
    assert(stk);

    #ifdef DEBUG
    stk->origin_file = file;
    stk->origin_str = code_str;
    stk->origin_func = func;
    #endif

    stk->size = 0;
    stk->capacity = START_STK_SIZE;

    size_t capacity = stk->capacity;

    stack_element_t* data = (stack_element_t*)calloc(capacity, sizeof(stack_element_t));
    ALLOCATION_CHECK(data)
    
    for (size_t i = 0; i < capacity; i++)
        data[i] = POISON;

    stk->data = data;

    #ifdef DEBUG
    data[0] = left_canary_value;
    data[capacity LEFT_CANARY_ADD] = right_canary_value;
    #endif

    //CHECK_STK(stk, err)
}

void stk_dtor(Stack *const stk)
{
    free(stk->data);
}

//------------FUNCTIONS-------------------------------------------------------------------------------------------------


void stk_push(Stack *const stk, stack_element_t element, Errors *const error)
{
    size_t size     = stk->size;
    size_t capacity = stk->capacity;

    if (size >= capacity)
    {
        change_capacity(stk, error);
        CHECK
    }
    
    stack_element_t* data = stk->data;

    data[size] = element;

    size++;
    stk->size = size;

    printf("--------------------------------\nSTACK PUSH\n");
    printf("\n\n");
    for (size_t i = 0; i < stk->capacity; i++)
            printf("%d ", stk->data[i]);
    printf("\n");

    //stk->data = data;   //может быть, это не нужно
}


void change_capacity(Stack *const stk, Errors *const error)
{
    size_t size = stk->size;
    size_t capacity = stk->capacity;
    stack_element_t* data = stk->data;

    size_t new_capacity = 0;

    if (size >= capacity)
        new_capacity = capacity * DELTA;
    else
        new_capacity = capacity / DELTA;

    data = (stack_element_t*)realloc(data, new_capacity * sizeof(stack_element_t));
    ALLOCATION_CHECK(data)
    
    for (size_t i = 0; i <= capacity; i++)
        data[capacity + i] = POISON;

    stk->capacity = new_capacity;
    stk->data = data; 

    /*for (int i = 0; i < new_capacity; i++)
        printf("%d ", data[i]);

    printf("\n");*/
}

void stk_pop(Stack *const stk, stack_element_t* elem, Errors *const error)
{
    size_t size = stk->size;
    size_t capacity = stk->capacity;

    if (size == (capacity / DOUBLE_DELTA) && size > MIN_STK_SIZE)
    {
        change_capacity(stk, error);
        CHECK
    }

    stack_element_t* data = stk->data;
    
    size--; 
    *elem = data[size];
    data[size] = POISON;
    
    stk->size = size;
    stk->data = data;

    return;

    printf("--------------------------------\nSTACK POP\n");
    printf("\n\n");
    for (size_t i = 0; i < stk->capacity; i++)
            printf("%d ", stk->data[i]);
    printf("\n");

    //CHECK_STK(stk, err)
    //return;
}