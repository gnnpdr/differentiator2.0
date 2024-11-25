#include <stdio.h>

#include "write_math_expression.h"  //переименовать, переместить сюда что-то еще

static char* get_node (Node *const node, Stack *const stk, char *const str, Errors *const error);
static bool compare_op_priority(Stack *const stk);

void write_math_expression(Node *const root, Stack *const stk, Errors *const error)
{
    assert(root);
    assert(stk);
    assert(error);

    char* str = (char*)calloc(MAX_STR_LEN, sizeof(char));
    ALLOCATION_CHECK(str)

    get_node(root, stk, str, error); //название вообще не очень. Надо другое придумать
    CHECK

    printf("THE STR %s\n", str);   //вывод можно сделать в файл, все дела. Главное - что строка получена

    free(str);
}

char* get_node (Node *const node, Stack *const stk, char *const str, Errors *const error)  //переименовать
{
    assert(node);
    assert(stk);
    assert(str);
    assert(error);
    
    int sprintf_res = 0;

    bool need_brace = false;

    /*printf("-----------------------------------------------\n");
    printf("address %p\n", node);
    printf("type %lld\n", node->type);
    printf("value %lg\n", node->value);
    printf("---------------------------------------------------\n");*/

    if (node->type == OP)
    {
        //printf("OP\n");
        
        char* left = (char*)calloc(MAX_STR_LEN, sizeof(double));
        if (left == nullptr)
        {       
            *error = ALLOCATION_ERROR;      
            return left;                         
        }

        char* right = (char*)calloc(MAX_STR_LEN, sizeof(double));
        if (right == nullptr)
        {       
            *error = ALLOCATION_ERROR;      
            return right;                         
        }

        stk_push(stk, (stack_element_t)node->value, error);
        if (*error != ALL_RIGHT)
            return nullptr;

        //printf("GO LEFT\n");
        if(node->Left)
        {
            get_node(node->Left, stk, left, error);
            if (*error != ALL_RIGHT)
                return nullptr;
        }
            
        if(node->Right)
        {
            get_node(node->Right, stk, right, error);
            if (*error != ALL_RIGHT)
                return nullptr;
        }
            
        //printf("LEFT %s\n", left);
        //printf("RIGHT %s\n", right);
        
        need_brace = compare_op_priority(stk);
        int op_num = 0;
        stk_pop(stk, &op_num, error);
        if (*error != ALL_RIGHT)
            return nullptr;

        if (op_num == DIV)  //отдельная функция
        {
            sprintf_res = sprintf_s(str, MAX_STR_LEN, "\\dfrac{%s}{%s})", left, right);
            if (sprintf_res == -1)
            {
                *error = SPRINTF_ERROR;
                return nullptr;
            }
        }
        else
        {
            if (need_brace)
            {
                sprintf_res = sprintf_s(str, MAX_STR_LEN, "(%s%s%s)", left, operations[op_num]->name, right);
                if (sprintf_res == -1)
                {
                    *error = SPRINTF_ERROR;
                    return nullptr;
                }
            }
            else
            {
                sprintf_res = sprintf_s(str, MAX_STR_LEN, "%s%s%s", left, operations[op_num]->name, right);
                if (sprintf_res == -1)
                {
                    *error = SPRINTF_ERROR;
                    return nullptr;
                }
            }
                
        }
        
        free(left);
        free(right);

    }
    else if (node->type == NUM)
    {
        //printf("NUM\n");
        snprintf(str, MAX_STR_LEN, "%lg", node->value);
        //printf("str %s\n", str);
    }
    else if (node->type == VAR)
    {
        //printf("VAR\n");
        strncpy(str, variables[(int)node->value]->name, MAX_STR_LEN);
        if (str == nullptr)
        {
            *error = CPY_ERROR;
            return str;
        }
        //snprintf(str, MAX_STR_LEN, "%c", (char)node->value);
        //printf("str %s\n", str);
    }

    return str;
}

bool compare_op_priority(Stack *const stk)
{
    assert(stk);

    size_t current_num = stk->size;

    bool need_brace = false;

    if (stk->data[current_num - 1] == MUL && stk->data[current_num] != MUL && stk->data[current_num] != DIV)
        need_brace = true;

    if (stk->data[current_num - 1] == POW)
        need_brace = true;
    
    return need_brace;
}