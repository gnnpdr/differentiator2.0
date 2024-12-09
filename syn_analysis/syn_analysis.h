#ifndef _SYN_H_
#define _SYN_H_

#include "../lex_analysis/lex_analysis.h"

static const size_t FIRST_PR_AMT = 2;
static const size_t SEC_PR_AMT = 3;
static const size_t BINARY_OP_AMT = 1;
static const size_t UNARY_OP_AMT = 4;

static const double EXP = 2.72;

static const struct OperationParameters first_pr [FIRST_PR_AMT]  = {ADD_STRUCT,
															        SUB_STRUCT};

static const struct OperationParameters sec_pr [SEC_PR_AMT] = {MUL_STRUCT,
															   DIV_STRUCT,
															   POW_STRUCT};


static const struct OperationParameters binary_op [BINARY_OP_AMT] = {LOG_STRUCT};

static const struct OperationParameters unary_op [UNARY_OP_AMT]  = {LN_STRUCT ,
																    SIN_STRUCT,
																    COS_STRUCT,
																    TAN_STRUCT};

Node* syn_analysis(Token *const tokens, Err_param *const error);

#endif //_SYN_H_