#ifndef _STRUCT_H_
#define _STRUCT_H_

//-----------------------LIBS-------------------------------------
#include "..\errors\errors.h"

#include <assert.h>
#include <stdlib.h>

//------------------------CONST-----------------------------------

static const size_t VAR_AMT =  1;
static const size_t OP_AMT  =  4;
static const char*  X_STR   = "x";

static const double START_VALUE = 0;

static const char*  ADD_STR = "+";
static const char*  MUL_STR = "*";
static const char*  DIV_STR = "/";
static const char*  SUB_STR = "-";

//-----------------------ENUM-------------------------------------

enum Variable
{
    X_VAR = 0,
};

enum Type
{
	NUM = 0,
	VAR = 1,
	OP  = 2
};

enum Operation
{
	ADD = 0,
	MUL = 1,
	DIV = 2,
	SUB = 3
};

//----------------------STRUCT-----------------------------------

struct Node
{
	Type type;
	double value;
	Node* Left;
	Node* Right;
};

struct Tree
{
	Node* top;
};

struct VariableParameters
{
    double value;
    const char* name;
    size_t num;
};

struct OperationParameters
{
    const char* name;
    Operation num;
};

static struct VariableParameters X_STRUCT = {START_VALUE, X_STR, X_VAR};

static struct VariableParameters* variables [VAR_AMT] = {&X_STRUCT};


const struct OperationParameters ADD_STRUCT = {ADD_STR, ADD};
const struct OperationParameters MUL_STRUCT = {MUL_STR, MUL};
const struct OperationParameters DIV_STRUCT = {DIV_STR, DIV};
const struct OperationParameters SUB_STRUCT = {SUB_STR, SUB};

static const struct OperationParameters* operations [OP_AMT] = {&ADD_STRUCT,
																&MUL_STRUCT,
																&DIV_STRUCT,
																&SUB_STRUCT};

//--------------------DEFINE------------------------------------------

#define NUM(num) 			make_node(NUM, num,  NULL,  NULL)
#define X 		 			make_node(VAR,  x ,  NULL,  NULL)
#define ADD(node1, node2)	make_node(OP , ADD, node1, node2)
#define MUL(node1, node2)	make_node(OP , MUL, node1, node2)
#define DIV(node1, node2)	make_node(OP , DIV, node1, node2)
#define SUB(node1, node2)	make_node(OP , SUB, node1, node2)


//----------------FUNC-------------------------------------------------

Node* node_ctor ();
Errors tree_ctor (Tree *const the_tree, Node *const start_node);
void tree_dtor(Node* node);
Node* make_node(Type type, int value, Node* Left, Node* Right);
void node_dtor(Node* node);

#endif //_STRUCT_H_