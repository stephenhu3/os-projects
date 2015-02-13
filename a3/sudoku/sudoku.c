#include "sudoku.h"

int main(int argc, char *argv[]) {
	int i, j, k;
	char scanSudoku[81];
	
	// Threads
	pthread_t rowsChk, colsChk, cellsChk[NUM_CELLS];
	
	// Validations
	int rowsValid = 0;
	int colsValid = 0;
	int cellsValid[NUM_CELLS] = {0,0,0,0,0,0,0,0,0};
	int solValid = 0;
	
	// 2D Array
	int **sudokuArray;
	
	// Struct Initializations
	struct Cell *cell = (struct Cell *)malloc(NUM_CELLS*sizeof(struct Cell));
	struct Pass *rowPass = (struct Pass *)malloc(sizeof(struct Pass));
	struct Pass *colPass = (struct Pass *)malloc(sizeof(struct Pass));
	struct CellPass *cellPass = (struct CellPass *)malloc(NUM_CELLS*sizeof(struct CellPass));
	
	// Cell Assignments -- not pretty but works
	i = 0;
	for(j = 0; j < NUM_ROWS; j+=3) {
		for(k = 0; k < NUM_COLS; k+=3) {
			cell[i].col = k;
			cell[i].row = j;
			cell[i].cellNum = i;
			i++;
		}
	}
	
	// Scan Handler
	printf("Input Sudoku string for validity test:\n");
	scanf("%81s", scanSudoku);
	
	char testSudoku[] = "624539187519728634837614295143865729958247361762391458371956842496182573285473911";

	if(strlen(scanSudoku) != 81){
		printf("Error: String size of %d is invalid (Need 81)! Defaulting to built in test...\n", strlen(scanSudoku));
		sudokuArray = sudokuStringToArray(testSudoku);
	}
	else
		sudokuArray = sudokuStringToArray(scanSudoku);
	
	// cellPass Assignments
	for(i = 0; i < NUM_CELLS; i++) {
		cellPass[i].cell = &cell[i];
		cellPass[i].sudokuArray = sudokuArray;
		cellPass[i].validArray = &cellsValid[i];
	}
	
	// rowPass Assignments
	rowPass->sudokuArray = sudokuArray;
	rowPass->validArray = &rowsValid;
	
	// colPass Assignments
	colPass->sudokuArray = sudokuArray;
	colPass->validArray = &colsValid;
	
	// Create Threads
	for(i = 0; i < NUM_CELLS; i++)
		pthread_create(&cellsChk[i], NULL, testCell, &cellPass[i]);
	pthread_create(&rowsChk, NULL, testRows, rowPass);
	pthread_create(&colsChk, NULL, testColumns, colPass);
	
	
	// Join Threads
	for(i = 0; i < NUM_CELLS; i++)
		pthread_join(cellsChk[i], NULL);
	pthread_join(rowsChk, NULL);
	pthread_join(colsChk, NULL);
	
	// Finalizing Messages
	if(rowsValid != 0) {
		printf("Error Dectected: First row error dectected at row #%i\n", rowsValid);
		solValid = FALSE;
	}
	if(colsValid != 0) {
		printf("Error Dectected: First column error dectected at column #%i\n", colsValid);
		solValid = FALSE;
	}
	for(i = 0; i < NUM_CELLS; i++) {
		if(cellsValid[i] != 0) {
			printf("Error Dectected: Cell error detected at cell #%i\n", cellsValid[i]);
			solValid = FALSE;
		}
	}
	
	if(solValid)
		printf("VALID SOLUTION: CONGRATULATIONS!\n");
	else
		printf("INVALID SOLUTION: Check the errors listed!\n");
	
	// Must Free
	free(cell);
	free(rowPass);
	free(colPass);
	free(cellPass);
	
	for(i = 0; i < NUM_COLS; i++)
		free(sudokuArray[i]);
	free(sudokuArray);
	
	printf("DONE!\n");
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
			sudokuArray[i][j] = sudokuString[i * ROW_VALUE + j] - 0x30; //0x30 to convert, wow this messed me up
	}
	
	return sudokuArray;
}

//PARAMS:	sudokuArray -- 9x9 sudoku number array
//EFFECTS:	tests rows for sudoku rules
//RETURNS:	to validArray -- 0 if rows are valid, 1-9 for first invalid row
void *testRows(struct Pass *pass) {
	int i, j, k, temp;
	
	// Standard parsing through the array, note that testColummns will have i and j reversed
	for(i = 0; i < NUM_ROWS; i++) {
		for(j = 0; j < NUM_COLS; j++) {
			temp = pass->sudokuArray[i][j];
			
			//checks current element against every other element ahead of its index in rows/columns for duplication
			for(k = (j + 1); k < NUM_COLS; k++) {
				if(temp == pass->sudokuArray[i][k]){
					//incrementing i to match row # we want for debugging
					i++;
					
					// printf("NOTIFICATION: Sudoku string provided is invalid at row #%i\n", i);
					*pass->validArray = i;
					return NULL;
				}
			}	
		}
	}
	*pass->validArray = ISVALID;
}

//PARAMS:	sudokuArray -- 9x9 sudoku number array
//EFFECTS:	tests columns for sudoku rules
//RETURNS:	to validArray -- 0 if columns are valid, 1-9 for first invalid column
void *testColumns(struct Pass *pass) {
	int i, j, k, temp;
	
	// Reverse parsing compared to testRows
	for(j = 0; j < NUM_COLS; j++) {
		for(i = 0; i < NUM_ROWS; i++) {
			temp = pass->sudokuArray[i][j];
			
			for(k = (i + 1); k < NUM_ROWS; k++) {
				if(temp == pass->sudokuArray[k][j]){
					//incrementing j to match col # we want for debugging
					j++;
					
					//printf("NOTIFICATION: Sudoku string provided is invalid at column #%i\n", j);
					*pass->validArray = j;
					return NULL;
				}
			}
		}
	}
	*pass->validArray = ISVALID;
}

//PARAMS:	sudokuArray -- 9x9 sudoku number array
//EFFECTS:	tests a 3x3 cell for sudoku rules
//RETURNS:	to validArray -- 0 if cell is valid, 1-9 for particular cell tested
void *testCell(struct CellPass *cellPass) {
	int i, j, k, l, temp;
	
	for(i = cellPass->cell->row; i < (cellPass->cell->row + CELL_ROWS); i++) {
		for(j = cellPass->cell->col; j < (cellPass->cell->col + CELL_COLS); j++) {
			temp = cellPass->sudokuArray[i][j];
			
			//parsing through the cell now -- note again that we don't need to check previous members as they've done the checking during their own routine
			for(k = i; k < (cellPass->cell->row + CELL_ROWS); k++) {
				for(l = (j + 1); l < (cellPass->cell->col + CELL_COLS); l++) {
					if(temp == cellPass->sudokuArray[k][l]){
						printf("NOTIFICATION: Sudoku string provided is invalid at cell #%i\n", (cellPass->cell->cellNum + 1));
						cellPass->validArray[cellPass->cell->cellNum] = (cellPass->cell->cellNum + 1);
						return NULL;
					}
				}
			}	
		}
	}
	cellPass->validArray[cellPass->cell->cellNum] = ISVALID;
}




