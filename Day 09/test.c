#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HASHMAP_X 10
#define HASHMAP_Y 10

#define MAX_LINE_LENGTH 10

typedef struct Coord Coord;
typedef struct HashNode HashNode;
typedef struct HashMap HashMap;

typedef void (*operation)(Coord* coord);

typedef struct Coord {
    int x;
    int y;
} Coord;

typedef struct HashNode {
    Coord* self;
    HashNode* next;
} HashNode;

typedef struct HashMap {
    HashNode** nodes;
} HashMap;


void print_coord(Coord* coord);
HashNode* hash(Coord* coord);
bool is_touching(Coord* tail, Coord* head);
void up(Coord* head);
void down(Coord* head);
void left(Coord* head);
void right(Coord* head);
void update_tail(Coord* head, Coord* tail);

int main(int argc, char* argv[]) {
    /* Initialize head and Tail */
    Coord* head;
    head->x = 0; 
    head->y = 0;
    Coord* tail;
    tail->x = 0;
    tail->y = 0;
    
    printf("Hello!\n");
    return 0;
}

void print_coord(Coord* coord) {
    printf("Coordinate is (%d, %d). \n", coord->x, coord->y);
}

int pos_mod(int value, int mod) {
    return (value % mod + mod) % mod;
}

HashNode* hash(Coord* coord) {
    HashNode* hash = malloc(sizeof(HashNode));
    hash->self = malloc(sizeof(Coord));
    hash->self->x = pos_mod(coord->x, HASHMAP_X);
    hash->self->y = pos_mod(coord->y, HASHMAP_Y);
    hash->next = NULL;
    return hash;
}

bool is_touching(Coord* tail, Coord* head) {
    int xdiff = abs(tail->x - head->x);
    int ydiff = abs(tail->y - head->y);
    return xdiff <=1 && ydiff <= 1;
}

void up(Coord* head) {
    (head->y)++;
}

void down(Coord* head) {
    (head->y)--;
}

void left(Coord* head) {
    (head->x)--;
}

void right(Coord* head) {
    (head->x)++;
}

void update_tail(Coord* head, Coord* tail) {
    int xdiff = tail->x - head->x;
    int ydiff = tail->y - head->y;
    
    /* If x is the same, the movement is only vertical */ 
    if (xdiff == 0 && ydiff > 0) {
        (tail->y)--;
    } 
    if (xdiff == 0 && ydiff < 0) {
        (tail->y)++;
    } 

    /* Same for y, horizontal movement */
    if (ydiff == 0 && xdiff > 0) {
        (tail->x)--;
    }
    if (ydiff == 0 && xdiff < 0) {
        (tail->x)++;
    }

    if (xdiff > 0 && ydiff > 0) {
        (tail->x)--;
        (tail->y)--;
    }
    
    if (xdiff < 0 && ydiff < 0) {
        (tail->x)++;
        (tail->y)++;  
    }
}