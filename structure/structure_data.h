#ifndef _STRUCTURE_DATA_H_
#define _STRUCTURE_DATA_H_


//------------------------CONST-----------------------------------

static const size_t VAR_AMT =  1;
static const size_t OP_AMT  =  9;
static const char*  X_STR   = "x";  //pragma

static const double START_VALUE =  0;
static const double PI          = 3.14;  //ну надо было

static const char*  ADD_STR = "+";
static const char*  MUL_STR = "*";
static const char*  DIV_STR = "/";
static const char*  SUB_STR = "-";
static const char*  POW_STR = "^";
static const char*  LOG_STR = "log";
static const char*  SIN_STR = "sin";
static const char*  COS_STR = "cos";
static const char*  TG_STR  = "tg";

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
	SUB = 3,
	POW = 4,
	LOG = 5,
	SIN = 6,
	COS = 7,
	TG  = 8
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
	Node* root;
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

static const struct OperationParameters ADD_STRUCT = {ADD_STR, ADD};
static const struct OperationParameters MUL_STRUCT = {MUL_STR, MUL};
static const struct OperationParameters DIV_STRUCT = {DIV_STR, DIV};
static const struct OperationParameters SUB_STRUCT = {SUB_STR, SUB};
static const struct OperationParameters POW_STRUCT = {POW_STR, POW};
static const struct OperationParameters LOG_STRUCT = {LOG_STR, LOG};
static const struct OperationParameters SIN_STRUCT = {SIN_STR, SIN};
static const struct OperationParameters COS_STRUCT = {COS_STR, COS};
static const struct OperationParameters  TG_STRUCT = { TG_STR,  TG};

static const struct OperationParameters* operations [OP_AMT] = {&ADD_STRUCT,
																&MUL_STRUCT,
																&DIV_STRUCT,
																&SUB_STRUCT,
																&POW_STRUCT,
																&LOG_STRUCT,
																&SIN_STRUCT,
																&COS_STRUCT,
																&TG_STRUCT};

//--------------------DEFINE------------------------------------------

#define NUM(num) 			make_node(NUM, num,  NULL,  NULL)
#define X 		 			make_node(VAR,  x ,  NULL,  NULL)
#define ADD(node1, node2)	make_node(OP , ADD, node1, node2)
#define MUL(node1, node2)	make_node(OP , MUL, node1, node2)
#define DIV(node1, node2)	make_node(OP , DIV, node1, node2)
#define SUB(node1, node2)	make_node(OP , SUB, node1, node2)

#endif //_STRUCTURE_DATA_H_