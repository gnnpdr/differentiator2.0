#include <stdio.h>

#include "tree_structure.h"

Node* node_ctor ()
{
	Node* node = (Node*)calloc(1, sizeof(Node));
	return node;
}

Node* make_node(Type type, double value, Node* Left, Node* Right)
{//node_ctor
	Node* node = (Node*)calloc(1, sizeof(Node));  //надо сделать проверку. по всему коду надо протянуть единую переменную для ошибок
	//ALLOCATION_CHECK(node)
	node->type = type;
	node->value = value;
	node->Left = Left;
	node->Right = Right;

	return node;
}

Errors tree_ctor (Tree *const the_tree, Node *const start_node)
{
    assert(the_tree);
    the_tree->root = start_node;

    return ALL_RIGHT;
}

void tree_dtor(Node* node)
{
	assert(node);

	if (!node->Left && !node->Right)
	{
		free(node);
		node = nullptr;
		return;
	}	

	if (node->Left)
	{
		tree_dtor(node->Left);

		if (!node->Right)
			free(node);
	}

	if (node->Right)
	{
		tree_dtor(node->Right);
		free(node);
	}
}


void node_dtor(Node* node)
{
	assert(node);

	free(node);
}