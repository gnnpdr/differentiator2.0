#include <stdio.h>

#include "structure\tree_structure.h"
#include "draw\draw_tree.h"
#include "interactions\read_math_expression.h"
#include "interactions\rec_tree_read.h"
#include "interactions\diff.h"
#include "interactions\talor.h"
#include "interactions\write_math_expression.h"
#include "user\user.h"

int main(int argc, char** argv)
{
	Errors error = ALL_RIGHT;
	Input base_text = {};

    input_ctor(&base_text, &error);

    get_database_name(&base_text, argv, &error);
    get_database_text(&base_text, &error);

	Node* root = node_ctor (&error);

	Tree the_tree = {};

	root = read_tree_rec(&base_text, &error);
	tree_ctor (&the_tree, root);

	choose_do_calc(&the_tree, &error);

	tree_dtor(the_tree.root);
	
	input_dtor(&base_text);
}