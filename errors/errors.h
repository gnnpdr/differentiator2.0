#ifndef _ERRORS_H_
#define _ERRORS_H_

enum Errors  //מעהוכüםûי פאיכ
{
    ALL_RIGHT,
    FILE_ERROR,
    READ_ERROR,
    WRITE_ERROR,
    CLOSE_ERROR,
    CMD_ERROR,
    ALLOCATION_ERROR,
    CPY_ERROR,
    STAT_ERROR
};

#define FILE_CHECK(file)    do                                      \
                            {                                       \
                                if (file == nullptr)                \
                                {                                   \
                                    printf("file wasn't opened\n"); \
                                    return FILE_ERROR;              \
                                }                                   \
                            }while(0);

#define ALLOCATION_CHECK(buf) do                                    \
                            {                                       \
                                if (buf == 0)                       \
                                {                                   \
                                    printf("no place\n");           \
                                    return ALLOCATION_ERROR;        \
                                }                                   \
                            }while(0);

#endif //_ERRORS_H_