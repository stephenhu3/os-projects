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
	
	// Allocate memory for 2D array --apparently this is how you have to do it D:
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
//RETURNS:	0 if rows are valid, 1-9 for first invalid row
int testRows(int **sudokuArray) {
	int i, j, k, temp;
	
	// Standard parsing of array, note that testColummns will have i and j reversed
	for(i = 0; i < NUM_ROWS; i++) {
		for(j = 0; j < NUM_COLS; j++) {
			temp = sudokuArray[i][j];
			
			// What am I doing here guys?
			for(k = (temp + 1); k < NUM_COLS; k++) {
				if(temp == sudokuArray[i][k]){
					//incrementing i to match row # we want for debugging later
					i++;
					
					printf("Parse Failed: Sudoku string provided is invalid at row #%i", i);
					return i;
				}
			}
				
		}
	}

	return 0;
}

//PARAMS:	sudokuArray -- 9x9 sudoku number array
//EFFECTS:	tests columns for sudoku rules
//RETURNS:	0 if columns are valid, 1-9 for first invalid column
int testColummns(int **sudokuArray) {	
	return 0;
}

//PARAMS:	sudokuArray -- 9x9 sudoku number array
//EFFECTS:	tests a 3x3 cell for sudoku rules
//RETURNS:	0 if cell is valid, 1-9 for particular cell tested
int testCell(int **sudokuArray, Cell *cell) {
	// TODO
	return 0;
}

// TODO -- more functions to add later... particularly one to do all the threads