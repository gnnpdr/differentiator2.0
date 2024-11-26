#include <stdio.h>

#include "write_math_expression.h"  //переименовать, переместить сюда что-то еще

static char* get_node (Node *const node, Stack *const stk, char *const str, Errors *const error);
static void get_op_node (Stack *const stk, Node *const node, char *const str, Errors *const error);
static void latex_printf_features (int op_num, bool need_brace, char *const str, char *const left, char *const right, Errors *const error);
static bool compare_op_priority(Stack *const stk);

static void convert_const (Node *const node, size_t *const new_change, Errors *const error);
static void calculations (Node *const node, size_t *const new_change);

void write_math_expression(Node *const root, Stack *const stk, Errors *const error)
{
    assert(root);
    assert(stk);
    assert(error);

    size_t new_change = 1;
    //size_t change = 1;

    while (new_change != 0)
    {
        printf("CALCULATIONS\n");
        calculations (root, &new_change);
        //change = new_change;
        graph_dump(root, root, error);
    }
        
    new_change = 1;
    while (new_change != 0) 
    {
        printf("CONSTS\n");
        convert_const (root, &new_change, error);
        //change = new_change;
        graph_dump(root, root, error);
    }
        
    printf("HERE!!!!!!!!!!!\n");
    char* str = (char*)calloc(MAX_STR_LEN, sizeof(char));
    ALLOCATION_CHECK(str)

    get_node(root, stk, str, error); //название вообще не очень. Надо другое придумать
    CHECK

    printf("THE STR %s\n", str);   //вывод можно сделать в файл, все дела. Главное - что строка получена

    free(str);
}

char* get_node (Node *const node, Stack *const stk, char *const str, Errors *const error)
{
    assert(node);
    assert(stk);
    assert(str);
    assert(error);
    
    int sprintf_res = 0;

    printf("-----------------------------------------------\n");
    printf("address %p\n", node);
    printf("type %lld\n", node->type);
    printf("value %lg\n", node->value);
    printf("---------------------------------------------------\n");

    if (node->type == OP)
    {
        printf("OP\n");
        get_op_node (stk, node, str, error);
        if (*error != ALL_RIGHT)
            return nullptr;
    }
    else if (node->type == NUM)
    {
        printf("NUM\n");
        snprintf(str, MAX_STR_LEN, "%lg", node->value);
        printf("str %s\n", str);
    }
    else if (node->type == VAR)
    {
        printf("VAR\n");
        /*strncpy(str, variables[(int)node->value]->name, MAX_STR_LEN);
        if (str == nullptr)
        {
            *error = CPY_ERROR;
            return str;
        }*/

        snprintf(str, MAX_STR_LEN, "%s", variables[(int)node->value]->name);
        printf("str %s\n", str);
    }

    return str;
}

void get_op_node (Stack *const stk, Node *const node, char *const str, Errors *const error)
{
    assert(stk);
    assert(node);
    assert(str);
    assert (error);

    int sprintf_res = 0;

    char* left = (char*)calloc(MAX_STR_LEN, sizeof(double));
    ALLOCATION_CHECK(left)

    char* right = (char*)calloc(MAX_STR_LEN, sizeof(double));
    ALLOCATION_CHECK(right)

    stk_push(stk, (stack_element_t)node->value, error);
    CHECK

    //printf("GO LEFT\n");
    if(node->Left)
    {
        get_node(node->Left, stk, left, error);
        CHECK
    }
        
    if(node->Right)
    {
        get_node(node->Right, stk, right, error);
        CHECK
    }
        
    //printf("LEFT %s\n", left);
    //printf("RIGHT %s\n", right);
    
    bool need_brace = compare_op_priority(stk);
    int op_num = 0;

    stk_pop(stk, &op_num, error);
    CHECK

    latex_printf_features (op_num, need_brace, str, left, right, error);
    
    free(left);
    free(right);
}

void latex_printf_features (int op_num, bool need_brace, char *const str, char *const left, char *const right, Errors *const error)
{
    assert(str);
    assert(left);
    assert(right);
    assert(error);

    int sprintf_res = 0;

    if (op_num == DIV)
    {
        sprintf_res = sprintf_s(str, MAX_STR_LEN, "\\dfrac{%s}{%s}", left, right);
        SPRINTF_CHECK

        //printf("STR %s\nLEFT STR %s RIGHT STR %s\n", str, left, right);
    }
    else if (op_num == POW)
    {
        sprintf_res = sprintf_s(str, MAX_STR_LEN, "(%s)%s%s", left, operations[op_num]->name, right);
        SPRINTF_CHECK

        //printf("STR %s\nLEFT STR %s RIGHT STR %s\n", str, left, right);
    }
    else
    {
        if (need_brace)
        {
            sprintf_res = sprintf_s(str, MAX_STR_LEN, "(%s%s%s)", left, operations[op_num]->name, right);
            SPRINTF_CHECK

            //printf("STR %s\nLEFT STR %s RIGHT STR %s\n", str, left, right);

        }
        else
        {
            sprintf_res = sprintf_s(str, MAX_STR_LEN, "%s%s%s", left, operations[op_num]->name, right);
            SPRINTF_CHECK

            //printf("STR %s\nLEFT STR %s RIGHT STR %s\n", str, left, right);
        }  
    }
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

void convert_const (Node *const node, size_t *const new_change, Errors *const error)
{
    *new_change = 0;
    assert(node);
    assert(error);

    //printf("CONSTS\n");
    printf("----------------\nNODE\n%d\n%lg\n------------------\n", node->type, node->value);

    if(node->Left)
    {
        printf("GO LEFT\n");
        convert_const (node->Left, new_change, error);
    }
        
    if(node->Right)
    {
        printf("GO RIGHT\n");
        convert_const (node->Right, new_change, error);
    }
        
    if (!node->Right && !node->Left)
        return;
    /*if (node->type == VAR)
    {
        //printf("RET VAR\n");
        return;
    }*/

    if (node->Left->type == NUM && node->Right->type == NUM)
    {
        if (node->value == ADD || node->value == SUB)
        {
            if (node->Left->value == 0 && node->Right->value != 0)
            {
                node->type = NUM;
                node->value = node->Right->value;

                node->Left = node_dtor(node->Left);
                node->Right = node_dtor(node->Right);
                (*new_change)++;
            }
            else if (node->Left->value != 0 && node->Right->value == 0)
            {
                node->type = NUM;
                node->value = node->Left->value;

                node->Left = node_dtor(node->Left);
                node->Right = node_dtor(node->Right);
                (*new_change)++;
            }
            else if (node->Left->value == 0 && node->Right->value == 0)
            {
                node->type = NUM;
                node->value = node->Left->value;

                node->Left = node_dtor(node->Left);
                node->Right = node_dtor(node->Right);
                (*new_change)++;
            }
        }
        else if (node->value == DIV)
        {
            if (node->Left->value == 0)
            {
                node->type = NUM;
                node->value = 0;
                
                node->Left = node_dtor(node->Left);
                node->Right = node_dtor(node->Right);
                (*new_change)++;
            }
            else if (node->Right->value == 1)
            {
                node->type = NUM;
                node->value = node->Left->value;
                
                node->Left = node_dtor(node->Left);
                node->Right = node_dtor(node->Right);
                (*new_change)++;
            }
            else if (node->Right->value == 0)
            {
                printf("MATH ERROR\n");
                *error = MATH_ERROR;
                return;
            }
        }
        else if (node->value == POW)
        {
            if (node->Right->value == 1)
            {
                node->type = NUM;
                node->value = node->Left->value;
                
                node->Left = node_dtor(node->Left);
                node->Right = node_dtor(node->Right);
                (*new_change)++;
            }
            else if (node->Left->value == 0)
            {
                node->type = NUM;
                node->value = 0;
                
                node->Left = node_dtor(node->Left);
                node->Right = node_dtor(node->Right);
                (*new_change)++;
            }
            else if (node->Right->value == 0)
            {
                node->type = NUM;
                node->value = 1;
                
                node->Left = node_dtor(node->Left);
                node->Right = node_dtor(node->Right);
                (*new_change)++;
            }
        }
        else
        {
            if (node->Right->value == 1)
            {
                node->type = NUM;
                node->value = 0;
                
                node->Left = node_dtor(node->Left);
                node->Right = node_dtor(node->Right);
                (*new_change)++;
            }
        }
    }

    if (node->value == MUL)
    {
        if (node->Left->value == 0 || node->Right->value == 0)
        {
            node->type = NUM;
            node->value = 0;
            node->Left = node_dtor(node->Left);
            node->Right = node_dtor(node->Right);
            (*new_change)++;
        }
        else if (node->Left->value != 1 && node->Right->value == 1)
        {
            node->type = NUM;
            node->value = node->Left->value;
            
            node->Left = node_dtor(node->Left);
            node->Right = node_dtor(node->Right);
            (*new_change)++;
        }
        else if (node->Left->value == 1 && node->Right->value != 1)
        {
            node->type = NUM;
            node->value = node->Right->value;
            
            node->Left = node_dtor(node->Left);
            node->Right = node_dtor(node->Right);
            (*new_change)++;
        } 
    }

    return;
}

void calculations (Node *const node, size_t *const new_change)
{
    *new_change = 0;
    assert(node);

    //printf("CALCULATIONS\n");
    printf("----------------\nNODE\n%d\n%lg\n------------------\n", node->type, node->value);

    if(node->Left)
    {
        //printf("GO LEFT\n");
        calculations (node->Left, new_change);
    }
        

    if(node->Right)
    {
        //printf("GO RIGHT\n");
        calculations (node->Right, new_change);
    }
        
    if (node->type == VAR)
    {
        //printf("RET VAR\n");
        return;
    }
        

    if (!node->Left || !node->Right)
    {
        //printf("RET LEAF\n");
        return;
    }
        
    //вот здесь точно нужна кодогенерация

    if (node->Left->type == NUM && node->Right->type == NUM)
    {
        //printf("NUMS\n");
        if (node->value == ADD)
        {
            node->type = NUM;
            node->value = node->Left->value + node->Right->value;

            node->Left = node_dtor(node->Left);
            node->Right = node_dtor(node->Right); 
            (*new_change)++;
        }
        else if (node->value == MUL)
        {
            node->type = NUM;
            node->value = node->Left->value * node->Right->value;
               
            node->Left = node_dtor(node->Left);
            node->Right = node_dtor(node->Right); 
            (*new_change)++;
        }
        else if (node->value == DIV)
        {
            node->type = NUM;
            node->value = node->Left->value / node->Right->value;
               
            node->Left = node_dtor(node->Left);
            node->Right = node_dtor(node->Right);
            (*new_change)++;
        }
        else if (node->value == SUB)
        {
            node->type = NUM;
            node->value = node->Left->value - node->Right->value;
               
            node->Left = node_dtor(node->Left);
            node->Right = node_dtor(node->Right);
            (*new_change)++;
        }
        else if (node->value == POW)
        {
            node->type = NUM;
            node->value = pow(node->Left->value, node->Right->value);
               
            node->Left = node_dtor(node->Left);
            node->Right = node_dtor(node->Right);
            (*new_change)++;
        }
        else
        {
            node->type = NUM;
            node->value = log(node->Right->value);
               
            node->Left = node_dtor(node->Left);
            node->Right = node_dtor(node->Right);
            (*new_change)++;
        }
    }

    //printf("RET END\n");
}