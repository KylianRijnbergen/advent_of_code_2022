#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main(int argc, char* argv[]) {
    int maxsize = 14;
    CharQueue* queue = cqueue(maxsize);

    FILE* file;
    file = fopen("input.txt", "r");
    char c;
    int count = 0;
    while ((c = fgetc(file)) != '\n') {
        if (queue->count == queue->maxsize) {
            eject(queue);
        }
        push(queue, c);
        if (queue->count == maxsize) {
            if(!has_dupes(queue)) {
                printf("The substring starts at index %d\n", count + queue->maxsize);
                return(0);
            } else {
                count++;            
            }
        }
    }
    return 0;
}



