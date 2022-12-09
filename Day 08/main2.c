#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define ROW_SIZE 99
#define COL_SIZE 99

typedef struct Tree {
    int height;
    int view_range_n;
    int view_range_e;
    int view_range_s;
    int view_range_w;
} Tree;

typedef struct Forest {
    int visible_trees;
    Tree** trees;
} Forest;

void load_forest(Forest* forest, int rows, int cols);
long long get_view_range(Forest* forest, int rowindex, int colindex);
int get_view_n(Forest* forest, int rowindex, int colindex);
int get_view_e(Forest* forest, int rowindex, int colindex);
int get_view_s(Forest* forest, int rowindex, int colindex);
int get_view_w(Forest* forest, int rowindex, int colindex);


int main(int argc, char* argv[]) {
    int row_size = ROW_SIZE;
    int column_size = COL_SIZE;

    Forest* forest;
    forest = malloc(sizeof(Forest));
    forest->visible_trees = 0; 
    forest->trees = malloc(sizeof(Tree*) * column_size);

 
    for (unsigned int i = 0; i < row_size; i++) {
        for (unsigned int j = 0; j < column_size; j++) {
            forest->trees[i] = malloc(sizeof(Tree) * row_size);
        }
    }

    load_forest(forest, row_size, column_size);
    long long best_score = 0;
    long long current = 0;
    for (int i = 0; i < ROW_SIZE; i++) {
        for (int j = 0; j < COL_SIZE; j++) {
            current = get_view_range(forest, i, j);
            if (current > best_score) {
                best_score = current;
            }
        }
    }
    printf("Best score was %lld\n", best_score);
    
    return 0;
}

void load_forest(Forest* forest, int rows, int cols) {
    FILE* file;
    file = fopen("input.txt", "r");
    int counter = 0;
    int row, column;
    char c; 
    while((c = fgetc(file)) != EOF) {
        if (c != '\n') {
            row = counter / cols;
            column = counter % rows;
            forest->trees[row][column].height = c - 48;
            forest->trees[row][column].view_range_n = 0;
            forest->trees[row][column].view_range_e = 0;
            forest->trees[row][column].view_range_s = 0;
            forest->trees[row][column].view_range_w = 0;
            counter++;
        }
    }
    fclose(file);
}

long long get_view_range(Forest* forest, int rowindex, int colindex) {
    int n, e, s, w, result;
    n = get_view_n(forest, rowindex, colindex);
    e = get_view_e(forest, rowindex, colindex);
    s = get_view_s(forest, rowindex, colindex);
    w =  get_view_w(forest, rowindex, colindex);
    result = n * e * s * w;
    return result;
}

int get_view_n(Forest* forest, int rowindex, int colindex) {
    if (rowindex == 0) {
        return 0;
    }
    int viewrange = 0;
    int own_height = forest->trees[rowindex][colindex].height;

    for (int i = rowindex; i > 0; i--) {
        int next_tree = forest->trees[i - 1][colindex].height;
        if (next_tree < own_height) {
            viewrange++;
        } else {
            return viewrange + 1;
        }
    }
    return viewrange;
}

int get_view_w(Forest* forest, int rowindex, int colindex) {
    if (colindex == 0) {
        return 0;
    }
    int viewrange = 0;
    int own_height = forest->trees[rowindex][colindex].height;

    for (int i = colindex; i > 0; i--) {
        int next_tree = forest->trees[rowindex][i - 1].height;
        if (next_tree < own_height) {
            viewrange++;
        } else {
            return viewrange + 1;
        }
    }
    return viewrange;
}

int get_view_s(Forest* forest, int rowindex, int colindex) {
    if (rowindex == COL_SIZE - 1) {
        return 0;
    }
    int viewrange = 0;
    int own_height = forest->trees[rowindex][colindex].height;

    for (int i = rowindex + 1; i < COL_SIZE; i++) {
        int next_tree = forest->trees[i][colindex].height;
        if (next_tree < own_height) {
            viewrange++;
        } else {
            return viewrange + 1;
        }
    }
    return viewrange;
}

int get_view_e(Forest* forest, int rowindex, int colindex) {
    if (colindex == ROW_SIZE - 1) {
        return 0;
    }
    int viewrange = 0;
    int own_height = forest->trees[rowindex][colindex].height;

    for (int i = colindex + 1; i < ROW_SIZE; i++) {
        int next_tree = forest->trees[rowindex][i].height;
        if (next_tree < own_height) {
            viewrange++;
        } else {
            return viewrange + 1;
        }
    }
    return viewrange;
}