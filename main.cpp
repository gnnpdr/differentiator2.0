#include <stdio.h>

#include "structure\tree_structure.h"
#include "draw\draw_tree.h"
//#include "tree_lib\get_database.h"   //так, ее лучше в отдельный файл, да? Проблема в логике, она не сломается?
#include "interactions\read_task.h"
#include "interactions\write_task.h"

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
	printf("======================================================\n");
	
	Stack stk = {};
	stk_ctor(&stk);
	printf("WRITE\n");
	write_task(&the_tree, &stk);

	tree_dtor(the_tree.top);
	input_dtor(&base_text);
	stk_dtor(&stk);
}