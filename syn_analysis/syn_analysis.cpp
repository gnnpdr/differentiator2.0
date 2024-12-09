#include <stdio.h>

#include "syn_analysis.h"

static bool op_match(Token token, const OperationParameters *const arr, size_t arr_size);

static Node* get_expression(Token *const tokens, size_t *const pointer, Err_param *const error);
static Node* get_t (Token *const tokens, size_t *const pointer, Err_param *const error);
static Node* get_brace (Token *const tokens, size_t* pointer, Err_param *const error);
static Node* get_unary(Token *const tokens, size_t *const pointer, Err_param *const error);
static Node* get_binary(Token *const tokens, size_t *const pointer, Err_param *const error);

static Node* get_num(Token *const token, size_t *const pointer, Err_param *const error);
static Node* get_id(Token *const token, size_t *const pointer, Err_param *const error);

Node* syn_analysis(Token *const tokens, Err_param *const error)
{
    assert(tokens);
    assert(error);

    size_t pointer = 0;

    Node* root = get_expression(tokens, &pointer, error);

    if (tokens[pointer].type != OP && tokens[pointer].value != END)
    {
        ERROR(SYN_ERROR)
        return nullptr;
    }

    pointer++;

    return root;
}

bool op_match(Token token, const OperationParameters *const arr, size_t arr_size)
{
    assert(arr);

    for (size_t ind = 0; ind < arr_size; ind++)
    {
        if (token.type == OP && token.value == arr[ind].num)
            return true;
    }

    return false;
}

Node* get_expression(Token *const tokens, size_t *const pointer, Err_param *const error)
{
    assert(tokens);
    assert(pointer);
    assert(error);

    Node* val = get_t(tokens, pointer, error);

    bool need_here = op_match(tokens[*pointer], first_pr, FIRST_PR_AMT);

    while (need_here)
    {
        size_t op = tokens[*pointer].value;
        (*pointer)++;

        Node* val2 = get_t(tokens, pointer, error);
        val = make_node(OP, op, val, val2, error);
        need_here = op_match(tokens[*pointer], first_pr, FIRST_PR_AMT);
    }

    return val;
}

Node* get_t (Token *const tokens, size_t *const pointer, Err_param *const error)
{
    Node* val = get_brace(tokens, pointer, error);

    bool need_here = op_match(tokens[*pointer], sec_pr, SEC_PR_AMT);

    while (need_here)
    {
        size_t op = tokens[*pointer].value;
        (*pointer)++;

        Node* val2 = get_brace(tokens, pointer, error);
        val = make_node(OP, op, val, val2, error);
        need_here = op_match(tokens[*pointer], first_pr, FIRST_PR_AMT);
    }

    return val;
}

Node* get_brace (Token *const tokens, size_t* pointer, Err_param *const error)
{
    assert(tokens);
    assert(pointer);
    assert(error);
    
    bool is_var = false;

    Node* val = nullptr;

    if (tokens[*pointer].type == OP)
    {
        bool unary  = op_match(tokens[*pointer], unary_op, UNARY_OP_AMT);
        bool binary = op_match(tokens[*pointer], binary_op, BINARY_OP_AMT);

        if (tokens[*pointer].value == O_BR)
        {
            (*pointer)++;

            val = get_expression(tokens, pointer, error);

            if (tokens[*pointer].type == OP && tokens[*pointer].value == C_BR)
            {
                ERROR(SYN_ERROR)
                return nullptr;
            }

            (*pointer)++;
        }
        else if (tokens[*pointer].value == FO_BR)  //для логарифма
        {
            (*pointer)++;

            val = get_expression(tokens, pointer, error);

            if (tokens[*pointer].type == OP && tokens[*pointer].value == FC_BR)
            {
                ERROR(SYN_ERROR)
                return nullptr;
            }

            (*pointer)++;
        }
        else if (binary_op)
        {
            val = get_binary(tokens, pointer, error);
        }
        else if (unary_op)
        {
            val = get_unary(tokens, pointer, error);
        }
        //else ??   
    }
    else if (tokens[*pointer].type == NUM)
        val = get_num(tokens, pointer, error);
    else 
        val = get_id(tokens, pointer, error);
        
    return val;
}

Node* get_unary(Token *const tokens, size_t *const pointer, Err_param *const error)
{
    assert(pointer);
    assert(error);

    size_t op = tokens[*pointer].value;
    (*pointer)++;

    Node* val = get_expression(tokens, pointer, error);

    if (op == LN)
    {
        Node* exp = make_node(NUM, EXP, nullptr, nullptr, error);
        val = make_node(OP, op, exp, val, error);
    }
    else
        val = make_node(OP, op, nullptr, val, error);

    return val;
}

Node* get_binary(Token *const tokens, size_t *const pointer, Err_param *const error)
{
    assert(pointer);
    assert(error);

    size_t op = tokens[*pointer].value;
    (*pointer)++;

    Node* val = get_expression(tokens, pointer, error);

    val = make_node(OP, op, nullptr, val, error);

    return val;
}

Node* get_num(Token *const token, size_t *const pointer, Err_param *const error)
{
    assert(pointer);
    assert(error);

    double value = token[*pointer].value;
    (*pointer)++;

    Node* val = make_node(NUM, value, nullptr, nullptr, error);

    return val;
}

Node* get_id(Token *const token, size_t *const pointer, Err_param *const error)
{
    assert(pointer);
    assert(error);

    double id_num = token[*pointer].value;
    (*pointer)++;

    Node* val = make_node(NUM, id_num, nullptr, nullptr, error);

    return val;
}