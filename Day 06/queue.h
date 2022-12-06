#ifndef MY_QUEUE_H
#define MY_QUEUE_H
#define MAX_QUEUE_SIZE 4
typedef struct CharQueue {
    int count;
    char queue[MAX_QUEUE_SIZE];
} CharQueue;

CharQueue* cqueue(void);
char eject(CharQueue* cstack);
void push(CharQueue* cqueue, char c);
void print_cqueue(CharQueue* cqueue);
#endif