#!/bin/bash

printf "Testing the sudoku solver\n"

# compile with flags
gcc -fsanitize=address,leak -Wall -Werror sudoku.c -o sudoku

# create a temporary file to store the result
touch temp.out

START=$(date +%s.%N)

# run the sudoku
./sudoku ./tests/sudoku4.in > temp.out

END=$(date +%s.%N)

printf "Time taken: $( echo "$END - $START" | bc -l )s\n"

cat temp.out

# get the difference
DIFF=$(diff temp.out ./tests/sudoku4.out)

# print the result
if [ -z "$DIFF" ]
then
    printf "Success!\n"
else
    printf "Fail!\n"
fi

# remove the temporary file
rm temp.out
