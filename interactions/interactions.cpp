#include <stdio.h>

#include "C:\Users\User\Desktop\differentiator2.0\interactions\interactions.h"

static Errors count_file_size (const char *const name, size_t* size);
static char* arithm_get_str (char* open, char** start_text, char* str);
static int find_var(char* str, bool* is_var);
static int find_op(char* str);

Errors input_ctor (Input *const base_text)
{
    assert(base_text);

    char* name = (char*)calloc(MAX_STR_LEN, sizeof(char));
    ALLOCATION_CHECK(name)

    char* text = (char*)calloc(MAX_FILE_SIZE, sizeof(char));
    ALLOCATION_CHECK(text)

    base_text->name = name;
    base_text->text = text;

    return ALL_RIGHT;
}

Errors get_database_name(Input *const base_text, char **const argv)
{
    assert(base_text);
    assert(argv);

    char* name = base_text->name;

    strncpy(name, argv[1], strlen(argv[1]) + 1);
    if(name == nullptr)
        return CPY_ERROR;

    base_text->name = name;

    return ALL_RIGHT;
}

Errors get_database_text (Input *const base_text)
{
    assert(base_text);

    Errors error = ALL_RIGHT;

    char* base_file_name = base_text->name;
    FILE* input_file;

    input_file = fopen(base_file_name, "rb");
    FILE_CHECK(input_file)

    size_t size = 0;

    error = count_file_size(base_text->name, &size);
    if(error != ALL_RIGHT)
        return STAT_ERROR;

    char* text = base_text->text;

    size_t read_result = fread(text, sizeof(char), size, input_file);
    if (read_result != size)
        return READ_ERROR;

    int close_res = fclose(input_file);
    if(close_res != 0)
        return CLOSE_ERROR;

    base_text->text = text;
    base_text->size = size;

    return ALL_RIGHT;
}

Errors count_file_size (const char *const name, size_t* size)
{
    assert(name);
    assert(size);

    struct stat file_info;

    if (stat(name, &file_info) == -1)
    {
        printf("stat problem\n");
        return STAT_ERROR;
    }

    *size = file_info.st_size;

    return ALL_RIGHT;
}

void handle_task (Input* base_text, Tree *const the_tree)
{
    assert(base_text);

    char* text = base_text->text;
    Node* start_node = the_tree->top;

    char* ch = strchr(text, ARITHM_OPEN_BRACE);

    arithm_read_tree_node(start_node, &ch, the_tree);
}

void arithm_read_tree_node (Node* node, char** start_text, Tree* the_tree)  //разбить на функции
{
    char* str = (char*)calloc(MAX_STR_LEN, sizeof(char));
    char* open = strchr(*start_text, ARITHM_OPEN_BRACE);

    char* endstr = nullptr; 

    bool is_var = false;
    double arg = 0;

    if (open == NULL)
        return;
    
    printf("START TEXT\n\n%s\n\n", *start_text);

    str = arithm_get_str (*start_text, start_text, str);  //лажово, надо поменять, конечно, названия
    printf("STR %s\n", str);

    int cmp_res = strncmp(str, EMPTY_STR, MAX_STR_LEN);

    if (cmp_res == 0)
    {
        free(node);
        return;
    }

    arg = strtod(str, &endstr);
    printf("ARG %lg\n", arg);
    cmp_res = strncmp(str, endstr, MAX_STR_LEN);
    if (cmp_res == 0)
    {
        //printf("HERE\n");
        arg = (double)find_var(endstr, &is_var);
        printf("VAR %lg\n", arg);

        node->type  = VAR;
        node->value = arg;

        if (arg == ERROR_VALUE)
        {
            arg = (double)find_op(endstr);
            printf("OP %lg\n", arg);

            if(arg == ERROR_VALUE)
                return;   //добавить ошибку

            node->type  = OP;
            node->value = arg;

            printf("TYPE %d\n", node->type);
        }
    }
    else
    {
        printf("NUM\n");
        node->type  = NUM;
        node->value = arg;
    }

    //graph_dump(the_tree->top, node);

    open = strchr(*start_text, ARITHM_OPEN_BRACE);
    char* close = strchr(*start_text, ARITHM_CLOSE_BRACE);

    if (open == NULL)
        return;

    if (open > close)
        return;
    
    str = nullptr;
    node->Left = node_ctor();  //с этим может быть проблема
    arithm_read_tree_node (node->Left, start_text, the_tree);

    //graph_dump(the_tree->top, node->Left);

    open = strchr(*start_text, ARITHM_OPEN_BRACE);
    close = strchr(*start_text, ARITHM_CLOSE_BRACE);

    str = nullptr;
    
    node->Right = node_ctor();
    arithm_read_tree_node (node->Right, start_text, the_tree);

    //graph_dump(the_tree->top, node->Right);

    free(str);

    return;
}

char* arithm_get_str (char* open, char** start_text, char* str)
{
    //char address[MAX_STR_LEN] = {};
    size_t symb_amt = 0;
    char* ch = nullptr;

    ch = strchr(open, ARITHM_OPEN_BRACE);
    if(!ch)
        ch = strchr(open, ARITHM_CLOSE_BRACE);

    ch++;
    symb_amt = ch - *start_text;
    char* open_ch = strchr(ch, ARITHM_OPEN_BRACE);
    char* close_ch = strchr(ch, ARITHM_CLOSE_BRACE);

    if (open_ch < close_ch)
    {
        printf("OPEN\n");
        sscanf(ch, "%[^(]", str);
    }
    else
    {
        printf("CLOSE\n");
        sscanf(ch, "%[^)]", str);
    }
        

    printf("STR IN GET STR %s\n", str);

    //strncpy(str, ch, MAX_STR_LEN);

    //*(str + strlen(address)) = '\0';
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
        //printf("NAME %s\n", variables[i]->name);

        cmp_res = strncmp(str, variables[i]->name, MAX_STR_LEN);
        //printf("CMP RES %d\n", cmp_res);

        if (cmp_res == 0)
        {
            //printf("REALLY VAR\n");
            *is_var = true;
            var_num = (int)i;
        }
        //printf("HELLO\n");
    }

    //printf("VAR NUM %d\n", var_num);
    return var_num;
}

int find_op(char* str)
{
    assert(str);

    int cmp_res = 0;
    int op_num = ERROR_VALUE;

    for (size_t i = 0; i < OP_AMT; i++)
    {
        printf("OP %d %s\n", i, operations[i]->name);
        cmp_res = strncmp(str, operations[i]->name, MAX_STR_LEN);
        if (cmp_res == 0)
        {
            op_num = (int)i;
            break;
        }
    }

    printf("OP NUM %d\n", op_num);
    return op_num;
}

void input_dtor(Input* base_text)
{
    free(base_text->name);
    free(base_text->text);
}