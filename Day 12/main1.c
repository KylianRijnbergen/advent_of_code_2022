#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ROW_LENGTH 8
#define ROWS 5

typedef struct Node Node;
typedef struct Queue Queue;
typedef struct Qnode Qnode;

typedef struct Node {
    int x;
    int y;
    char value;
    bool visited;
    int depth;
    Queue* valid_neighbours;
} Node;

typedef struct Queue {
    int size;
    Qnode* head;
    Qnode* tail;
} Queue;

typedef struct Qnode {
    Node* node;
    Qnode* next;
} Qnode;

void index_to_coord(unsigned int index, int* xloc, int* yloc);
void grid_print(Node** grid);
void node_print(Node* node);
Node* get_start_node(Node** grid);
Node* get_end_node(Node** grid);
Queue* queue_init(void);
void queue_push(Queue* queue, Node* node);
Node* queue_eject(Queue* queue);
void queue_print(Queue* queue);
bool b_is_neighbour_of_a(Node* a, Node* b);

int main(int argc, char* argv[]) {
    /* Init grid */
    const unsigned int NUM_NODES = ROWS * ROW_LENGTH;
    Node** grid = malloc(sizeof(Node*) * NUM_NODES);
    for (unsigned int node = 0; node < NUM_NODES; node++) {
        grid[node] = NULL;
    }

    /* Initialize all nodes in grid */
    FILE* file = fopen("test.txt", "r");
    char c;
    unsigned int current_character = 0;
    while((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            continue;
        }
        Node* node = malloc(sizeof(Node));
        int xloc;
        int yloc;
        index_to_coord(current_character, &xloc, &yloc);
        node->x = xloc;
        node->y = yloc;
        node->value = c;
        node->visited = false;
        node->depth = 0;
        node->valid_neighbours = queue_init();
        grid[current_character] = node;
        current_character++;
    }
    fclose(file);
    
    /* Find start and end nodes */
    Node* start_node = get_start_node(grid);
    Node* end_node = get_end_node(grid);
    node_print(start_node);
    node_print(end_node);

    Queue* queue = queue_init();
    queue_push(queue, start_node);
    queue_push(queue, start_node);
    queue_push(queue, start_node);
    queue_push(queue, start_node);
    queue_print(queue);

    /* Construct Graph by assigning all neighbouring nodes to each other. */
    for (unsigned int rowindex = 0; rowindex < ROWS; rowindex++) {
        for (unsigned int colindex = 0; colindex < ROW_LENGTH; colindex++) {
            Node* current_node = grid[rowindex * ROW_LENGTH + colindex];
            Node* candidate_neighbour = NULL;
            if (rowindex >= 1) {
                candidate_neighbour = grid[(rowindex - 1) * ROW_LENGTH + colindex];
                if (b_is_neighbour_of_a(current_node, candidate_neighbour)) {
                    queue_push(current_node->valid_neighbours, candidate_neighbour);
                }
            }
            if (rowindex < ROWS - 1) {
                candidate_neighbour = grid[(rowindex + 1) * ROW_LENGTH + colindex];
                if (b_is_neighbour_of_a(current_node, candidate_neighbour)) {
                    queue_push(current_node->valid_neighbours, candidate_neighbour);
                }
            }

            if (colindex >= 1) {
                candidate_neighbour = grid[rowindex * ROW_LENGTH + colindex - 1];
                if (b_is_neighbour_of_a(current_node, candidate_neighbour)) {
                    queue_push(current_node->valid_neighbours, candidate_neighbour);
                }
            }
            if (colindex < ROW_LENGTH - 1) {
                candidate_neighbour = grid[rowindex * ROW_LENGTH + colindex + 1];
                if (b_is_neighbour_of_a(current_node, candidate_neighbour)) {
                    queue_push(current_node->valid_neighbours, candidate_neighbour);
                }
            }
        }
    }

    printf("Printing valid neighbours: \n");
    queue_print(end_node->valid_neighbours);
    return 0;
}

void index_to_coord(unsigned int index, int* xloc, int* yloc) {
    *xloc = index % ROW_LENGTH;
    *yloc = index / ROW_LENGTH;
}

void grid_print(Node** grid) {
    for (unsigned int i = 0; i < ROWS; i++) {
        for (unsigned int j = 0; j < ROW_LENGTH; j++) {
            node_print(grid[i * ROW_LENGTH + j]);
        }
        printf("\n");
    }
}

void node_print(Node* node) {
    printf("X: %d, Y: %d, Value: %c\n", node->x, node->y, node->value);
}

Node* get_start_node(Node** grid) {
    Node* start_node = NULL;
    for (unsigned int i = 0; i < ROWS; i++) {
        for (unsigned int j = 0; j < ROW_LENGTH; j++) {
            if (grid[i * ROW_LENGTH + j]->value == 'S') {
                return grid[i * ROW_LENGTH + j];
            }
        }
    }
    printf("Start node not found!\n");
}

Node* get_end_node(Node** grid) {
    Node* end_node = NULL;
    for (unsigned int i = 0; i < ROWS; i++) {
        for (unsigned int j = 0; j < ROW_LENGTH; j++) {
            if (grid[i * ROW_LENGTH + j]->value == 'E') {
                return grid[i * ROW_LENGTH + j];
            }
        }
    }
    printf("End node not found!\n");
}

Queue* queue_init(void) {
    Queue* queue = malloc(sizeof(Queue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void queue_push(Queue* queue, Node* node) {
    /* Create node */
    Qnode* queue_node = malloc(sizeof(Qnode));
    queue_node->node = node;
    queue_node->next = NULL;

    /* Update tail */
    if (queue->tail != NULL) {
        queue->tail->next = queue_node;
    }
    queue->tail = queue_node;

    /* Update head */
    if (queue->head == NULL) {
        queue->head = queue_node;
    }
    queue->size++;
}

Node* queue_eject(Queue* queue) {
    if (queue->head == NULL) {
        printf("Error: Eject from empty queue\n");
    }
    Qnode* tmp = queue->head;
    Node* ejection_node = tmp->node;
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    free(tmp);
    queue->size--;
    return ejection_node;
}

void queue_print(Queue* queue) {
    Qnode* curr = queue->head;
    while(curr != NULL) {
        node_print(curr->node);
        curr = curr->next;
    }
}

bool b_is_neighbour_of_a(Node* a, Node* b) {
    bool neighbouring_chars = (b->value - a->value) <= 1;
    bool start_and_a = (a->value == 'S' && b->value == 'a') || (a->value == 'a' && b->value == 'S');
    bool end_and_z = (a->value == 'E' && b->value == 'z') || (a->value == 'z' && b->value == 'E');
    return neighbouring_chars  || start_and_a || end_and_z; 
}