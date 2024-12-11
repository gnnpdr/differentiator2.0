#ifndef _LATEX_H_
#define _LATEX_H_

#include "talor.h"

static const char* LATEX_START = "\\documentclass{article}\n\\usepackage[english]{babel}\n\\usepackage{amsmath}\n\\begin{document}\n\n";
static const char* LATEX_END = "\\end{document}\n";
static const char* FILE_NAME = "letex.txt";

char* latex_ctor(Err_param *const error);
void latex_dtor(char* text);

void latex_add(char *const text, char *const res_text, Err_param *const error);

void latex_dump(char *const text, Err_param *const error);

#endif //_LATEX_H_