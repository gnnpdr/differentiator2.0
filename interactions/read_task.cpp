#include <stdio.h>

#include "read_task.h"

static char* arithm_get_str (char** start_text, char* str);
static void get_arg(Node *const node, char *const str, int cmp_res);
static int find_var(char* str, bool* is_var);
static int find_op(char* str);

void handle_task (Input *const base_text, Tree *const the_tree)
{
    assert(base_text);
    assert(the_tree);

    char* text = base_text->text;
    Node* start_node = the_tree->top;

    char* ch = strchr(text, ARITHM_OPEN_BRACE);

    arithm_read_tree_node(start_node, &ch, the_tree);
}

void arithm_read_tree_node (Node *const node, char **const start_text, Tree *const the_tree)  //разбить на функции
{
    assert(node);
    assert(start_text);
    assert(the_tree);

    char* str = (char*)calloc(MAX_STR_LEN, sizeof(char));
    char* open = strchr(*start_text, ARITHM_OPEN_BRACE);

    if (open == NULL)
        return;

    str = arithm_get_str (start_text, str);

    int cmp_res = strncmp(str, EMPTY_STR, MAX_STR_LEN);
    if (cmp_res == 0)
    {
        free(node);
        return;
    }

    get_arg(node, str, cmp_res);

    open = strchr(*start_text, ARITHM_OPEN_BRACE);
    char* close = strchr(*start_text, ARITHM_CLOSE_BRACE);

    if (open == NULL)
        return;

    if (open > close)
        return;
    
    str = nullptr;
    node->Left = node_ctor();
    arithm_read_tree_node (node->Left, start_text, the_tree);

    open = strchr(*start_text, ARITHM_OPEN_BRACE);
    close = strchr(*start_text, ARITHM_CLOSE_BRACE);

    str = nullptr;
    
    node->Right = node_ctor();
    arithm_read_tree_node (node->Right, start_text, the_tree);

    free(str);
    return;
}

void get_arg(Node *const node, char *const str, int cmp_res)
{
    char* endstr = nullptr;
    bool is_var = false;
    double arg = 0;

    arg = strtod(str, &endstr);
    cmp_res = strncmp(str, endstr, MAX_STR_LEN);
    if (cmp_res == 0)
    {
        arg = (double)find_var(endstr, &is_var);

        node->type  = VAR;
        node->value = arg;

        if (arg == ERROR_VALUE)
        {
            arg = (double)find_op(endstr);

            if(arg == ERROR_VALUE)
                return;   //добавить ошибку

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
    char* ch = nullptr;

    ch = strchr(*start_text, ARITHM_OPEN_BRACE);
    if(!ch)
        ch = strchr(*start_text, ARITHM_CLOSE_BRACE);

    ch++;
    symb_amt = ch - *start_text;
    char* open_ch = strchr(ch, ARITHM_OPEN_BRACE);
    char* close_ch = strchr(ch, ARITHM_CLOSE_BRACE);

    if (open_ch < close_ch)
        sscanf(ch, "%[^(]", str);
    else
        sscanf(ch, "%[^)]", str);
    
    *start_text += symb_amt + strlen(str);

    return str;
    
}

int find_var(char* str, bool* is_var)
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

int find_op(char* str)
{
    assert(str);

    int cmp_res = 0;
    int op_num = ERROR_VALUE;

    for (size_t i = 0; i < OP_AMT; i++)
    {
        //printf("OP %lld %s\n", i, operations[i]->name);
        cmp_res = strncmp(str, operations[i]->name, MAX_STR_LEN);
        if (cmp_res == 0)
        {
            op_num = (int)i;
            break;
        }
    }

    return op_num;
}