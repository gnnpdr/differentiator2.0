#include <stdio.h>

#include "rec_tree_read.h"

static Node* get_trig (char* string, int* pointer, size_t op_ind, Errors *const error);
static Node* get_log(char* string, int* pointer, size_t op_ind, Errors *const error);
static Node* get_pow (char* string, int* pointer, Errors *const error);
static Node* get_add_sub (char* string, int* pointer, Errors *const error);
static Node* get_num (char* string, int* pointer, Errors *const error);
static Node* get_brace (char* string, int* pointer, Errors *const error);
static Node* get_mul_div (char* string, int* pointer, Errors *const error);
static Node* get_var (char* string, int* pointer, int var_ind, Errors *const error);

static char* get_str(char *const string, char *const var_name);

Node* read_tree_rec (Input *const base_text, Errors* error)
{
    int pointer = 0;

    Node* root = get_add_sub (base_text->text, &pointer, error);

    if (base_text->text[pointer] != '$')
    {
        printf("NO END MARK PROBLEM\n");
        *error = SYN_ERROR;
        return 0;
    }

    pointer++;

    return root;
}

Node* get_add_sub (char* string, int* pointer, Errors *const error)
{
    Node* val = get_pow(string, pointer, error);

    while (string[*pointer] == '+' || string[*pointer] == '-')
    {
        char op = string[*pointer];
        (*pointer)++;
        Node* val2 = get_pow(string, pointer, error);

        if (op == '+')
            val = make_node(OP, ADD, val, val2, error);
        else
            val = make_node(OP, SUB, val, val2, error);
    }

    return val;
}

Node* get_pow (char* string, int* pointer, Errors *const error)
{
    Node* val = get_mul_div(string, pointer, error);

    while (string[*pointer] == '^')
    {
        char op = string[*pointer];
        (*pointer)++;

        Node* val2 = get_mul_div(string, pointer, error);

        val = make_node(OP, POW, val, val2, error);
    }

    return val;
}

Node* get_mul_div (char* string, int* pointer, Errors *const error)
{
    Node* val = get_brace(string, pointer, error);

    while (string[*pointer] == '*' || string[*pointer] == '/')
    {
        char op = string[*pointer];
        (*pointer)++;

        Node* val2 = get_brace(string, pointer, error);

        if (op == '*')
            val = make_node(OP, MUL, val, val2, error);
        else
            val = make_node(OP, DIV, val, val2, error);
    }
    
    return val;
}

Node* get_brace (char* string, int* pointer, Errors *const error)
{
    char name[MAX_STR_LEN] = {};
    strncpy(name, get_str(string + *pointer, name), MAX_STR_LEN);
    int op_ind = get_op_ind(name);

    bool is_var = false;
    int var_ind = get_var_ind(name, &is_var);

    bool is_trig = false;

    if (op_ind == SIN || op_ind == COS || op_ind == TAN)  //надо сделать отдельную структуру с этими значениями и вложить в основную с операциями
        is_trig = true;

    Node* val = nullptr;

    if (string[*pointer] == '(')
    {
        (*pointer)++;

        val = get_add_sub(string, pointer, error);

        if (string[*pointer] != ')')
        {
            printf("BRACE PROBLEM\n");
            *error = SYN_ERROR;
            return 0;
        }

        (*pointer)++;
    }
    else if (string[*pointer] == '{')  //для логарифма
    {
        (*pointer)++;

        val = get_add_sub(string, pointer, error);

        if (string[*pointer] != '}')
        {
            printf("BRACE PROBLEM\n");
            *error = SYN_ERROR;
            return 0;
        }

        (*pointer)++;
    }
    else if (is_var)
    {
        if (var_ind == ERROR_VALUE)
        {
            printf("NO SUCH VAR\n");
            *error = SYN_ERROR;
            return nullptr;
        }
        
        val = get_var(string, pointer, var_ind, error);
    }
    else if (is_trig)
        val = get_trig(string, pointer, op_ind, error);
    
    else if (op_ind == LOG || op_ind == LN)
        val = get_log(string, pointer, op_ind, error);

    else if (!is_var)
        val = get_num(string, pointer, error);
        
    return val;
}

Node* get_var (char* string, int* pointer, int var_ind, Errors *const error)
{
    double val = 0;

    size_t var_len = strlen(variables[var_ind]->name);

    for (int i = 0; i < var_len; i++)
        (*pointer)++;

    Node* node = make_node(VAR, (double)var_ind, nullptr, nullptr, error);

    return node;
}

Node* get_num (char* string, int* pointer, Errors *const error)
{
    double val = 0;

    size_t start_pointer = *pointer;
    while (string[*pointer] <= '9' && string[*pointer] >= '0')
    {
        val = val * 10 + string[*pointer] - '0';
        
        (*pointer)++;
    }
    
    if (string[*pointer] == '.')
    {
        (*pointer)++;

        size_t i = 1;
        while (string[*pointer] <= '9' && string[*pointer] >= '0')
        {
            val += (string[*pointer] - '0') / pow(10, i);
            i++;

            (*pointer)++;
        }
    }

    if (*pointer == start_pointer)
    {
        printf("NO NUMBER\n");
        *error = SYN_ERROR;
        return nullptr;
    }

    Node* node = make_node(NUM, val, nullptr, nullptr, error);

    return node;
}

char* get_str(char *const string, char *const name)
{
    size_t ind = 0;

    while (isalpha(string[ind]))
    {
        sprintf_s(name, MAX_STR_LEN, "%s%c", name, string[ind]);
        ind++;
    }

    return name;
}

Node* get_trig (char* string, int* pointer, size_t op_ind, Errors *const error)
{
    *pointer += strlen(operations[op_ind]->name);
    Node* val = get_add_sub(string, pointer, error);

    if (op_ind == SIN)
        val = make_node(OP, SIN, nullptr, val, error);

    else if (op_ind ==  COS)
        val = make_node(OP, COS, nullptr, val, error);

    else
        val = make_node(OP, TAN, nullptr, val, error);

    return val;
}

Node* get_log(char* string, int* pointer, size_t op_ind, Errors *const error)
{
    *pointer += strlen(operations[op_ind]->name);
    
    Node* base = nullptr;

    if (op_ind == LN)  //для вида ln{}
        base = make_node(NUM, EXP, nullptr, nullptr, error);
    else
    {
        (*pointer)++; //для вида log_{}{}
        base = get_add_sub(string, pointer, error);
    }
        

    Node* val = get_add_sub(string, pointer, error);

    val = make_node(OP, LOG, base, val, error);

    return val;
}