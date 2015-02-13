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

#define CELL_ROWS 3
#define CELL_COLS 3

#define ISVALID 0

// Struct Typedefs

typedef struct Cell{
	int col;
	int row;
	int cellNum;
} Cell;

// Function Prototypes

int **sudokuStringToArray(char *);
void *testColummns(int **, int *);
void *testRows(int **, int *);
void *testCell(int **, Cell *, int *);

#endif