#include <stdio.h>


#include "C:\Users\User\Desktop\differentiator2.0\structure\tree_structure.h"
#include "C:\Users\User\Desktop\differentiator2.0\draw\draw_tree.h"
//#include "C:\Users\User\Desktop\tree\get_database.h"   //так, ее лучше в отдельный файл, да? Проблема в логике, она не сломается?
#include "C:\Users\User\Desktop\differentiator2.0\interactions\interactions.h"

int main(int argc, char** argv)
{
	Input base_text = {};

    input_ctor(&base_text);

    get_database_name(&base_text, argv);

    get_database_text(&base_text);
	printf("HELLO\n");

	Node* top = node_ctor ();

	Tree the_tree = {};
	tree_ctor (&the_tree, top);
	printf("NOT SO GOOD\n");
    handle_task (&base_text, &the_tree);
	/*int x = 0;
	Node* top = DIV(ADD(X, NUM(3)), SUB(NUM(1000), NUM(7)));
	the_tree.top = top;*/

	graph_dump(the_tree.top, top);



	tree_dtor(the_tree.top);
	input_dtor(&base_text);
}