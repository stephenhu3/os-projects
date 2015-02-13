#ifndef SUDOKU_H
#define SUDOKU_H

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

// Defines

#define NUM_COLS 9
#define NUM_ROWS 9
#define NUM_CELLS 9

#define ROW_VALUE 9

#define CELL_ROWS 3
#define CELL_COLS 3

#define ISVALID 0

#define TRUE 1
#define FALSE 0

// Struct Typedefs

struct Cell{
	int col;
	int row;
	int cellNum;
};

struct Pass{
	int **sudokuArray;
	int *validArray;
};

struct CellPass{
	int **sudokuArray;
	int *validArray;
	struct Cell *cell;
};

// Function Prototypes

int **sudokuStringToArray(char *);
void *testColumns(struct Pass *);
void *testRows(struct Pass *);
void *testCell(struct CellPass *);

#endif