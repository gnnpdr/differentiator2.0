#include <stdio.h>

#include "tree_structure.h"

Node* node_ctor (Errors *const error)
{
	assert(error);

	Node* node = (Node*)calloc(1, sizeof(Node));
	if (node == nullptr)
		*error = ALLOCATION_ERROR;

	return node;
}

Node* make_node(Type type, double value, Node* Left, Node* Right, Errors *const error)
{
	assert(error);
	Node* node = node_ctor(error);
	if (*error != ALL_RIGHT)
		return nullptr;
	
	node->type = type;
	node->value = value;
	node->Left = Left;
	node->Right = Right;

	return node;
}

void tree_ctor (Tree *const the_tree, Node *const start_node)
{
    assert(the_tree);
	assert(start_node);

    the_tree->root = start_node;
}

Node* tree_dtor(Node* node)
{
	assert(node);

	if (!node->Left && !node->Right)
	{
		free(node);
		node = nullptr;
		return node;
	}	

	if (node->Left)
	{
		tree_dtor(node->Left);

		if (!node->Right)
		{
			free(node);
			node = nullptr;
		}
	}

	if (node->Right)
	{
		tree_dtor(node->Right);
		free(node);
		node = nullptr;
	}

	return node;
}


Node* node_dtor(Node* node)
{
	assert(node);

	free(node);
	node = nullptr;

	return node;
}