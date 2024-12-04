#include <stdio.h>

#include "rec_tree_read.h"

static Node* get_trig (char* string, int* pointer, Errors *const error);
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

    Node* root = get_trig (base_text->text, &pointer, error);

    if (base_text->text[pointer] != '$')
    {
        printf("NO END MARK PROBLEM\n");
        *error = SYN_ERROR;
        return 0;
    }

    pointer++;

    return root;
}

Node* get_trig (char* string, int* pointer, Errors *const error)
{
    Node* val = nullptr;

    char op_name[MAX_STR_LEN] = {};
    strncpy(op_name, get_str(string + *pointer, op_name), MAX_STR_LEN);
    int op_ind = get_op_ind(op_name);

    bool is_var = false;
    int var_ind = get_var_ind(op_name, &is_var);

    bool is_trig = false;

    if (op_ind != ERROR_VALUE)
        is_trig = true;

    if (strlen(op_name) == 0 || is_var)
        val = get_add_sub (string, pointer, error);
        
    else
    {
        int op_ind = get_op_ind(op_name);

        if (op_ind == ERROR_VALUE)
            printf("NO SUCH OPERATION\n");

        *pointer += strlen(op_name);

        val = get_add_sub(string, pointer, error);

        if (op_ind == SIN)
            val = make_node(OP, SIN, nullptr, val, error);

        else if (op_ind ==  COS)
            val = make_node(OP, COS, nullptr, val, error);

        else
            val = make_node(OP, TG, nullptr, val, error);
    }

    /*strncpy(op_name, "", MAX_STR_LEN);  //надо освободить строку
    strncpy(op_name, get_str(string + *pointer, op_name), MAX_STR_LEN);// не знаю, как по-другому. нельзя сувать в цикл повтор по трем или двум проверкам. да и с триемя стркмп тоже не хочется, надо аккуратно зацтклить, получилось через рекурсию
    op_ind = get_op_ind(op_name);  //даже не страшно, что это рекурсия. фактически как цикл, так как прям перед завершением
    //если не делать проверку снова, придется что-то изворачивать, если действовать рекурсией, то будет раскрываться как штука со скобками
    is_trig = false;

    if (op_ind != ERROR_VALUE)
        is_trig = true;

    while (is_trig)
        val = get_trig(string, pointer, error);*/

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
    bool is_var = false;
    char var_name[MAX_STR_LEN] = {};
    strncpy(var_name, get_str(string + *pointer, var_name), MAX_STR_LEN);

    int var_ind = get_var_ind(var_name, &is_var);

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
    else if (!is_var)
        val = get_num(string, pointer, error);

    else
    {
        if (var_ind == ERROR_VALUE)
        {
            printf("NO SUCH VAR\n");
            *error = SYN_ERROR;
            return nullptr;
        }
        
        val = get_var(string, pointer, var_ind, error);
    }
        
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