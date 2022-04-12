#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"

#define MAX_BUFFER 1000

extern char *strsep(char **stringp, const char *delim);

void free_sudoku(sudoku_node** sudoku) {
    for (int i = 0; i < 81; i++) {
        sudoku_node* sudoku_node = sudoku[i];
        free(sudoku_node);
    }
    free(sudoku);
}

int main(int argc, char** argv) {

    // check whether there is a file
    if (argc < 2) {
        printf("Error: Please provide a file with sudoku in it.\n");
        return 1;
    }

    // get the file
    FILE* fpointer = fopen(argv[1], "r");
    if (fpointer == NULL) {
        printf("Error: File does not exist.\n");
        return 1;
    }

    // create a sudoku array of pointers
    sudoku_node** sudoku = (struct sudoku_node **) malloc(sizeof(struct sudoku_node *) * 81);
    if (sudoku == NULL) {
        printf("Error: Fails to malloc.\n");
        return 1;
    }

    // read from the file and create the sudoku nodes
    char buffer[MAX_BUFFER] = { '\0' };
    int id = 0; // to track the nodes
    while (fgets(buffer, MAX_BUFFER, fpointer) != NULL) {

        // get the length of the buffer
        size_t size_of_buffer = strlen(buffer);

        // check for new line character at the end and remove it
        if (buffer[size_of_buffer - 1] == '\n') {
            buffer[size_of_buffer - 1] = '\0';
            size_of_buffer -= 1;
        }

        // separate the strings with " " and create sudoku nodes
        char* duplicate_buffer = (char *) malloc(sizeof(char) * MAX_BUFFER);
        // copy the content of buffer to `duplicate_buffer`
        strcpy(duplicate_buffer, buffer);
        // get another copy to prevent memory leaks later
        char* another_copy = duplicate_buffer;
        char* number; // to store the number here
        while ((number = strsep(&another_copy, " ")) != NULL) {

            // convert the number
            int data = atoi(number);

            // in case it is not a number or a negative number
            if ((strcmp(number, "0") && data == 0) || (data < 0)) {
                free(duplicate_buffer);
                free_sudoku(sudoku);
                printf("Error: There is an error reading the data in the file.\n");
                return 1;
            }

            // create the sudoku node here
            sudoku_node* sudoku_node = (struct sudoku_node *) malloc(sizeof(struct sudoku_node));
            sudoku_node->id = id;
            sudoku_node->data = data;
            if (data != 0) { // means the node is already solved
                sudoku_node->is_solved = 1;
            } else {
                sudoku_node->is_solved = 0;
            }
            printf("%d ", data);

            // store the sudoku address to the sudoku (array)
            sudoku[id] = sudoku_node;

            id += 1;
        }
        printf("\n");

        free(duplicate_buffer);
    }

    // update the next, prev and neighbour attributes of each sudoku node in the sudoku
    for (int i = 0; i < 81; i++) {

        // get the current node of the sudoku
        sudoku_node* current_sudoku_node = sudoku[i];

        // update the next and prev attributes
        if (i == 0) { // start of the node
            current_sudoku_node->next = sudoku[i + 1];
            current_sudoku_node->prev = NULL;
        }
        else if (i == 80) { // end of the node
            current_sudoku_node->next = NULL;
            current_sudoku_node->prev = sudoku[i - 1];
        }
        else { // middle nodes
            current_sudoku_node->next = sudoku[i + 1];
            current_sudoku_node->prev = sudoku[i - 1];
        }

        // update the neighbour of the sudoku
        // horizontal, vertical, box
        // for example, the id: 0, neighbours: [1 to 8], [9, 18, 27, ..., 72], [1, 2, ]

    }

    // find the solution
    

    // temporary print
    printf("sudoku.c: Success!\n");

    free_sudoku(sudoku);

    return 0;
}