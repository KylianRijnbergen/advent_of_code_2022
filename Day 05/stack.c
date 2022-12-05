#include <stdio.h>
#include <stdlib.h> 
#include "stack.h"

CharStack* cstack(void) {
    struct CharStack* cstack = malloc(sizeof(CharStack));
    cstack->count = 0;
    return cstack;
}

char pop(CharStack* cstack) {
    char c = cstack->stack[cstack->count - 1];
    cstack->count--;
    return c;

}

void push(CharStack* cstack, char c) {
    cstack->stack[cstack->count] = c;
    cstack->count++;
}

void print_cstack(CharStack* cstack) {
    for (int i = 0; i < cstack->count; i++) {
        putchar(cstack->stack[i]);
    }
}