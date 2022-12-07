#ifndef MY_TREE_H
#define MY_TREE_H
#include <stdbool.h>
#include "tree.h"

typedef struct Node Node;

typedef struct Node {
    long long value;
    char* name;
    Node* parent_node;
    int num_children;
    Node** children;
} Node; 

Node* new_node(long long value, char* name);
bool has_children(Node* node);
long long node_value(Node* node);
void print_node(Node* node);
void assign_to_parent(Node* parent, Node* child);
#endif