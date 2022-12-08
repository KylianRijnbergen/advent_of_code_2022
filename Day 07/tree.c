#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

Node* new_node(long long value, char* name) {
    Node* node = malloc(sizeof(Node));
    node->value = value;
    node->name = malloc(sizeof(char) * (strlen(name) + 1)); /* Add 1 for NULL terminator */
    node->name = name;
    node->parent_node = NULL;
    node->num_children = 0;
    node->children = malloc(sizeof(Node*) * node->num_children);
}

bool has_children(Node* node) {
    return node->children == NULL; 
}

long long node_value(Node* node) {
    return node->value;
}

void print_node(Node* node) {
    printf("Printing node:\n");
    printf("Node name is %s\n", node->name);
    printf("Node value is %lld\n", node->value);
    if (!has_children(node)) {
        printf("This node has %lld children.\n", node->num_children);
        for (int i = 0; i < node->num_children; i++) {
            printf("Child is : %s\n", node->children[i]->name);
        }
    } else {
        printf("This node does not have any children.\n");
    }
}

void assign_to_parent(Node* parent, Node* child) {
    parent->num_children += 1;
    parent->children = realloc(parent->children, sizeof(Node*) * parent->num_children);
    parent->children[parent->num_children - 1] = child;
    child->parent_node = parent;
}

Node* build_tree_from_file(FILE* file) {
    char line[50];
    Node* tree = NULL;
    while (fgets(line, 50, file) != NULL) {
        int x;
    }
}

Node* create_root_node(void) {
    Node* root = new_node(0, "/");
    return root;
}