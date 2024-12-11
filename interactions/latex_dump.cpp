#include <stdio.h>

#include "latex_dump.h"

//
void latex_add(char *const str, char *const res_text, Err_param *const error)
{
    assert(str);
    assert(res_text);
    assert(error);

    int sprintf_res = 0;

    //надо добавить строчки, отличающие действия программы. Как это сделать? кейсы?
    //так как это не основная функция вывод, ее можно вложить в разные функции просто так, но как подавать сигнал о том, какой разделитель?
    //записывать в параметры? и делать еще один спринтф на строчку 

    sprintf_res = sprintf_s(res_text, MAX_STR_LEN, "%s%s\n\n", res_text, str);

    SPRINTF_CHECK
}


void latex_dump(char *const text, Err_param *const error)
{
    assert(text);
    assert(error);

    int sprintf_res = sprintf_s(text, MAX_STR_LEN, "%s%s", text, LATEX_END);
    SPRINTF_CHECK

    FILE* latex_file;

    latex_file = fopen(FILE_NAME, "w");
    FILE_CHECK(latex_file)

    int write_res = fwrite(text, sizeof(char), MAX_FILE_SIZE, latex_file);
    WRITE_CHECK

    int close_res = fclose(latex_file);
    CLOSE_CHECK
}

char* latex_ctor(Err_param *const error)
{
    assert(error);

    char* text = (char*)calloc(MAX_FILE_SIZE, sizeof(char));
    ALLOCATION_CHECK_RET(text)

    int sprintf_res = sprintf_s(text, MAX_STR_LEN, "%s", LATEX_START);
    SPRINTF_CHECK_NULL

    return text;
}

void latex_dtor(char* text)
{
    free(text);
}