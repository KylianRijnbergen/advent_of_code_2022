#include <stdio.h>
#include <stdlib.h> 
#include "queue.h"

CharQueue* cqueue(void) {
    struct CharQueue* cqueue = malloc(sizeof(CharQueue));
    cqueue->count = 0;
    return cqueue;
}

char pop(CharQueue* cqueue) {
    /* To do */
    return c;

}

void push(CharQueue* cqueue, char c) {
    /* To do */
}

void print_cqueue(CharQueue* cqueue) {
    for (int i = 0; i < cqueue->count; i++) {
        putchar(cqueue->queue[i]);
    }
}