#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ROW_LENGTH  162
#define ROWS 41

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
Queue* queue_concat(Queue* head_queue, Queue* tail_queue);
Node* queue_eject(Queue* queue);
void queue_print(Queue* queue);
bool b_is_neighbour_of_a(Node* a, Node* b);
void generate_neighbours(Node** grid);
unsigned int BFS(Node* start_node, Node* end_node);

int main(void) {
    /* Init grid */
    const unsigned int NUM_NODES = ROWS * ROW_LENGTH;
    Node** grid = (Node**)malloc(sizeof(Node*) * NUM_NODES);
    for (unsigned int node = 0; node < NUM_NODES; node++) {
        grid[node] = NULL;
    }

    /* Initialize all nodes in grid */
    FILE* file = fopen("input.txt", "r");
    char c;
    unsigned int current_character = 0;
    while((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            continue;
        }
        Node* node = (Node*)malloc(sizeof(Node));
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
    /* Construct Graph by assigning all neighbouring nodes to each other. */
    generate_neighbours(grid);
    unsigned int final_depth = BFS(start_node, end_node);

    printf("Fewest required steps is %d\n", final_depth);

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
    printf("X: %d, Y: %d, Value: %c, Depth: %d\n", node->x, node->y, node->value, node->depth);
}

Node* get_start_node(Node** grid) {
    for (unsigned int i = 0; i < ROWS; i++) {
        for (unsigned int j = 0; j < ROW_LENGTH; j++) {
            if (grid[i * ROW_LENGTH + j]->value == 'S') {
                return grid[i * ROW_LENGTH + j];
            }
        }
    }
    printf("Start node not found!\n");
    exit(1);
}

Node* get_end_node(Node** grid) {
    for (unsigned int i = 0; i < ROWS; i++) {
        for (unsigned int j = 0; j < ROW_LENGTH; j++) {
            if (grid[i * ROW_LENGTH + j]->value == 'E') {
                return grid[i * ROW_LENGTH + j];
            }
        }
    }
    printf("End node not found!\n");
    exit(1);
}

Queue* queue_init(void) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void queue_push(Queue* queue, Node* node) {
    /* Create node */
    Qnode* queue_node = (Qnode*)malloc(sizeof(Qnode));
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

Queue* queue_concat(Queue* head_queue, Queue* tail_queue) {
    /* If head queue is empty, return tail queue */
    if (head_queue->head == NULL) {
        head_queue->head = tail_queue->head;
        head_queue->tail = tail_queue->tail;
        free(tail_queue);
        return head_queue;
    }
    /* If tail queue is empty, return head queue */
    if (tail_queue->head == NULL) {
        return head_queue;
    }

    /* If head neither head_queue nor tail_queue are empty, we need to connect the queues. 
    To do this, we need to let the tail of the head queue point to the head of the tail queue. 
    Also, we need to adjust the size of the queue. 
    Lastly, we need to free the tail_queue. */

    head_queue->tail->next = tail_queue->head;
    head_queue->size += tail_queue->size;
    free(tail_queue);
    return head_queue;
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
    bool neighbouring_chars = ((b->value - a->value) <= 1 && (!(b->value == 'E')));
    bool start_and_a = (a->value == 'S' && b->value == 'a') || (a->value == 'a' && b->value == 'S');
    bool end_and_z = (a->value == 'E' && b->value == 'z') || (a->value == 'z' && b->value == 'E');
    return neighbouring_chars || start_and_a || end_and_z; 
}

/* Construct Graph by assigning all neighbouring nodes to each other. */
void generate_neighbours(Node** grid) {
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
}

unsigned int BFS(Node* start_node, Node* end_node) {
    start_node->visited = true;
    Queue* priority_queue_current = queue_init(); /* Create empty queue */
    Queue* priority_queue_next = queue_init();
    Node* current_node = start_node;
    /* Keep track of which node is the start node */
    unsigned int current_depth = 0; /* Set initial depth to 1, such that we can update the depth of all nodes */
    
    /* We keep iterating over our nodes until we reach our destination */
    while (current_node != end_node) {
        /* Set depth of current node */
        /* Then, we add all unvisited nodes to the back of our priority queue, */
        Qnode* tmp_qnode = current_node->valid_neighbours->head;
        while (tmp_qnode != NULL) {
            Node* tmp_node = tmp_qnode->node;
            if (tmp_node->visited == false && b_is_neighbour_of_a(current_node, tmp_node)) {
                tmp_node->visited = true;
                if (tmp_node->value == 'a') {
                    tmp_node->depth = 0;
                } else {
                    tmp_node->depth = current_node->depth + 1;
                }
                queue_push(priority_queue_next, tmp_node);;
            }
            tmp_qnode = tmp_qnode->next;
        }

        /* If there are nodes left, we want to make the next node in our queue the current node */
        if (priority_queue_current->size == 0) {
            /* If there are no nodes left of the current level, we increase depth. */
            current_depth++;
            Queue* tmp = priority_queue_current;
            priority_queue_current = priority_queue_next;
            priority_queue_next = tmp;
        }
        current_node = queue_eject(priority_queue_current);
    }
    /* We now have our end node. We need to add 1 to get the correct depth. */
    /* Once we have reached our destination, we wish to return depth. */
    return end_node->depth;
}
