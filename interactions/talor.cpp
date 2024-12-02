#include <stdio.h>

#include "talor.h"

static void func_dependency (Node *const node, double var_val, Errors *const error);

Node* make_talor(Node *const root, size_t decomp_degree, Errors *const error)
{
    //так то по факту маклорен подразумевается, иначе надо другую типа переменную вводить. Можно конечно, но к черту
    assert(root);
    assert(error);

    double denom = 1;

    Node* final_node = nullptr;

    Node* diff = diff_node(root, error);
    Node* copy_diff = copy_node(diff, error);

    Node* var = copy_node(root->Right, error);  //так то это не очень подходит, тут надо будет разбираться дальше

    func_dependency (copy_diff, 0, error);

    Node* num_node = make_node(OP, MUL, var, copy_diff, error);
    Node* denom_node = make_node(NUM, denom, nullptr, nullptr, error);
    Node* new_node = make_node(OP, DIV, num_node, denom_node, error);

    for (size_t num = 0; num < decomp_degree; num++)
    {
        denom *= num;

        diff = diff_node(diff, error);

        var = copy_node(root->Right, error);
        
        copy_diff = copy_node(diff, error);

        func_dependency (copy_diff, 0, error);

        num_node = make_node(OP, MUL, var, copy_diff, error);
        denom_node = make_node(NUM, denom, nullptr, nullptr, error);
        Node* new_new_node = make_node(OP, DIV, num_node, denom_node, error);

        final_node = make_node(OP, ADD, new_node, new_new_node, error);

        new_node = new_new_node;
    }

    if (final_node)
        return final_node;
    else
        return new_node;
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
