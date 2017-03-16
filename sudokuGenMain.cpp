
/* TODO:
    - Everything
*/
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <windows.h>  // Only needed for debugging
#include <string>

using namespace std;

bool debug = false;

struct RowCol {                 //Define the RowCol structure to contain values simultaneously
    short row;
    short col;
    };

struct cell {                   //Define the cell structure
    short number;               //To hold the value in the cell, needs to be initialized to 0 (an invalid input) for all cells
    short blockNum;             //To hold the blockNumber the cell is in
    short row;                  //To hold the row the cell is in
    short column;               //To hold the column the cell is in
    bool pote[9];               //To hold which value could be placed in the cell; subscript 0 corresponds to 1, 1 to 2, etc all must be initialized to true
    bool byNec;                 //True if the number in the cell was placed by necessity from numbers in other cells; must be initialized to false
};

struct grid {                   //Define the grid structure
    cell GRID[81];              //Define main grid; will hold in sequential order the cells
    cell* GRIDRF[9][9];         //Define Row First grid; a matrix with the first index equal to the row number and the second the column; consists of pointers to cell objects
    cell* GRIDBB[9][9];         //Define By Block grid; a matrix with the first index equal to the block number and the second the lower index; consists of pointers to cell objects
    vector<RowCol> poteCells;   //Define Potential cell vector; used to store all the iterations of rows and columns; to be used to grab random cells quickly
};

struct C4N {
    short counter = 0;
    vector<RowCol> indecies;
};

//Premeditative function declarations
int intswitch(int, int, int);
cell cell_initializer(short, short, short);
grid main_initializer ();
void printGrid(grid);
short findPoteCellIndex(cell, grid);
void testPotentials(cell &, grid &);
void changePotentials(cell &, grid &);
void soleCell(grid &);
void randomCellNum(grid &);


//Define function main
int main() {
    grid theGrid = main_initializer();
    //printGrid(theGrid);
    while (theGrid.poteCells.size() > 2) {
        randomCellNum(theGrid);
        soleCell(theGrid);
    }
    printGrid(theGrid);
    return 0;
}
//End function main

//Convoluted unit step function
int intswitch(int num, int switchy = 0, int switchtype = 0) {
	if (switchtype == 0) {
		if (!(num < switchy)) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		if (!(num < switchy)) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

//Cell initializer function; for declaring new cells initialized with specific values
cell cell_initializer (short ROW, short COL, short BLO) {
    cell temp_cell;
    temp_cell.number = 0;
    temp_cell.blockNum = BLO;
    temp_cell.row = ROW;
    temp_cell.column = COL;
    temp_cell.byNec = false;
    for (short i; i < 9; i++) {
        temp_cell.pote[i] = true;
    }
    return temp_cell;
}

//Main initializer function, declares everything in the grid structure
grid main_initializer () {
    grid GRIDDY;
    RowCol tempRowCol;
    short blockit = 0;
    for (short rowNum = 0; rowNum < 9; rowNum++) {
        for (short colNum = 0; colNum < 9; colNum++) {
            short blockNum = (static_cast<int>(blockit / 3.) % 3) + (3 * static_cast<int>(static_cast<int>(blockit / 9.) / 3.));
            short blockSubIndex = (blockit % 3) + 3 * (static_cast<int>(static_cast<int>(blockit / 3.) / 3.) % 3 * (1 - intswitch(blockit,27) + intswitch(blockit,36)));
            GRIDDY.GRID[blockit] = cell_initializer(rowNum, colNum, blockNum);
            GRIDDY.GRIDRF[rowNum][colNum] = GRIDDY.GRIDBB[blockNum][blockSubIndex] = &GRIDDY.GRID[blockit];
            tempRowCol.row = rowNum;
            tempRowCol.col = colNum;
            GRIDDY.poteCells.push_back(tempRowCol);
            //In the code::blocks IDE the debugging window for watching variable contents does not innately display vector contents; hence the line below
            if (debug) cout << GRIDDY.poteCells.size() << ": " << GRIDDY.poteCells[blockit].row << ", " << GRIDDY.poteCells[blockit].col << "\n";
            blockit += 1;
        }
    }
    return GRIDDY;
}

void printGrid(grid GRIDDY) {
    for (int n = 0; n < 9; n++){
        if (n % 3 == 0) cout << "+---------+---------+---------+\n";
        for (int m = 0; m < 9; m++) {
            if (m % 3 == 0) cout << "|";
            cout << "[" << GRIDDY.GRIDRF[n][m]->number << "]"; //'->' is how a pointer for a class points to an attribute of that class
        }
        cout << "|" << endl;
    }
    cout << "+---------+---------+---------+\n";
}

short findPoteCellIndex(cell CELL, grid __GRID__) {
	for (short index = 0; index < __GRID__.poteCells.size(); index++) {
		if (__GRID__.poteCells[index].row == CELL.row && __GRID__.poteCells[index].col == CELL.column)
			return index;
	}
}

void testPotentials(cell &CELL, grid &__GRID__) {
	short truths = 0;
	short truthindex = 0;
	for (short i = 0; i < 9; i++) {
		if (CELL.pote[i]) {
			truths += 1;
			truthindex = i;
		}
	}
	if (truths == 1) {
		CELL.number = truthindex + 1;
		CELL.byNec = true;
		__GRID__.poteCells.erase(__GRID__.poteCells.begin() + findPoteCellIndex(CELL, __GRID__));
		changePotentials(CELL, __GRID__);
	}
}

void changePotentials(cell &CELL, grid &__GRID__) {
	for (short i = 0; i < 9; i++) CELL.pote[i] = false;
	for (short c = 0; c < 9; c++) { //Checking the row
		if (__GRID__.GRIDRF[CELL.row][c]->number == 0) {
			__GRID__.GRIDRF[CELL.row][c]->pote[CELL.number - 1] = false;
			testPotentials(*__GRID__.GRIDRF[CELL.row][c], __GRID__);
		}
	}
	for (short r = 0; r < 9; r++) { //Checking the column
		if (__GRID__.GRIDRF[r][CELL.column]->number == 0) {
			__GRID__.GRIDRF[r][CELL.column]->pote[CELL.number - 1] = false;
			testPotentials(*__GRID__.GRIDRF[r][CELL.column], __GRID__);
		}
	}
    for (short b = 0; b < 9; b++) { //Checking the block
        if (__GRID__.GRIDBB[CELL.blockNum][b]->number == 0) {
            __GRID__.GRIDBB[CELL.blockNum][b]->pote[CELL.number - 1] = false;
            testPotentials(*__GRID__.GRIDBB[CELL.blockNum][b], __GRID__);
        }
    }
}

void soleCell(grid &__GRID__) {
    for (short r = 0; r < 9; r++) { //Checking row by row...
        C4N CforN[9];
        RowCol tempRowCol;
        CforN[r].counter = 0;
        for (short c = 0; c < 9; c++) {
            for (short index = 0; index < 9; index++) {
                if (__GRID__.GRIDRF[r][c]->pote[index]) {
                    CforN[index].counter += 1;
                    tempRowCol.row = r;
                    tempRowCol.col = c;
                    CforN[index].indecies.push_back(tempRowCol);
                }
            }
        }
        for (short i = 0; i < 9; i++) { //Looking for isolated values
            if (CforN[i].counter == 1) {
                __GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col]->number = i + 1;
				changePotentials(*__GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col], __GRID__);
            }
        }
    }
    for (short c = 0; c < 9; c++) { //Checking column by column...
        C4N CforN[9];
        RowCol tempRowCol;
        CforN[c].counter = 0;
        for (short r = 0; r < 9; r++) {
            for (short index = 0; index < 9; index++) {
                if (__GRID__.GRIDRF[r][c]->pote[index]) {
                    CforN[index].counter += 1;
                    tempRowCol.row = r;
                    tempRowCol.col = c;
                    CforN[index].indecies.push_back(tempRowCol);
                }
            }
        }
        for (short i = 0; i < 9; i++) { //Looking for isolated values
            if (CforN[i].counter == 1) {
                __GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col]->number = i + 1;
				changePotentials(*__GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col], __GRID__);
            }
        }
    }
    for (short b = 0; b < 9; b++) { //Checking block by block...
        C4N CforN[9];
        RowCol tempRowCol;
        CforN[b].counter = 0;
        for (short i = 0; i < 9; i++) {
            for (short index = 0; index < 9; index++) {
                if (__GRID__.GRIDBB[b][i]->pote[index]) {
                    CforN[index].counter += 1;
                    tempRowCol.row = __GRID__.GRIDBB[b][i]->row;
                    tempRowCol.col = __GRID__.GRIDBB[b][i]->column;
                    CforN[index].indecies.push_back(tempRowCol);
                }
            }
        }
        for (short i = 0; i < 9; i++) { //Looking for isolated values
            if (CforN[i].counter == 1) {
                __GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col]->number = i + 1;
				changePotentials(*__GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col], __GRID__);
            }
        }
    }
}

void randomCellNum(grid &__GRID__) {
	short index = rand() % __GRID__.poteCells.size();
	short row = __GRID__.poteCells[index].row;
	short column = __GRID__.poteCells[index].col;
	vector<short> pote;
	for (short p = 0; p < 9; p++) {
		if (__GRID__.GRIDRF[row][column]->pote[p]) {
			pote.push_back(p + 1);
		}
	__GRID__.GRIDRF[row][column]->number = pote[rand() % static_cast<int>(pote.size())];
	__GRID__.poteCells.erase(__GRID__.poteCells.begin() + index);
	changePotentials(*__GRID__.GRIDRF[row][column], __GRID__);
	}
}


/*
RowCol* poteCells = new RowCol[];
GRIDDY.poteCells[blockit] = tempRowCol;
*/
