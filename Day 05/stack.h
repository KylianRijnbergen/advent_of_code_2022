#ifndef MY_STACK_H
#define MY_STACK_H
#define MAX_STACK_SIZE 256
typedef struct CharStack {
    int count;
    char stack[MAX_STACK_SIZE];
} CharStack;

CharStack* cstack(void);
char pop(CharStack* cstack);
void push(CharStack* cstack, char c);
void print_cstack(CharStack* cstack);
#endif