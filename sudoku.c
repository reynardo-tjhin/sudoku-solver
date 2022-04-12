#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"

#define MAX_BUFFER 1000

extern char *strsep(char **stringp, const char *delim);

// free the sudoku that was malloc'ed
void free_sudoku(sudoku_node** sudoku) {
    for (int i = 0; i < 81; i++) {
        sudoku_node* sudoku_node = sudoku[i];
        free(sudoku_node->neighbours);
        if (sudoku_node->possible_solutions != NULL) {
            free(sudoku_node->possible_solutions);
        }
        free(sudoku_node);
    }
    free(sudoku);
}

// print the sudoku with the current solution
void print_sudoku(sudoku_node** sudoku) {
    sudoku_node* current_sudoku_node = sudoku[0];
    while (current_sudoku_node != NULL) {
        if ((current_sudoku_node->id + 1) % 9 == 0) {
            printf("%d\n", current_sudoku_node->data);
        } else {
            printf("%d ", current_sudoku_node->data);
        }
        current_sudoku_node = current_sudoku_node->next;
    }
}

void print_neighbour(sudoku_node** sudoku) {
    for (int i = 0; i < 81; i++) {
        sudoku_node* current_sudoku_node = sudoku[i];
        printf("index %d: ", current_sudoku_node->id);
        for (int j = 0; j < 20; j++) {
            if (current_sudoku_node->neighbours[j] == NULL) {
                break;
            }
            printf("%d ", current_sudoku_node->neighbours[j]->data);
        }
        printf("\n");
    }
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
            sudoku_node* new_sudoku_node = (struct sudoku_node *) malloc(sizeof(sudoku_node));
            new_sudoku_node->id = id;
            new_sudoku_node->data = data;
            if (data != 0) { // means the node is already solved
                new_sudoku_node->is_solved = 1;
                // initialize the possible solutions as NULL since it is already solved
                new_sudoku_node->possible_solutions = NULL;
            } else {
                new_sudoku_node->is_solved = 0;
                // initialize the possible solutions
                new_sudoku_node->possible_solutions = (int *) calloc(9, sizeof(int));
                for (int i = 1; i < 10; i++) {
                    new_sudoku_node->possible_solutions[i - 1] = i;
                }
            }

            // store the sudoku address to the sudoku (array)
            sudoku[id] = new_sudoku_node;

            id += 1;
        }
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
        sudoku_node** neighbours = (struct sudoku_node **) malloc(sizeof(sudoku_node) * NUMBER_OF_NEIGHBOURS);
        int index = 0; // to keep track for the neighbours
        
        // add in the horizontal
        int start = (current_sudoku_node->id / 9) * 9;
        for (int j = 0; j < 9; j++) {
            // ignore its own id
            if (current_sudoku_node->id == j + start) {
                continue;
            }
            // add others
            neighbours[index] = sudoku[start + j];
            index += 1;
        }

        // add in the vertical
        start = current_sudoku_node->id % 9;
        for (int j = 0; j < 9; j++) {
            // ignore its own id
            if (current_sudoku_node->id == j * 9 + start) {
                continue;
            }
            // add others
            neighbours[index] = sudoku[j * 9 + start];
            index += 1;
        }

        // add in the box
        int i = current_sudoku_node->id % 9; // x-axis
        int j = current_sudoku_node->id / 9; // y-axis
        int start_x_axis = (i / 3) * 3; // the x-corrdinate of start of the box
        int start_y_axis = (j / 3) * 3; // the y-coordinate of start of the box
        int x_axis = start_x_axis; // x-axis to be incremented
        for (int j = 0; j < 9; j++) {

            if (j % 3 == 0 && j != 0) {
                start_y_axis += 1;
            }
            x_axis = start_x_axis + (j % 3);
            int id = start_y_axis * 9 + x_axis;

            // ignore its own id
            if (id == current_sudoku_node->id) {
                continue;
            }
            // check whether the id has already been added
            int is_already_added = 0;
            for (int k = 0; k < index; k++) {
                if (neighbours[k] == NULL) {
                    break;
                }
                if (neighbours[k]->id == id) {
                    is_already_added = 1; break;
                }
            }
            // add if it has not existed
            if (!is_already_added) {
                neighbours[index] = sudoku[id];
                index += 1;
            }
        }
        // assign the neighbour attribute
        current_sudoku_node->neighbours = neighbours;
    }
    
    // find the solution
    int no_solution = 0;
    int has_backtrack = 0;
    sudoku_node* current_sudoku_node = sudoku[0];
    while (current_sudoku_node != NULL) {
        
        // already solved => move on to the next node if backtrack flag is not set
        if (current_sudoku_node->is_solved && !has_backtrack) {
            current_sudoku_node = current_sudoku_node->next;
            continue;
        }
        // already solved and backtrack flag is set => move to the previous node
        else if (current_sudoku_node->is_solved && has_backtrack) {
            current_sudoku_node = current_sudoku_node->prev;
            continue;
        }

        // backtrack flag is set => get the most recent data
        if (has_backtrack) {

            // reset the backtrack flag
            has_backtrack = 0;

            // get the most recent data
            int i = 0;
            for (i = 0; i < 9; i++) {
                if (current_sudoku_node->possible_solutions[i] != 0) {
                    int data = current_sudoku_node->possible_solutions[i];
                    current_sudoku_node->data = data;
                    // remove the found index
                    current_sudoku_node->possible_solutions[i] = 0;
                    break;
                }
            }

            // check if need to backtrack again
            if (i == 9) {
                has_backtrack = 1;
                // reset the data
                current_sudoku_node->data = 0;
                // reset the possible solutions
                for (int j = 1; j < 10; j++) {
                    current_sudoku_node->possible_solutions[j - 1] = j;
                }
                current_sudoku_node = current_sudoku_node->prev;
                if (current_sudoku_node == NULL) {
                    no_solution = 1;
                    printf("program can't solve! author is dumb!\n");
                }
            }
            else {
                current_sudoku_node = current_sudoku_node->next;
            }
        }
        else {
            // get the possible solutions
            for (int i = 0; i < 20; i++) {
                // get its neighbour's data
                int data = current_sudoku_node->neighbours[i]->data;
                if (data != 0) {
                    // reset the data in the possible solutions to 0
                    current_sudoku_node->possible_solutions[data - 1] = 0;
                }
            }

            // get the most recent data
            int i = 0;
            for (i = 0; i < 9; i++) {
                if (current_sudoku_node->possible_solutions[i] != 0) {
                    int data = current_sudoku_node->possible_solutions[i];
                    current_sudoku_node->data = data;
                    // remove the the very first index
                    current_sudoku_node->possible_solutions[i] = 0;
                    break;
                }
            }
            
            // check if need to backtrack again
            if (i == 9) {
                has_backtrack = 1;
                // reset the data
                current_sudoku_node->data = 0;
                // reset the possible solutions
                for (int j = 1; j < 10; j++) {
                    current_sudoku_node->possible_solutions[j - 1] = j;
                }
                current_sudoku_node = current_sudoku_node->prev;
                if (current_sudoku_node == NULL) {
                    no_solution = 1;
                    printf("program can't solve! author is dumb!\n");
                }
            }
            else {
                current_sudoku_node = current_sudoku_node->next;
            }
        }
    }

    if (!no_solution) {
        print_sudoku(sudoku);
    }

    free_sudoku(sudoku);

    return 0;
}