#include <stdio.h>

#include "lex_analysis.h"

static Token* tokens_ctor(Err_param *const error);
static void make_token(Token *const tokens, Type type, double val, Err_param *const error);
static int find_free_token(Token *const tokens, Err_param *const error);

static Id* id_ctor(Err_param *const error);
static size_t make_id(Id *const ids, size_t len, char *const text, Err_param *const error);
static int find_free_id(Id *const ids, Err_param *const error);

static void get_num(char *const text, size_t *const pointer, Token *const tokens, Err_param *const error);
static bool get_op(char *const text, size_t *const pointer, Token *const tokens, Err_param *const error);
static size_t find_match(char *const start_address, size_t len);
static void get_id(char *const text, Id *const ids, size_t *const pointer, Token *const tokens, Err_param *const error);

void lex_analysis(Input *const base_text, Err_param* error)
{
    assert(base_text);
    assert(error);

    char* text = base_text->text;

    Token* tokens = tokens_ctor(error);
    RETURN_VOID

    Id* ids = id_ctor(error);
    RETURN_VOID

    size_t pointer = 0;
    size_t size = base_text->size;
    char symb = text[pointer];

    while (pointer != size)
    {
        bool is_func = false;

        if (symb >= '0' && symb <= '9')
            get_num(text, &pointer, tokens, error);
        else 
            is_func = get_op(text, &pointer, tokens, error);
        
        if (!is_func)
            get_id(text, ids, &pointer, tokens, error);

        RETURN_VOID
    }
}

//------------------TOKENS------------------------------
Token* tokens_ctor(Err_param *const error)
{
    assert(error);

    Token* tokens = (Token*)calloc(TOKEN_AMT, sizeof(Token));
    ALLOCATION_CHECK_RET(tokens)

    for (int i = 0; i < TOKEN_AMT; i++)
        tokens[i].value = ERROR_VALUE;

    return tokens;
}

void make_token(Token *const tokens, Type type, double val, Err_param *const error)
{
    assert(tokens);
    assert(error);

    size_t ind = find_free_token(tokens, error);
    RETURN_VOID

    tokens[ind].type = type;
    tokens[ind].value = val;
}

int find_free_token(Token *const tokens, Err_param *const error)
{
    assert(tokens);
    assert(error);

    size_t free_ind = ERROR_VALUE_SIZE_T;

    for (size_t ind = 0; ind < TOKEN_AMT; ind++)
    {
        if (tokens[ind].value == ERROR_VALUE)
        {
            free_ind = ind;
            break;
        }
    }

    if (free_ind == ERROR_VALUE_SIZE_T)
    {
        printf("no free tokens\n");
        ERROR(ALLOCATION_ERROR)
    }

    return free_ind;
}

//-------------------------IDS--------------------------------
Id* id_ctor(Err_param *const error)
{
    assert(error);

    Id* ids = (Id*)calloc(ID_AMT, sizeof(Id));
    ALLOCATION_CHECK(ids)

    for (int i = 0; i < ID_AMT; i++)
        ids[i].len = ERROR_VALUE_SIZE_T;

    return ids;
}

size_t make_id(Id *const ids, size_t len, char *const text, Err_param *const error)
{
    assert(ids);
    assert(text);
    assert(error);

    size_t free_ind = find_free_id(ids, error);
    RETURN_VOID

    ids[free_ind].len = len;
    ids[free_ind].start_address = text;

    return free_ind;
}

int find_free_id(Id *const ids, Err_param *const error)
{
    assert(ids);
    assert(error);

    size_t free_ind = ERROR_VALUE_SIZE_T;

    for (size_t ind = 0; ind < TOKEN_AMT; ind++)
    {
        if (ids[ind].len == ERROR_VALUE_SIZE_T)
        {
            free_ind = ind;
            break;
        }
    }

    if (free_ind == ERROR_VALUE_SIZE_T)
    {
        printf("no free ids\n");
        ERROR(ALLOCATION_ERROR)
    }

    return free_ind;
}


//----------------------LEX_ANALYSIS-------------------------
void get_num(char *const text, size_t *const pointer, Token *const tokens, Err_param *const error)
{
    assert(text);
    assert(pointer);
    assert(tokens);
    assert(error);

    double val = 0;

    size_t start_pointer = *pointer;
    while (text[*pointer] >= '0' && text[*pointer] <= '9')
    {
        val = val * 10 + text[*pointer] - '0';
        (*pointer)++;
    }
    
    if (text[*pointer] == '.')
    {
        (*pointer)++;

        size_t i = 1;
        while (text[*pointer] >= '0' && text[*pointer] <= '9')
        {
            val += (text[*pointer] - '0') / pow(10, i);
            i++;

            (*pointer)++;
        }
    }

    if (*pointer == start_pointer)
    {
        printf("NO NUMBER\n");
        ERROR(SYN_ERROR)
        return;
    }

    make_token(tokens, NUM, val, error);
    RETURN_VOID
}

bool get_op(char *const text, size_t *const pointer, Token *const tokens, Err_param *const error)
{
    assert(text);
    assert(pointer);
    assert(tokens);
    assert(error);

    size_t len = 0;
    char* start_address = text + *pointer;

    while (isalpha(*(start_address + len)))
        len++;
        

    size_t op_ind = find_match(start_address, len);
    if (op_ind == ERROR_VALUE_SIZE_T)
        return false;
    
    make_token(tokens, OP, op_ind, error);
    RETURN_BOOL 

    *pointer += len;

    return true;
}

size_t find_match(char *const start_address, size_t len)
{
    assert(start_address);

    size_t match_ind = ERROR_VALUE_SIZE_T;

    for (size_t ind = 0; ind < OP_AMT; ind++)
    {
        int cmp_res = strncmp(start_address, operations[ind]->name, len);

        if(cmp_res == 0)
        {
            match_ind = ind;
            break;
        }
    }

    return match_ind;
}

void get_id(char *const text, Id *const ids, size_t *const pointer, Token *const tokens, Err_param *const error)
{
    assert(text);
    assert(pointer);
    assert(tokens);
    assert(error);

    size_t len = 0;
    char* start_address = text + *pointer;

    while (isalpha(*(start_address + len)))
        len++;
    
    size_t id_ind = make_id(ids, len, text, error);
    RETURN_VOID

    make_token(tokens, ID, id_ind, error);
    RETURN_VOID

    *pointer += len;
}