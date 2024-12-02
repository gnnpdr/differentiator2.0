#include <stdio.h>

#include "read_math_expression.h"

//поделить на этапы функцию

static char* arithm_get_str (char** start_text, char* str);
static void get_arg(Node *const node, char *const str, int cmp_res, Errors *const error);

void text_to_tree_convert (Input *const base_text, Tree *const the_tree, Errors *const error)
{
    assert(base_text);
    assert(the_tree);
    assert(error);

    char* text = base_text->text;
    Node* start_node = the_tree->root;

    char* text_ptr = strchr(text, ARITHM_OPEN_BRACE);
    
    arithm_read_tree_node(start_node, &text_ptr, the_tree, error);
}

void arithm_read_tree_node (Node *const node, char **const start_text, Tree *const the_tree, Errors *const error)  //разбить на функции
{
    assert(node);
    assert(start_text);
    assert(the_tree);
    assert(error);

    char* str = (char*)calloc(MAX_STR_LEN, sizeof(char));
    ALLOCATION_CHECK(str)

    char* open = strchr(*start_text, ARITHM_OPEN_BRACE);
    if (open == nullptr)
        return;

    str = arithm_get_str (start_text, str);

    int cmp_res = strncmp(str, EMPTY_STR, MAX_STR_LEN);
    if (cmp_res == 0)
    {
        node_dtor(node);
        return;
    }

    get_arg(node, str, cmp_res, error);
    CHECK

    open = strchr(*start_text, ARITHM_OPEN_BRACE);
    char* close = strchr(*start_text, ARITHM_CLOSE_BRACE);

    if (open == NULL)
        return;

    if (open > close)
        return;
    
    str = nullptr;   //поделить на три этапа
    node->Left = node_ctor(error);
    arithm_read_tree_node (node->Left, start_text, the_tree, error);
    CHECK

    open = strchr(*start_text, ARITHM_OPEN_BRACE);
    close = strchr(*start_text, ARITHM_CLOSE_BRACE);

    str = nullptr;
    
    node->Right = node_ctor(error);
    arithm_read_tree_node (node->Right, start_text, the_tree, error);
    CHECK

    free(str);
    return;
}

void get_arg(Node *const node, char *const str, int cmp_res, Errors *const error)
{
    assert(node);
    assert(str);
    assert(error);

    char* endstr = nullptr;
    bool is_var = false;
    double arg = 0;

    arg = strtod(str, &endstr);  //проверка не может быть осуществлена, 0 входит в норм значения

    cmp_res = strncmp(str, endstr, MAX_STR_LEN);
    if (cmp_res == 0)
    {
        arg = (double)get_var_ind(endstr, &is_var);

        node->type  = VAR;
        node->value = arg;

        if (arg == ERROR_VALUE)
        {
            arg = (double)get_op_ind(endstr);

            if(arg == ERROR_VALUE)
            {
                *error = SYN_ERROR;
                return;
            }

            node->type  = OP;
            node->value = arg;
        }
    }
    else
    {
        node->type  = NUM;
        node->value = arg;
    }
}

char* arithm_get_str (char** start_text, char* str)
{
    size_t symb_amt = 0;
    char* text_ptr = nullptr;

    text_ptr = strchr(*start_text, ARITHM_OPEN_BRACE);
    if(!text_ptr)
        text_ptr = strchr(*start_text, ARITHM_CLOSE_BRACE);
        

    text_ptr++;
    symb_amt = text_ptr - *start_text;
    char* open_ch = strchr(text_ptr, ARITHM_OPEN_BRACE);
    char* close_ch = strchr(text_ptr, ARITHM_CLOSE_BRACE);

    if (open_ch < close_ch && open_ch != nullptr)
        sscanf(text_ptr, "%[^(]", str);

    else if (open_ch == nullptr)
        sscanf(text_ptr, "%[^)]", str);

    else
        sscanf(text_ptr, "%[^)]", str);
    
    *start_text += symb_amt + strlen(str);

    return str;
}

int get_var_ind(char* str, bool* is_var)
{
    assert(str);
    int cmp_res = 0;
    int var_num = ERROR_VALUE;

    for (size_t i = 0; i < VAR_AMT; i++)
    {
        cmp_res = strncmp(str, variables[i]->name, MAX_STR_LEN);
        if (cmp_res == 0)
        {
            *is_var = true;
            var_num = (int)i;
        }
    }

    return var_num;
}

int get_op_ind(char* str)
{
    assert(str);

    int cmp_res = 0;
    int op_num = ERROR_VALUE;

    for (size_t i = 0; i < OP_AMT; i++)
    {
        cmp_res = strncmp(str, operations[i]->name, MAX_STR_LEN);
        if (cmp_res == 0)
        {
            op_num = (int)i;
            break;
        }
    }

    return op_num;
}