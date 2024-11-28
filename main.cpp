#include <stdio.h>

#include "structure\tree_structure.h"
#include "draw\draw_tree.h"
#include "interactions\read_math_expression.h"
#include "interactions\diff.h"
#include "interactions\write_math_expression.h"

int main(int argc, char** argv)
{
	Errors error = ALL_RIGHT;
	Input base_text = {};

    input_ctor(&base_text, &error);
	//printf("error %d\n", error);

    get_database_name(&base_text, argv, &error);
	//printf("error %d\n", error);

    get_database_text(&base_text, &error);
	//printf("error %d\n", error);


	Node* root = node_ctor (&error);

	Tree the_tree = {};
	tree_ctor (&the_tree, root);
	//printf("error %d\n", error);


	//printf("INIT TREE\n");
	//printf("base text %p\nthe_tree %p\nerror %p\n", &base_text, &the_tree, &error);

    text_to_tree_convert (&base_text, &the_tree, &error);
	
	Stack stk = {};   //можно вложить в макрос, чтобы не надо было кажды раз задавать стэк и удалять его, чтобы написать строчку
	stk_ctor(&stk, &error);
	//graph_dump(the_tree.root, root, &error);
	write_math_expression(root, &stk, &error);
	
	Node* diff_tree = diff_node(the_tree.root, &error);
	//graph_dump(diff_tree, diff_tree, &error);
	write_math_expression(diff_tree, &stk, &error);
	//graph_dump(diff_tree, diff_tree, &error);

	stk_dtor(&stk);
	tree_dtor(the_tree.root);
	tree_dtor(diff_tree);
	input_dtor(&base_text);
}