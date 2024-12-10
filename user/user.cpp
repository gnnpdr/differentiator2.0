#include <stdio.h>

#include "user.h"

static int find_action (char *const str);

void choose_do_calc(Tree* the_tree, Err_param* error)
{
    Stack stk = {};
	stk_ctor(&stk, error);

    printf("what do you want?\n");  //ну или можно загрузить в командную строку
	char answer[MAX_STR_LEN] = {};
	scanf("%s", answer);

	int action = find_action (answer);
    if (action == ERROR_VALUE)
    ERROR(SYN_ERROR)

	Node* res_tree = nullptr;
	if (action == DIFF)  //можно ещ едобавить вопрос про количество повторений дифференцирования и 
	{
		res_tree = diff_node(the_tree->root, error);
	}
	else if (action == TALOR)
	{
        printf("degree of decomposition?\n");
        int dec_deg = 0;
        scanf("%d", &dec_deg);

		double denom = 0; //заменить denom на константу типа start denom
		res_tree = make_talor(the_tree->root, dec_deg, denom, error);
	}
    else if (action == KENNEDY)
        printf("do you really want to know who killed kennedy??\nso, i ll tell you\nit was cuno. but cuno doesnt even care\n");

	res_tree = write_math_expression(res_tree, &stk, error);
	graph_dump(res_tree, res_tree, error);

    tree_dtor(res_tree);
    stk_dtor(&stk);
}

int find_action (char *const str)
{
    assert(str);

    int res = ERROR_VALUE;

    int cmp_talor = strncmp(str, DO_TALOR, MAX_STR_LEN);
    int cmp_diff = strncmp(str, DO_DIFF, MAX_STR_LEN);
    int cmp_kennedy = strncmp(str, "kennedy", MAX_STR_LEN);

    if (cmp_talor == 0)
        res = TALOR;
    else if (cmp_diff == 0)
        res = DIFF;
    else if (cmp_kennedy == 0)
        res = KENNEDY;

    return res;
}