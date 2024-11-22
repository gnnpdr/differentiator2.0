#include <stdio.h>

#define AKIN
#ifdef AKIN
#include "get_database.h"
#endif

static Errors count_file_size (const char *const name, size_t* size);
//static void read_tree_node (Node* node, char** start_text, Tree* the_tree);
//static char* get_str (char* open, char** start_text);


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
/*
void handle_text (Input* base_text, Tree *const the_tree)
{
    assert(base_text);

    char* text = base_text->text;
    Node* start_node = the_tree->top;

    char* ch = strchr(text, QUOT_MARK);

    read_tree_node(start_node, &ch, the_tree);
}

void read_tree_node (Node* node, char** start_text, Tree* the_tree)  //разбить на функции
{
    #ifdef AKIN
    char* open = strchr(*start_text, OPEN_BRACE);
    #endif

    #ifdef DIF
    char* open = strchr(*start_text, ARITHM_OPEN_BRACE);
    #endif

    char* str = nullptr;

    if (open == NULL)
        return;

    str = get_str (*start_text, start_text);  //лажово, надо поменять, конечно, названия
    
    int cmp_res = strncmp(str, EMPTY_STR, MAX_STR_LEN);

    if(cmp_res != 0)
        node->str = str;
    else
    {
        free(node);
        return;
    }

    #ifdef AKIN
    open = strchr(*start_text, OPEN_BRACE);
    char* close = strchr(*start_text, CLOSE_BRACE);
    #endif

    #ifdef DIF
    open = strchr(*start_text, ARITHM_OPEN_BRACE);
    char* close = strchr(*start_text, ARITHM_CLOSE_BRACE);
    #endif

    if (open == NULL)
        return;

    if (open > close)
        return;
    

    str = nullptr;
    //node->Left = make_node(str);
    read_tree_node (node->Left, start_text, the_tree);

    #ifdef AKIN
    open = strchr(*start_text, OPEN_BRACE);
    close = strchr(*start_text, CLOSE_BRACE);
    #endif

    #ifdef DIF
    open = strchr(*start_text, ARITHM_OPEN_BRACE);
    close = strchr(*start_text, ARITHM_CLOSE_BRACE);
    #endif

    str = nullptr;
    
    //node->Right = make_node(str);
    read_tree_node (node->Right, start_text, the_tree);

    return;
}

char* get_str (char* open, char** start_text)
{
    char address[MAX_STR_LEN] = {};
    size_t symb_amt = 0;
    char* ch = nullptr;

    ch = strchr(open, QUOT_MARK);
    ch++;
    symb_amt = ch - *start_text;
    sscanf(ch, "%[^\"]", address);
    *(ch + strlen(address)) = '\0';
    *start_text += symb_amt + strlen(address) + 1;

    return ch;
    
}
*/
void input_dtor(Input* base_text)
{
    free(base_text->name);
    free(base_text->text);
}