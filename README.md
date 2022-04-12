# sudoku-solver

Solves a sudoku by using backtracking idea <br>

The program takes in a file as the first argument in the command line argument. <br>

The file must contain appropriate data:
- Each value must be in the range of 0 to 9 where 0 means it is unsolved yet.
- There must be no non-integer values.

# Running the program

1. Compile the program (You can use any flags to detect any errors within the program)
2. `gcc -fsanitize=address,leak -Wall -Werror sudoku.c -o sudoku`
3. Once it is compiled, send the file you wish to solve
4. `./sudoku ./tests/sudoku1.in`

# Testing

You can test the program by creating a new .in file and .out file where
.in file is the unsolved sudoku and .out file is the sudoku. <br>
If the sudoku has more than 1 solution, the .out file may be different. <br>
1. Edit the `test.sh`
2. Go to line 14
3. Change the .in file you wish to check (Edit the `./tests/sudoku1.in`)
4. Go to line 23
5. Change the .out file you wish to check (Edit the `./tests/sudoku1.out`)

You can run the test file by running `./test.sh` command. <br>
If there are any differences in the output of the program and the .out file, it will print "Fail!"
Otherwise, it will print "Success". <br>
It also prints the time taken to get the sulution.