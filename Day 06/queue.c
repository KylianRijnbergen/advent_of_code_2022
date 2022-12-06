#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include "queue.h"

CharQueue* cqueue(int maxsize) {
    struct CharQueue* cqueue = malloc(sizeof(CharQueue));
    cqueue->count = 0;
    cqueue->maxsize = maxsize;
    cqueue->head = NULL;
    cqueue->tail = NULL;
    return cqueue;
}

char eject(CharQueue* cqueue) {
    if (cqueue->head == NULL) {
        printf("Error: Eject from empty queue\n");
    }
    CharNode* tmp = cqueue->head;
    char result = tmp->value;
    cqueue->head = cqueue->head->next;
    if (cqueue->head == NULL) {
        cqueue->tail = NULL;
    }
    free(tmp);
    cqueue->count -= 1;
    return result;

}

void push(CharQueue* cqueue, char c) {
    if (cqueue->count == cqueue->maxsize) {
        printf("Max queue size reached. Element not added."); 
        return;
    }
    CharNode* new_node = malloc(sizeof(CharNode));
    new_node->value = c;
    new_node->next = NULL;
    if (cqueue->tail != NULL) {
        cqueue->tail->next = new_node;
    }
    cqueue->tail = new_node;
    if(cqueue->head == NULL) {
        cqueue->head = new_node;
    }
    cqueue->count += 1;
}

void print_cqueue(CharQueue* cqueue) {
    printf("Queue is: ");
    CharNode* tmp = cqueue->head;
    for (unsigned int i = 0; i < cqueue->count; i++) {
        printf("%c, ", tmp->value);
        tmp = tmp->next;
    }
    printf("\n");
}

bool has_dupes(CharQueue* cqueue) {
    CharNode* curr_node = cqueue->head;
    bool seen[256];
    for (int i = 0; i < 256; i++) {
        seen[i] = false;
    }

    for (int i = 0; i < cqueue->count; i++) {
        if (seen[curr_node->value]) {
            return true;
        }
        seen[curr_node->value] = true;
        if (curr_node->next != NULL) {
            curr_node = curr_node->next;
        }
    }
    return false;
}