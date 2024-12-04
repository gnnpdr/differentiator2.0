#ifndef _USER_H_
#define _USER_H_

#include "..\interactions\write_math_expression.h"

static const size_t ANS_AMT = 2;

enum Answer  //надо будет сделать файл для взаимодействия с юзером с этим енамом и функциями, связанными в выгрузкой в латех
{
	DIFF,
	TALOR
};

static const char* DO_DIFF = "diff";
static const char* DO_TALOR = "talor";

struct AnswerParameters
{
    const char* answer;
    int num;
};

static const struct AnswerParameters DIFF_STRUCT = {DO_DIFF, DIFF};
static const struct AnswerParameters TALOR_STRUCT = {DO_TALOR, TALOR};


static const struct AnswerParameters* answers [ANS_AMT] = {&DIFF_STRUCT,
														   &TALOR_STRUCT};


void choose_do_calc(Tree* the_tree, Errors* error);                                                     
#endif //_USER_H_