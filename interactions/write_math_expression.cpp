#include <stdio.h>

#include "write_math_expression.h"  //переименовать, переместить сюда что-то еще

static char* get_node (Node *const node, Stack *const stk, char *const str, Errors *const error);
static void get_op_node (Stack *const stk, Node *const node, char *const str, Errors *const error);
static void latex_printf_features (int op_num, bool need_brace, char *const str, char *const left, char *const right, Errors *const error);
static bool compare_op_priority(Stack *const stk);

static Node* convert_const (Node* node, size_t *const new_change, Errors *const error, Node* root);
static void calculations (Node *const node, size_t *const new_change);
static void make_ln(Node *const node, size_t *const new_change);

static Node* check_node_add (Node* node, Errors *const error, size_t *const new_change);
static Node* check_node_div (Node* node, Errors *const error, size_t *const new_change);
static Node* check_node_mul (Node* node, Errors *const error, size_t *const new_change);
static Node* check_node_pow (Node* node, Errors *const error, size_t *const new_change);

static void compare_branches(Node *const node1, Node *const node2, bool *const is_equal);
static bool compare_nodes (Node *const node1, Node *const node2);

void write_math_expression(Node* root, Stack *const stk, Errors *const error)
{
    assert(root);
    assert(stk);
    assert(error);

    size_t new_change = 1;

    printf("WRITE\n");

    while (new_change != 0)
    {
        printf("CHANGE\n");
        //graph_dump(root, root, error);

        make_ln(root, &new_change);
        //graph_dump(root, root, error);

        calculations (root, &new_change);
        //graph_dump(root, root, error);

        convert_const (root, &new_change, error, root);
        graph_dump(root, root, error);
    }
        
    char* str = (char*)calloc(MAX_STR_LEN, sizeof(char));
    ALLOCATION_CHECK(str)

    get_node(root, stk, str, error);
    CHECK

    printf("THE STR %s\n", str);

    free(str);
}

char* get_node (Node *const node, Stack *const stk, char *const str, Errors *const error)
{
    assert(node);
    assert(stk);
    assert(str);
    assert(error);
    
    int sprintf_res = 0;

    if (node->type == OP)
    {
        get_op_node (stk, node, str, error);
        if (*error != ALL_RIGHT)
            return nullptr;
    }
    else if (node->type == NUM)
        snprintf(str, MAX_STR_LEN, "%lg", node->value);

    else if (node->type == VAR)
        snprintf(str, MAX_STR_LEN, "%s", variables[(int)node->value]->name);

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
    }
    else if (op_num == POW)
    {
        sprintf_res = sprintf_s(str, MAX_STR_LEN, "(%s)%s%s", left, operations[op_num]->name, right);
        SPRINTF_CHECK
    }
    else
    {
        if (need_brace)
        {
            sprintf_res = sprintf_s(str, MAX_STR_LEN, "(%s%s%s)", left, operations[op_num]->name, right);
            SPRINTF_CHECK
        }
        else
        {
            sprintf_res = sprintf_s(str, MAX_STR_LEN, "%s%s%s", left, operations[op_num]->name, right);
            SPRINTF_CHECK
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

Node* convert_const (Node* node, size_t *const new_change, Errors *const error, Node* root)
{
    *new_change = 0;
    assert(node);
    assert(error);

    Node* old_node = node;

    //printf("CONSTS\n");
    //printf("----------------\nNODE\n%d\n%lg\n------------------\n", node->type, node->value);

    if(node->Left)
    {
        printf("GO LEFT\n");
        node->Left = convert_const (node->Left, new_change, error, root);
    }
        
    if(node->Right)
    {
        printf("GO RIGHT\n");
        node->Right = convert_const (node->Right, new_change, error, root);
    }
        
    if (!node->Right && !node->Left)
        return node;

    if (node->value == ADD || node->value == SUB)
    {
        node = check_node_add(node, error, new_change);
        
    }
    else if (node->value == DIV)
    {
        node = check_node_div(node, error, new_change);
    }
    else if (node->value == POW)
    {
        node = check_node_pow(node, error, new_change);
    }
    if (node->value == MUL)
    {
        node = check_node_mul(node, error, new_change);
    }

    if (node != old_node)
        graph_dump(root, node, error);

    return node;
}

Node* check_node_add (Node* node, Errors *const error, size_t *const new_change)
{
    assert(node);
    assert(error);

    //printf("NUMS ADD\n");
    if (node->Left->value == 0 && node->Left->type == NUM)
    {
        node->Left = node_dtor(node->Left);  //это нормально, нет проблем с *памятью*?
        node = node->Right;
        (*new_change)++;
    }
    else if (node->Right->value == 0 && node->Right->type == NUM)
    {
        node->Right = node_dtor(node->Left);
        node = node->Left;
        (*new_change)++;
        (*new_change)++;
    }
    else if (node->Left->value == 0 && node->Right->value == 0 && node->Right->type == NUM && node->Left->type == NUM)
    {
        node->type = NUM;
        node->value = 0;
        node->Left = node_dtor(node->Left);
        node->Right = node_dtor(node->Right);
        (*new_change)++;
    }
    return node;
}

Node* check_node_div (Node* node, Errors *const error, size_t *const new_change)
{
    assert(node);
    assert(error);

    printf("DIV\n");

    bool is_equal = true;
    compare_branches(node->Left, node->Right, &is_equal);
    //printf("is equal %d\n", is_equal);

    if (is_equal)
    {
        printf("TRY TO CHANGE\n");
        node->Left = node_dtor(node->Left);
        node->Right = node_dtor(node->Right);

        node->type = NUM;
        node->value = 1;

        (*new_change)++;
    }

    //graph_dump(node, node, error);

    if (node->Left->value == 0 && node->Left->type == NUM)
    {
        node->type = NUM;
        node->value = 0;
        
        node->Left = node_dtor(node->Left);
        node->Right = node_dtor(node->Right);
        (*new_change)++;
    }
    else if (node->Right->value == 1 && node->Right->type == NUM)
    {
        node->Right = node_dtor(node->Right);
        node = node->Left;
        
        (*new_change)++;
    }
    else if (node->Right->value == 0 && node->Right->type == NUM)
    {
        printf("MATH ERROR\n");
        *error = MATH_ERROR;
        return nullptr;
    }

    return node;
}

void compare_branches(Node *const node1, Node *const node2, bool *const is_equal)
{
    assert(node1);
    assert(node2);
    printf("TRY TO COMPARE\n");

    Errors error = ALL_RIGHT;

    //graph_dump(node1, node1, &error);
    //graph_dump(node2, node2, &error);

    if (node1->Left && node2->Left)
        compare_branches(node1->Left, node2->Left, is_equal);

    if (node1->Right && node2->Right)
        compare_branches(node1->Right, node2->Right, is_equal);

    //printf("is eq 1 %d\n", *is_equal);

    if (!(*is_equal))
        return;

    *is_equal = compare_nodes(node1, node2);
}

bool compare_nodes (Node *const node1, Node *const node2)
{
    assert(node1);
    assert(node2);

    printf("COPMARE NODES\n");

    bool is_equal = false;

    if (node1->type == node2->type && node1->value == node2->value)
        is_equal = true;
    
    return is_equal;
}

Node* check_node_mul (Node* node, Errors *const error, size_t *const new_change)
{
    //printf("MUL\n");
    //printf("left %lg\nright %lg\n", node->Left->value, node->Right->value);
    if ((node->Left->value == 0 && node->Left->type == NUM) || (node->Right->value == 0 && node->Right->type == NUM))
    {
        //printf("NULL\n");
        node->Left = node_dtor(node->Left);
        node->Right = node_dtor(node->Right);

        node->type = NUM;
        node->value = 0;
        
        (*new_change)++;
    }
    else if (node->Right->value == 1 && node->Right->type == NUM)
    {
        node->Right = node_dtor(node->Left);
        node = node->Left;

        (*new_change)++;
    }
    else if (node->Left->value == 1 && node->Left->type == NUM)
    {
        node->Left = node_dtor(node->Left);
        node = node->Right;

        (*new_change)++;
    } 
    else if (node->Right->type == OP && node->Right->value == DIV)
    {
        Node* new_num_mul = make_node(OP, MUL, node->Right->Left, node->Left, error);

        node->Left = node_dtor(node->Left);

        node->Right->Left = new_num_mul;

        node = node->Right;
    }
    else if (node->Left->type == OP && node->Left->value == DIV)
    {
        Node* new_num_mul = make_node(OP, MUL, node->Left->Left, node->Right, error);

        node->Right = node_dtor(node->Left);

        node->Left->Left = new_num_mul;

        node = node->Left;
    }

    return node;
}

Node* check_node_pow (Node* node, Errors *const error, size_t *const new_change)
{
    assert(node);
    assert(error);

    //printf("POW\n");
    if (node->Right->value == 1 && node->Right->type == NUM)
    {
        //printf("before node address %p\ntype %d\nvalue %lg\n", node, node->type, node->value);
        node->Right = node_dtor(node->Right);
        //graph_dump(node->Left, node->Left, error);
        
        node = node->Left;
        
        //printf("after node address %p\ntype %d\nvalue %lg\n", node, node->type, node->value);  //видно, что адрес и значения меняются
        (*new_change)++;
    }
    else if (node->Left->value == 0 && node->Left->type == NUM)
    {
        node->Right = node_dtor(node->Right);
        node = node->Left, error;
        (*new_change)++;
    }
    else if (node->Right->value == 0 && node->Right->type == NUM)
    {
        node->type = NUM;
        node->value = 1;
        
        node->Left = node_dtor(node->Left);
        node->Right = node_dtor(node->Right);
        (*new_change)++;
    }

    return node;
}

void make_ln(Node *const node, size_t *const new_change)
{
    assert(node);
    assert(new_change);

    if(node->Left)
    {
        make_ln (node->Left, new_change);
    }
        
    if(node->Right)
    {
        make_ln (node->Right, new_change);
    }
        
    if (!node->Left && !node->Right)
    {
        return;
    }

    if (node->type == OP && node->value == LOG && node->Left->value == EXP && node->Right->value == EXP)
    {
        node->Left = node_dtor(node->Left);
        node->Right = node_dtor(node->Right);

        node->type = NUM;
        node->value = 1;
    }

    return;
}

void calculations (Node *const node, size_t *const new_change)
{
    *new_change = 0;
    assert(node);

    //printf("CALCULATIONS\n");
    //printf("----------------\nNODE\n%d\n%lg\n------------------\n", node->type, node->value);

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