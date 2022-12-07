#include <stdio.h>
#include "tree.h"

int main (int argc, char* argv[]) {
    FILE* file;
    file = fopen("input.txt", "r");

    Node* root_node = new_node(123456, "Root");
    print_node(root_node);

    Node* child_node1 = new_node(23, "Child1");
    Node* child_node2 = new_node(123, "Child2");
    assign_to_parent(root_node, child_node1);
    assign_to_parent(root_node, child_node2);

    print_node(root_node);    

    return 0;
}