#ifndef SUDOKU_H
#define SUDOKU_H

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

// Defines

#define NUM_COLS 9
#define NUM_ROWS 9

#define ROW_VALUE 9

#define TRUE 1
#define FALSE 0

// Struct Typedefs

typedef struct {
	int Col;
	int Row;
} Cell;

// Function Prototypes

int **sudokuStringToArray(char *);
int testColummns(int **);
int testRows(int **);
int testCell(int **, Cell *);

// TODO -- more functions to add later

#endif