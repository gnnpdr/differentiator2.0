#ifndef _ERRORS_H_
#define _ERRORS_H_

enum Errors  //��������� ����
{
    ALL_RIGHT,
    FILE_ERROR,
    READ_ERROR,
    WRITE_ERROR,
    CLOSE_ERROR,
    CMD_ERROR,
    ALLOCATION_ERROR,
    CPY_ERROR,
    STAT_ERROR,
    SPRINTF_ERROR,
    SYN_ERROR,
    MATH_ERROR
};

#define FILE_CHECK(file)    do                                      \
                            {                                       \
                                if (file == nullptr)                \
                                {                                   \
                                    printf("file wasn't opened\n"); \
                                    *error = FILE_ERROR;            \
                                    return;                         \
                                }                                   \
                            }while(0);

#define ALLOCATION_CHECK(buf) do                                    \
                            {                                       \
                                if (buf == 0)                       \
                                {                                   \
                                    printf("no place\n");           \
                                    *error = ALLOCATION_ERROR;      \
                                    return;                         \
                                }                                   \
                            }while(0);

#define SPRINTF_CHECK do                                            \
                            {                                       \
                                if (sprintf_res == -1)              \
                                {                                   \
                                    *error = SPRINTF_ERROR;         \
                                    return;                         \
                                }                                   \
                            }while(0);


#define CPY_CHECK(name)     do                                   \
                            {                                    \
                                if(name == nullptr)              \
                                {                                \
                                    *error =  CPY_ERROR;         \
                                    return;                      \
                                }                                \
                            }while(0);

#define READ_CHECK           do                                 \
                            {                                   \
                                if (read_result != size)        \
                                {                               \
                                    *error = READ_ERROR;        \
                                    return;                     \
                                }                               \
                            }while(0);

#define CLOSE_CHECK           do                                \
                            {                                   \
                                if(close_res != 0)              \
                                {                               \
                                    *error = CLOSE_ERROR;       \
                                    return;                     \
                                }                               \
                            }while(0);

#define CHECK               do                                  \
                            {                                   \
                                if(*error != ALL_RIGHT)         \
                                    return;                     \
                            }while(0);


#endif //_ERRORS_H_