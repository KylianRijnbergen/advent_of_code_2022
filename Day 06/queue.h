#ifndef MY_QUEUE_H
#define MY_QUEUE_H
#include <stdbool.h>

typedef struct CharNode {
    char value;
    struct CharNode* next;
} CharNode;

typedef struct CharQueue {
    int count;
    int maxsize;
    CharNode* head;
    CharNode* tail;
} CharQueue;

CharQueue* cqueue(int maxsize);
char eject(CharQueue* cqueue);
void push(CharQueue* cqueue, char c);
void print_cqueue(CharQueue* cqueue);
bool has_dupes(CharQueue* cqueue);
#endif