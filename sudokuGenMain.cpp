
/* TODO:
    - Everything
    - Figure out why the poteCells vector member in grid is not accumulating values through push_back
*/
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <windows.h>  // Only needed for debugging
#include <string>

using namespace std;

struct RowCol {
    short row;
    short col;
};

struct cell {                   // Define the cell structure
    short val;                  // To hold the value in the cell, needs to be initialized to 0 (an invalid input) for all cells
    short blockNum;             // To hold the blockNumber the cell is in
    short row;                  // To hold the row the cell is in
    short column;               // To hold the column the cell is in
    bool pote[9];               // To hold which value could be placed in the cell; subscript 0 corresponds to 1, 1 to 2, etc all must be initialized to true
    bool byNec;                 // True if the number in the cell was placed by necessity from numbers in other cells; must be initialized to false
};

struct grid {
    cell GRID[81];              //Define main grid; will hold in sequential order the cells
    cell* GRIDRF[9][9];         //Define Row First grid; a matrix with the first index equal to the row number and the second the column; consists of pointers to cell objects
    cell* GRIDCF[9][9];         //Define Column First grid; a matrix with the first index equal to the column number and the second the row; consists of pointers to cell objects
    cell* GRIDBB[9][9];         //Define By Block grid; a matrix with the first index equal to the block number and the second the lower index; consists of pointers to cell objects
    vector<RowCol> poteCells;   //Define Potential cell vector; used to store all the iterations of rows and columns; to be used to grab random cells quickly
};

//Premeditative function declarations
int intswitch(int, int, int);
cell cell_initializer(short, short, short);
grid main_initializer ();
void printGrid(grid);

//Define function main
int main() {
    grid theGrid = main_initializer();
    printGrid(theGrid);
    //for (vector<RowCol>::const_iterator i = theGrid.poteCells.begin(); i != theGrid.poteCells.end(); ++i) cout << *i << "\n";
    return 0;
}
//End function main

//Very convoluted unit step function
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
    temp_cell.val = 0;
    temp_cell.blockNum = BLO;
    temp_cell.row = ROW;
    temp_cell.column = COL;
    temp_cell.byNec = false;
    for (short i; i < 9; i++) {
        temp_cell.pote[i] = false;
    }
    return temp_cell;
}

//Main initializer function, declares everything in the grid structure
grid main_initializer () {
    grid GRIDDY;
    short blockit = 0;
    for (short rowNum = 0; rowNum < 9; rowNum++) {
        for (short colNum = 0; colNum < 9; colNum++) {
            short blockNum = (static_cast<int>(blockit / 3.) % 3) + (3 * static_cast<int>(static_cast<int>(blockit / 9.) / 3.));
            short blockSubIndex = (blockit % 3) + 3 * (static_cast<int>(static_cast<int>(blockit / 3.) / 3.) % 3 * (1 - intswitch(blockit,27) + intswitch(blockit,36)));
            GRIDDY.GRID[blockit] = cell_initializer(rowNum, colNum, blockNum);
            GRIDDY.GRIDRF[rowNum][colNum] = GRIDDY.GRIDCF[colNum][rowNum] = GRIDDY.GRIDBB[blockNum][blockSubIndex] = &GRIDDY.GRID[blockit];
            RowCol tempRowCol;
            tempRowCol.row = rowNum;
            tempRowCol.col = colNum;
            GRIDDY.poteCells.push_back(tempRowCol);
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
            cout << "[" << GRIDDY.GRIDRF[n][m]->val << "]"; //'->' is how a pointer for a class points to an attribute of that class
        }
        cout << "|" << endl;
    }
    cout << "+---------+---------+---------+\n";
}
