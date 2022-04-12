#!/bin/bash

printf "Testing the sudoku solver\n"

# compile with flags
gcc -fsanitize=address,leak -Wall -Werror sudoku.c -o sudoku

# create a temporary file to store the result
touch temp.out

# run the sudoku
./sudoku ./tests/sudoku1.in > temp.out

cat temp.out

# get the difference
DIFF=$(diff -q temp.out ./tests/sudoku1.out)

# print the result
if [ -z "$DIFF" ]
then
    printf "Success!\n"
else
    printf "Fail!\n"
fi

# remove the temporary file
rm temp.out
