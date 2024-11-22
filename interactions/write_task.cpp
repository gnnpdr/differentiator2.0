#include <stdio.h>

#include "write_task.h"

static char* get_node_value (Node* node, Stack* stk, char* str);
static bool check_operations_value(Stack* stk);

void write_task(Tree* the_tree, Stack* stk)
{
    assert(the_tree);

    char* str = (char*)calloc(MAX_STR_LEN, sizeof(char));
    //printf("HERE\n");

    get_node_value(the_tree->top, stk, str); //название вообще не очень. Надо другое придумать
    printf("THE STR %s\n", str);   //вывод можно сделать в файл, все дела. Главное - что строка получена

    free(str);
}

char* get_node_value (Node* node, Stack* stk, char* str)
{
    assert(node);
    assert(stk);
    assert(str);

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
        char* right = (char*)calloc(MAX_STR_LEN, sizeof(double));

        stk_push(stk, (stack_element_t)node->value);

        //printf("GO LEFT\n");
        get_node_value(node->Left, stk, left);
        get_node_value(node->Right, stk, right);
        //printf("LEFT %s\n", left);
        //printf("RIGHT %s\n", right);
        
        need_brace = check_operations_value(stk);
        int op_num = 0;
        stk_pop(stk, &op_num);

        if (op_num == DIV)
            sprintf_s(str, MAX_STR_LEN, "\\dfrac{%s}{%s})", left, right);
        else
        {
            if (need_brace)
                sprintf_s(str, MAX_STR_LEN, "(%s%s%s)", left, operations[op_num]->name, right);
            else
                sprintf_s(str, MAX_STR_LEN, "%s%s%s", left, operations[op_num]->name, right);
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
        //snprintf(str, MAX_STR_LEN, "%c", (char)node->value);
        //printf("str %s\n", str);
    }

    return str;
}

bool check_operations_value(Stack* stk) //скорее всего, надо переименовать
{
    assert(stk);

    size_t current_num = stk->size;

    bool need_brace = false;

    if (stk->data[current_num - 1] == MUL && stk->data[current_num] != MUL && stk->data[current_num] != DIV)
        need_brace = true;
    
    return need_brace;
}