#include <stdio.h>

#include "structure\tree_structure.h"
#include "draw\draw_tree.h"
//#include "tree_lib\get_database.h"   //так, ее лучше в отдельный файл, да? Проблема в логике, она не сломается?
#include "interactions\read_task.h"
#include "interactions\write_task.h"
#include "interactions\diff.h"

int main(int argc, char** argv)
{
	Input base_text = {};

    input_ctor(&base_text);

    get_database_name(&base_text, argv);
    get_database_text(&base_text);

	Node* top = node_ctor ();

	Tree the_tree = {};
	tree_ctor (&the_tree, top);

    handle_task (&base_text, &the_tree);
	graph_dump(the_tree.top, top);
	
	Stack stk = {};   //можно вложить в макрос, чтобы не надо было кажды раз задавать стэк и удалять его, чтобы написать строчку
	stk_ctor(&stk);
	write_task(the_tree.top, &stk);

	Node* diff_tree = diff_node(the_tree.top);
	graph_dump(diff_tree, diff_tree);
	write_task(diff_tree, &stk);

	stk_dtor(&stk);
	tree_dtor(the_tree.top);
	tree_dtor(diff_tree);
	input_dtor(&base_text);
}