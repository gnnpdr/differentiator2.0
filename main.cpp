#include <stdio.h>

#include "errors\errors.h"
#include "structure\tree_structure.h"
#include "draw\draw_tree.h"
#include "lex_analysis\lex_analysis.h"
#include "syn_analysis\syn_analysis.h"
//#include "interactions\read_math_expression.h"
//#include "interactions\rec_tree_read.h"
#include "interactions\diff.h"
#include "interactions\talor.h"
#include "interactions\write_math_expression.h"
#include "user\user.h"

int main(int argc, char** argv)
{
	ERROR_CTOR

	Input base_text = {};
    input_ctor(&base_text, &error);
	MAIN_CHECK
    get_database_name(&base_text, argv, &error);
	MAIN_CHECK 
    get_database_text(&base_text, &error);
	MAIN_CHECK 

	Node* root = node_ctor();
	MAIN_CHECK  //сделать проверку на nullptr и выделить ей отдельный макрос

	Token* tokens = tokens_ctor(&error);
    MAIN_CHECK 
    Id* ids = id_ctor(&error);
    MAIN_CHECK 

	lex_analysis(tokens, ids, &base_text, &error);
	root = syn_analysis(tokens, &error);

	Tree the_tree = {};

	//root = read_tree_rec(&base_text, &error);
	tree_ctor (&the_tree, root);
	graph_dump(root, root, &error);

	//choose_do_calc(&the_tree, &error);

	tokens_dtor(tokens);
	ids_dtor(ids);
	tree_dtor(the_tree.root);
	input_dtor(&base_text);
}