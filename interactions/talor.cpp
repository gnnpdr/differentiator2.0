#include <stdio.h>

#include "talor.h"

static void func_dependency (Node *const node, double var_val, Errors *const error);
static size_t make_fact(size_t num);

Node* make_talor(Node *const root, int decomp_degree, double step, Errors *const error)
{
    assert(root);
    assert(error);

    Node* final_node = nullptr;
    Node* diff = nullptr;

    if (step != 0) //может, стоит сделать эту переменную статической, чтобы не добавлять ее в аргументы функции, а то логически не очень
        diff = diff_node(root, error);
    else
        diff = copy_node(root, error);

    Node* copy_diff = copy_node(diff, error);

    Node* var = copy_node(root->Right, error);

    while(var->Right)
        var = copy_node(var->Right, error); //эта проверка очень глупая, если честно

    Node* pow_num = make_node(NUM, step, nullptr, nullptr, error);
    Node* pow_var = make_node(OP, POW, var, pow_num, error);
    func_dependency (copy_diff, 0, error);

    Node* num_node = make_node(OP, MUL, pow_var, copy_diff, error);

    size_t fact = make_fact(step);
    Node* denom_node = make_node(NUM, (double)fact, nullptr, nullptr, error);
    Node* new_node = make_node(OP, DIV, num_node, denom_node, error);

    decomp_degree--;
    step++;

    if (decomp_degree >= 0)
    {
        Node* new_new_node = make_talor(diff, decomp_degree, step, error);

        Node* sum_node = make_node(OP, ADD, new_node, new_new_node, error);
        new_node = sum_node;
    }

    final_node = new_node;

    return final_node;
}


size_t make_fact(size_t num)
{
    size_t fact = 1;

    for (int i = 1; i <= num; i++)
        fact *= i;

    return fact;
}

void func_dependency (Node *const node, double var_val, Errors *const error)
{
    if (node->Left)
        func_dependency (node->Left, var_val, error);
    
    if (node->Right)
        func_dependency (node->Right, var_val, error);

    if (node->type == VAR)
    {
        node->type = NUM;
        node->value = var_val;
    }
}
