#include "sudoku.h"

int main(int argc, char *argv[]) {
	// TODO -- mainly calls to functions to keep things neat
	// TOOD -- free calls to any mallocs
	return 0;
}

//PARAMS:	sudokuString -- sudoku number string
//EFFECTS:	converts a sudoku number string into a 9x9 array for easier parsing
//RETURNS:	sudokuArray -- a 2D int array
int **sudokuStringToArray(char *sudokuString) {
	// note that a 2D array is passed as a ** and has [ROWS][COLUMNS] format
	int **sudokuArray;
	
	int i, j;
	
	// Allocate memory for 2D array
	sudokuArray = (int **)malloc(sizeof(int*) * 9);
	for(i = 0; i < NUM_COLS; i++)
		sudokuArray[i] = (int *)malloc(sizeof(int) * 9);
	
	for(i = 0; i < NUM_ROWS; i++) {
		for(j = 0; j < NUM_COLS; j++)
			// the mask here might not be neccessary, or things could horrible blow up if it isn't
			sudokuArray[i][j] = (int) sudokuString[i * ROW_VALUE + j];
	}
	
	return sudokuArray;
}

//PARAMS:	sudokuArray -- 9x9 sudoku number array
//EFFECTS:	tests rows for sudoku rules
//RETURNS:	to validArray -- 0 if rows are valid, 1-9 for first invalid row
void *testRows(int **sudokuArray, int *validArray) {
	int i, j, k, temp;
	
	// Standard parsing through the array, note that testColummns will have i and j reversed
	for(i = 0; i < NUM_ROWS; i++) {
		for(j = 0; j < NUM_COLS; j++) {
			temp = sudokuArray[i][j];
			
			//checks current element against every other element ahead of its index in rows/columns for duplication
			for(k = (j + 1); k < NUM_COLS; k++) {
				if(temp == sudokuArray[i][k]){
					//incrementing i to match row # we want for debugging
					i++;
					
					printf("Parse Failed: Sudoku string provided is invalid at row #%i", i);
					*validArray = i;
					return;
				}
			}	
		}
	}
	*validArray = ISVALID;
}

//PARAMS:	sudokuArray -- 9x9 sudoku number array
//EFFECTS:	tests columns for sudoku rules
//RETURNS:	to validArray -- 0 if columns are valid, 1-9 for first invalid column
void *testColummns(int **sudokuArray, int *validArray) {
	int i, j, k, temp;
	
	// Reverse parsing compared to testRows
	for(j = 0; j < NUM_COLS; j++) {
		for(i = 0; i < NUM_ROWS; i++) {
			temp = sudokuArray[i][j];
			
			for(k = (i + 1); k < NUM_ROWS; k++) {
				if(temp == sudokuArray[k][j]){
					//incrementing j to match col # we want for debugging
					j++;
					
					printf("Parse Failed: Sudoku string provided is invalid at column #%i", j);
					*validArray = j;
					return;
				}
			}
		}
	}
	*validArray = ISVALID;
}

//PARAMS:	sudokuArray -- 9x9 sudoku number array
//EFFECTS:	tests a 3x3 cell for sudoku rules
//RETURNS:	to validArray -- 0 if cell is valid, 1-9 for particular cell tested
void *testCell(int **sudokuArray, Cell *cell, int *validArray) {
	int i, j, k, l, temp;
	
	for(i = cell->row; i < CELL_ROWS; i++) {
		for(j = cell->col; j < CELL_COLS; j++) {
			temp = sudokuArray[i][j];
			
			//parsing through the cell now -- note again that we don't need to check previous members as they've done the checking during their own routine
			for(k = i; k < CELL_ROWS; k++) {
				for(l = (j + 1); l < CELL_COLS; l++) {
					if(temp == sudokuArray[k][l]){
						printf("Parse Failed: Sudoku string provided is invalid at cell #%i", (cell->cellNum + 1));
						validArray[cell->cellNum] = cell->cellNum + 1;
					}
				}
			}	
		}
	}
	validArray[cell->cellNum] = ISVALID;
}




