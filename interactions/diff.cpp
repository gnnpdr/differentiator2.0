#include <stdio.h>

#include "diff.h"

//здесь будет куча проверок, так как создаются новые узлы

static Node* diff_op (Node *const node, Errors *const error);

static Node* add_sub_diff (Node *const node, Errors *const error);
static Node* mul_diff(Node *const node, Errors *const error);
static Node* div_diff(Node *const node, Errors *const error);
static Node* pow_diff(Node *const node, Errors *const error);
static Node* exp_func_diff(Node *const node, Errors *const error);
static Node* compl_func_diff(Node *const node, Errors *const error);
static Node* log_diff(Node *const node, Errors *const error);
static Node* sin_diff(Node *const node, Errors *const error);
static Node* cos_diff(Node *const node, Errors *const error);
static Node* tg_diff(Node *const node, Errors *const error);

Node* diff_node (Node *const node, Errors *const error)
{
    assert(node);
    assert(error);

    if (node->type == VAR)
    {
        Node* new_node = make_node(NUM, 1, nullptr, nullptr, error);
        return new_node;
    }

    if (node->type == NUM)
    {
        Node* new_node = make_node(NUM, 0, nullptr, nullptr, error);
        return new_node;
    }

    if (node->type == OP)
    {
        Node* new_node = diff_op(node, error);
        return new_node;
    }
    
    return nullptr;
}

Node* copy_node (Node *const node, Errors *const error)  //поменять на copy_branch
{
    assert(node);
    assert(error);

    Node* left_node = nullptr;
    Node* right_node = nullptr;

    
    if (node->Left)
        left_node = copy_node(node->Left, error);
    
    if (node->Right)
        right_node = copy_node(node->Right, error);
    
    Node* new_node = make_node(node->type, node->value, left_node, right_node, error);

    return new_node;
}

Node* diff_op (Node *const node, Errors *const error)
{
    assert(node);
    assert(error);
    //можно сделать это кодогенерацией

    Node* new_node = nullptr;

    if (node->value == ADD || node->value == SUB)
        new_node = add_sub_diff (node, error);

    else if (node->value == MUL)
        new_node = mul_diff(node, error);

    else if (node->value == DIV)
        new_node = div_diff(node, error);

    else if (node->value == LOG)
        new_node = log_diff(node, error);

    else if (node->value == SIN)
        new_node = sin_diff(node, error);

    else if (node->value == COS)
        new_node = cos_diff(node, error);

    else if (node->value == TG)
        new_node = tg_diff(node, error);

    else if (node->value == POW)
    {
        if (node->Left->type == VAR && node->Right->type == NUM)
            new_node = pow_diff(node, error);
           
        else if (node->Left->type == NUM && node->Right->type == VAR) //для этого случая надо сделать отдельный вывод. Если натуральный или десятичный выписать его по-другому
            new_node = exp_func_diff(node, error);
        
        else if (node->Left->type != NUM && node->Right->type != NUM)
            new_node = compl_func_diff(node, error);
    }

    return new_node;
}

Node* add_sub_diff (Node *const node, Errors *const error)
{
    Node* left_node = diff_node(node->Left, error);
    
    Node* right_node = diff_node(node->Right, error);

    Node* new_node = make_node(node->type, node->value, left_node, right_node, error);

    return new_node;
}

Node* mul_diff(Node *const node, Errors *const error)
{
    Node* ll_node = diff_node(node->Left, error);
    Node* lr_node = copy_node(node->Right, error);

    Node* rl_node = copy_node(node->Left, error);
    Node* rr_node = diff_node(node->Right, error);

    Node* left_node = make_node(OP, MUL, ll_node, lr_node, error);
    Node* right_node = make_node(OP, MUL, rl_node, rr_node, error);

    Node* new_node = make_node(OP, ADD, left_node, right_node, error);
    
    return new_node;
}

Node* div_diff(Node *const node, Errors *const error)
{
    Node* ll_node = diff_node(node->Left, error);
    Node* lr_node = copy_node(node->Right, error);

    Node* rl_node = copy_node(node->Left, error);
    Node* rr_node = diff_node(node->Right, error);

    Node* left_node = make_node(OP, MUL, ll_node, lr_node, error);
    Node* right_node = make_node(OP, MUL, rl_node, rr_node, error);

    Node* num_node = make_node(OP, SUB, left_node, right_node, error);

    Node* sec_pow_node = make_node(NUM, SEC, nullptr, nullptr, error);
    Node* denom_node = make_node(OP, POW, node->Right, sec_pow_node, error);

    Node* new_node = make_node(OP, DIV, num_node, denom_node, error);

    return new_node;
}

Node* log_diff(Node *const node, Errors *const error)
{
    assert(node);
    assert(error);

    Node* num_node = diff_node(node->Right, error);

    Node* exp_node = make_node(NUM, EXP, nullptr, nullptr, error);

    Node* log_node = make_node(OP, LOG, exp_node, node->Left, error);

    Node* denom_node = make_node(OP, MUL, node->Right, log_node, error);

    Node* new_node = make_node(OP, DIV, num_node, denom_node, error);

    //Node* final_node = make_node(OP, MUL, diff_node(num_node, error), new_node, error); //проблема

    return new_node;
}

Node* sin_diff(Node *const node, Errors *const error)
{
    Node* arg = copy_node(node->Right, error);  //пусть аргументы тригонометрии будут справа

    Node* trig = make_node(OP, COS, nullptr, arg, error);
    Node* compl_case = diff_node(arg, error);
    Node* new_node = make_node(OP, MUL, compl_case, trig, error);

    return new_node;
}

Node* cos_diff(Node *const node, Errors *const error)
{
    Node* arg = copy_node(node->Right, error);  //пусть аргументы тригонометрии будут справа
    
    Node* minus = make_node(NUM, -1, nullptr, nullptr, error);
    Node* sin = make_node(OP, SIN, nullptr, arg, error);

    Node* trig = make_node(OP, MUL, minus, sin, error);

    Node* compl_case = diff_node(arg, error);
    Node* new_node = make_node(OP, MUL, compl_case, trig, error);

    return new_node;
}

Node* tg_diff(Node *const node, Errors *const error)
{
    Node* arg = copy_node(node->Right, error);

    Node* two = make_node(NUM, 2, nullptr, nullptr, error); 
    Node* double_arg = make_node(OP, POW, arg, two, error);

    Node* one = make_node(NUM, 1, nullptr, nullptr, error);  
    Node* denom_node = make_node(OP, ADD, one, double_arg, error);

    Node* num_node = copy_node(one, error);

    Node* trig = make_node(OP, DIV, num_node, denom_node, error);

    Node* compl_case = diff_node(arg, error);
    Node* new_node = make_node(OP, MUL, compl_case, trig, error);

    return new_node;
}

Node* pow_diff(Node *const node, Errors *const error)
{
    Node* pow_num_node = make_node(NUM, node->Right->value - 1, nullptr, nullptr, error);
    Node* var_node = copy_node(node->Left, error);

    Node* num_node = copy_node(node->Right, error);
    Node* pow_node = make_node(OP, POW, var_node, pow_num_node, error);

    Node* new_node = make_node(OP, MUL, num_node, pow_node, error);

    return new_node;
}

Node* exp_func_diff(Node *const node, Errors *const error)
{
    Node* exp_node = copy_node(node, error);

    Node* llog_node = make_node(NUM, EXP, nullptr, nullptr, error);
    Node* rlog_node = copy_node(node->Left, error);

    Node* log_node = make_node(OP, LOG, llog_node, rlog_node, error);

    Node* new_node = make_node(OP, MUL, exp_node, log_node, error);

    return new_node;
}

Node* compl_func_diff(Node *const node, Errors *const error)
{
    Node* left_node = copy_node(node, error);

    Node* exp_node = make_node(NUM, EXP, nullptr, nullptr, error);

    Node* rpow_node = make_node(OP, LOG, exp_node, node->Left, error); 
    Node* pow_node = make_node(OP, MUL, node->Right, rpow_node, error);

    Node* right_node = mul_diff(pow_node, error);
    
    Node* new_node = make_node(OP, MUL, left_node, right_node, error);

    return new_node;
}