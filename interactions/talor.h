#ifndef _TALOR_H_
#define _TALOR_H_

#include "diff.h"

Node* make_talor(Node *const root, int decomp_degree, double denom, Err_param *const error);

#endif //_TALOR_H_