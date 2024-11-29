#include <stdio.h>

int get_add_sub (char* string, int* pointer);
int get_num (char* string, int* pointer);
int get_brace (char* string, int* pointer);
int get_mul_div (char* string, int* pointer);

int main ()
{
    char string[4000] = "25*10*(3*(25-10*2)+1)-15$";

    int pointer = 0;

    int val = get_add_sub (string, &pointer);

    if (string[pointer] != '$')
    {
        printf("NO END MARK PROBLEM\n");
        return 0;
    }

    pointer++;

    printf("RES %d\n", val);

    return 0;
}

int get_add_sub (char* string, int* pointer)
{
    int val = get_mul_div(string, pointer);

    while (string[*pointer] == '+' || string[*pointer] == '-')
    {
        char op = string[*pointer];
        (*pointer)++;
        int val2 = get_mul_div(string, pointer);

        if (op == '+')
            val += val2;
        else
            val -= val2;
    }

    return val;
}

int get_mul_div (char* string, int* pointer)
{
    int val = get_brace(string, pointer);

    while (string[*pointer] == '*' || string[*pointer] == '/')
    {
        char op = string[*pointer];
        (*pointer)++;

        int val2 = get_brace(string, pointer);

        if (op = '*')
            val *= val2;
        else
            val /= val2;
    }
    
    return val;
}

int get_brace (char* string, int* pointer)
{
    if (string[*pointer] == '(')
    {
        (*pointer)++;

        int val = get_add_sub(string, pointer);

        if (string[*pointer] != ')')
        {
            printf("BRACE PROBLEM\n");
            return 0;
        }

        (*pointer)++;

        return val;
    }
    else
    {
        int val = get_num(string, pointer);
        return val;
    }
        
}

int get_num (char* string, int* pointer)
{
    int val = 0;

    while (string[*pointer] <= '9' && string[*pointer] >= '0')
    {
        val = val * 10 + string[*pointer] - '0';
        
        (*pointer)++;
    }
    
    return val;
}