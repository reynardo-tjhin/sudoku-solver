#ifndef SUDOKU_H
#define SUDOKU_H

#define NUMBER_OF_NEIGHBOURS 20

typedef struct sudoku sudoku;
typedef struct sudoku_node sudoku_node;

struct sudoku_node {
    int id; // unique id for each node

    // stores data
    // if data is 0 means it is not solved yet
    int data;

    struct sudoku_node* next; // next sudoku node
    struct sudoku_node* prev; // prev sudoku node
    
    int is_solved; // indicates whether it is already there

    // if it is solved, array is full of zeroes
    int* possible_solutions; // [ 1, 2, 3, 4, 5, 6, 7, 8, 9 ]

    // there are 20 neighbours for each node
    struct sudoku_node** neighbours;
};

#endif