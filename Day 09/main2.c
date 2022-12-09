#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HASHMAP_X 50
#define HASHMAP_Y 50

#define MAX_LINE_LENGTH 10
#define KNOTS 10

typedef struct Coord Coord;
typedef struct Entry Entry;
typedef struct HashMap HashMap;

typedef void (*operation)(Coord* coord);

typedef struct Coord {
    int x;
    int y;
} Coord;

typedef struct Entry {
    Coord* coord;
    Entry* next;
} Entry;

typedef struct HashMap {
    Entry*** entries;
} HashMap;


void print_coord(Coord* coord);
bool is_touching(Coord* tail, Coord* head);
void up(Coord* head);
void down(Coord* head);
void left(Coord* head);
void right(Coord* head);
bool update_tail(Coord* head, Coord* tail);
Coord* coord_copy(Coord* coord);
HashMap* hm_create(void);
void hash(Coord* coord, int* xloc, int* yloc);
bool hashtable_set(HashMap* hashmap, Coord* coord);
Entry* ht_append(Coord* coord);
Entry* ht_get(HashMap* hashtable, Coord* coord);
bool coord_equal(Coord* coord_a, Coord* coord_b);


int main(int argc, char* argv[]) {
    int score_counter = 0;
    /* Initialize head, knots, and Tail */
    Coord* knots[KNOTS];
    for (int i = 0; i < KNOTS; i++) {
        knots[i] = malloc(sizeof(Coord));
        knots[i]->x = 0;
        knots[i]->y = 0;
    }
    
    /* Init HashMap */
    HashMap* hashmap;
    hashmap = hm_create();


    /* Start logic */
    FILE* file;
    file = fopen("input.txt", "r");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        operation op;
        if (line[0] == 'U') {
            op = &up;
        } else if (line[0] == 'R') {
            op = &right;
        } else if (line[0] == 'D') {
            op = &down;
        } else if (line[0] == 'L') {
            op = &left;
        }

        char buff[3];
        buff[0] = line[2];
        if (line[3] == '\n') {
            buff[1] = '\0';
        } else {
            buff[1] = line[3];
        }
        buff[2] = '\0';

        int count = atoi(buff);
        for (int i = 0; i < count; i++) {
            op(knots[0]);
            for (int knotindex = 1; knotindex < KNOTS; knotindex++) {
                update_tail(knots[knotindex - 1], knots[knotindex]);
            }
            if(hashtable_set(hashmap, knots[KNOTS-1])) {
                score_counter++;
            }
        }
    }
    printf("Score is %d\n", score_counter);
    return 0;
}

void print_coord(Coord* coord) {
    printf("Coordinate is (%d, %d). \n", coord->x, coord->y);
}

int pos_mod(int value, int mod) {
    return (value % mod + mod) % mod;
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

bool update_tail(Coord* head, Coord* tail) {
    if(is_touching(head, tail)) {
        return false;
    }

    int xdiff = tail->x - head->x;
    int ydiff = tail->y - head->y;
    
    /* If x is the same, the movement is only vertical */ 
    if (ydiff > 0) {
        (tail->y)--;
    } 
    if (ydiff < 0) {
        (tail->y)++;
    } 

    /* Same for y, horizontal movement */
    if (xdiff > 0) {
        (tail->x)--;
    }
    if (xdiff < 0) {
        (tail->x)++;
    }
    return true;
}

Coord* coord_copy(Coord* coord) {
    Coord* new_coord = malloc(sizeof(Coord));
    new_coord->x = coord->x;
    new_coord->y = coord->y;
    return new_coord;
}

HashMap* hm_create(void) {
    HashMap* hashtable = malloc(sizeof(HashMap) * 1);
    hashtable->entries = malloc(sizeof(Entry**) * HASHMAP_Y);
    for (int i = 0; i < HASHMAP_Y; i++) {
        hashtable->entries[i] = malloc(sizeof(Entry*) * HASHMAP_X);
    }
    for (int i = 0; i < HASHMAP_Y; i++) {
        for (int j = 0; j < HASHMAP_Y; j++) {
            hashtable->entries[i][j] = NULL;
        }
    }
    return hashtable;
}

void hash(Coord* coord, int* xloc, int* yloc) {
    *xloc = pos_mod(coord->x, HASHMAP_X);
    *yloc = pos_mod(coord->y, HASHMAP_Y);
}

bool hashtable_set(HashMap* hashmap, Coord* coord) {
    int xloc, yloc;
    hash(coord, &xloc, &yloc);

    Entry* entry = hashmap->entries[xloc][yloc];

    if (entry == NULL) {
        hashmap->entries[xloc][yloc] = ht_append(coord);
        return true;
    }

    Entry* previous;

    while (entry != NULL) {
        if (coord_equal(coord, entry->coord)) {
            //Match found return;
            return false; 
        }

        // Otherwise, go to next
        previous = entry;
        entry = previous->next;

    }
    previous->next = ht_append(coord);
    return true;

}

Entry* ht_append(Coord* coord) {
    Entry* entry = malloc(sizeof(Entry) * 1);
    entry->coord = malloc(sizeof(Coord*) * 1);
    entry->coord = coord_copy(coord);
    entry->next = NULL;
    return entry;
}

bool coord_equal(Coord* coord_a, Coord* coord_b) {
    return coord_a->x == coord_b->x && coord_a->y == coord_b->y;
}