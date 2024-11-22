#include <stdio.h>

#include "diff.h"

static Node* copy_node (Node *const node);
static Node* diff_op (Node *const node);

static Node* add_sub_diff (Node *const node);
static Node* mul_diff(Node *const node);
static Node* div_diff(Node *const node);
static Node* pow_diff(Node *const node);
static Node* exp_func_diff(Node *const node);
static Node* compl_func_diff(Node *const node);

Node* diff_node (Node *const node)
{
    assert(node);

    if (node->type == VAR)
    {
        Node* new_node = make_node(NUM, 1, nullptr, nullptr);
        return new_node;
    }

    if (node->type == NUM)
    {
        Node* new_node = make_node(NUM, 0, nullptr, nullptr);
        return new_node;
    }

    if (node->type == OP)
    {
        Node* new_node = diff_op(node);
        return new_node;
    }
    
    return nullptr;
}

Node* copy_node (Node *const node)  //поменять на copy_branch
{
    assert(node);

    Node* left_node = nullptr;
    Node* right_node = nullptr;
        
    printf("-------------CURRENT NODE----------------------------------\n");
    printf("address %p\n", node);
    printf("type %lld\n", node->type);
    printf("value %lg\n", node->value);
    printf("---------------------------------------------------\n");

    
    if (node->Left)
    {
        printf("MAKE LEFT\n");
        left_node = copy_node(node->Left);
        printf("-------------LEFT NODE----------------------------------\n");
        printf("address %p\n", left_node);
        printf("type %lld\n", left_node->type);
        printf("value %lg\n", left_node->value);
        printf("---------------------------------------------------\n");
    }
    
    if (node->Right)
    {
        printf("MAKE RIGHT\n");
        right_node = copy_node(node->Right);
        printf("-------------RIGHT NODE----------------------------------\n");
        printf("address %p\n", right_node);
        printf("type %lld\n", right_node->type);
        printf("value %lg\n", right_node->value);
        printf("---------------------------------------------------\n");
    }
    
    Node* new_node = make_node(node->type, node->value, left_node, right_node);

    return new_node;
}

Node* diff_op (Node *const node)
{
    assert(node);
    //можно сделать это кодогенерацией

    Node* new_node = nullptr;

    if (node->value == ADD || node->value == SUB)
        new_node = add_sub_diff (node);

    else if (node->value == MUL)
        new_node = mul_diff(node);

    else if (node->value == DIV)
        new_node = div_diff(node);

    else if (node->value == POW)
    {
        if (node->Left->type == VAR && node->Right->type == NUM)
           new_node = pow_diff(node);
        
        else if (node->Right->type == NUM && node->Right->type == VAR) //для этого случая надо сделать отдельный вывод. Если натуральный или десятичный выписать его по-другому
            new_node = exp_func_diff(node);
        
        else if (node->Left->type != NUM && node->Right->type != NUM)
            new_node = compl_func_diff(node);
    }

    return new_node;
}

Node* add_sub_diff (Node *const node)
{
    Node* left_node = diff_node(node->Left);
    Node* right_node = diff_node(node->Right);

    Node* new_node = make_node(node->type, node->value, left_node, right_node);

    return new_node;
}

Node* mul_diff(Node *const node)
{
    Node* ll_node = diff_node(node->Left);
    Node* lr_node = copy_node(node->Right);

    Node* rl_node = copy_node(node->Left);
    Node* rr_node = diff_node(node->Right);

    Node* left_node = make_node(OP, MUL, ll_node, lr_node);
    Node* right_node = make_node(OP, MUL, rl_node, rr_node);

    Node* new_node = make_node(OP, ADD, left_node, right_node);
    
    return new_node;
}

Node* div_diff(Node *const node)
{
    Node* ll_node = diff_node(node->Left);
    Node* lr_node = copy_node(node->Right);

    Node* rl_node = copy_node(node->Left);
    Node* rr_node = diff_node(node->Right);

    Node* left_node = make_node(OP, MUL, ll_node, lr_node);
    Node* right_node = make_node(OP, MUL, rl_node, rr_node);

    Node* num_node = make_node(OP, SUB, left_node, right_node);

    Node* sec_pow_node = make_node(NUM, SEC, nullptr, nullptr);
    Node* denom_node = make_node(OP, POW, node->Right, sec_pow_node);

    Node* new_node = make_node(OP, DIV, num_node, denom_node);

    return new_node;
}

Node* pow_diff(Node *const node)
{
    Node* pow_node = make_node(NUM, node->Right->value - 1, nullptr, nullptr);
    Node* var_node = copy_node(node->Left);

    Node* new_node = make_node(OP, POW, var_node, pow_node);

    return new_node;
}

Node* exp_func_diff(Node *const node)
{
    Node* exp_node = copy_node(node);

    Node* llog_node = make_node(NUM, EXP, nullptr, nullptr);
    Node* rlog_node = make_node(NUM, node->Right->value, nullptr, nullptr);

    Node* log_node = make_node(OP, LOG, llog_node, rlog_node);

    Node* new_node = make_node(OP, MUL, exp_node, log_node);

    return new_node;
}

Node* compl_func_diff(Node *const node)
{
    Node* left_node = copy_node(node);

    Node* llog_node = make_node(NUM, EXP, nullptr, nullptr);
    Node* rlog_node = copy_node(node->Left);

    Node* rr_node = make_node(OP, LOG, llog_node, rlog_node);
    Node* lr_node = copy_node(node->Right);

    Node* temp_node = make_node(OP, MUL, lr_node, rr_node);
    Node* right_node = diff_node(temp_node);

    Node* new_node = make_node(OP, MUL, left_node, right_node);

    return new_node;
}