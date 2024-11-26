#ifndef _STRUCT_H_
#define _STRUCT_H_

//-----------------------LIBS-------------------------------------
#include "..\errors\errors.h"
#include "structure_data.h"

#include <assert.h>
#include <stdlib.h>

//----------------------FUNCS--------------------------------------
Node* node_ctor (Errors *const error);
void tree_ctor (Tree *const the_tree, Node *const start_node);
Node* make_node(Type type, double value, Node* Left, Node* Right, Errors *const error);

Node* tree_dtor(Node* node);
Node* node_dtor(Node* node);

#endif //_STRUCT_H_