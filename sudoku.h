#ifndef SUDOKU_H
#define SUDOKU_H

typedef struct sudoku sudoku;
typedef struct sudoku_node sudoku_node;

struct sudoku {
    sudoku_node** arr;
};

struct sudoku_node {
    int id; // unique id for each node

    // stores data
    // if data is 0 means it is not solved yet
    int data;

    sudoku_node* next; // next sudoku node
    sudoku_node* prev; // prev sudoku node
    
    int is_solved; // indicates whether it is already there

    // if it is solved, array is full of zeroes
    int possible_solution[9];

    // there are 20 neighbours for each node
    sudoku_node* neighbours;
};

#endif