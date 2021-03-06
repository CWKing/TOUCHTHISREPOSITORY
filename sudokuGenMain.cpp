
/* TODO:
    - Everything
    - Build in difficulty scaling
    - Build in sudoku-to-file writer
    - Prog crashes for some seeds; find out why: Program received signal SIGSEGV, Segmentation fault.
        - SIGSEV comes from bad memory access; i.e. attempting to do stuff to memory that is not accessible
        - BAD SEED(S): 17, 27, 70, 79, 98
*/
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;

bool debug = false;
bool printCompleted = false;

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
//void debugPrintOut (grid);


//Define function main
int main() {
    short resp, resp1;
    int seed;
    cout << "Run debugging? 1)Yes 2)No: ";
    cin >> resp;
    if (resp == 2) {
        cout << "Print a filled sudoku? 1)Yes 2)No: ";
        cin >> resp1;
        if (resp1 == 1) printCompleted = true;
    }
    cout << "\nPlease specify an integer seed: ";
    cin >> seed;
    srand(seed);
    if (resp == 1) debug = true;
    grid theGrid = main_initializer();
    while (theGrid.poteCells.size() > 0) {
        //if (debug) cout << "Calling randomCellNum\n";
        randomCellNum(theGrid);
        //if (debug) cout << "Calling soleCell\n";
        soleCell(theGrid);
    }
    printGrid(theGrid);
    cout << "Sudoku for seed: " << seed << "\nPress any key to exit";
    cin.ignore();   //Flushes nullspace from cin
    cin.get();      //To stop the prog from exiting immediately on completion
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
            //if (debug) cout << GRIDDY.poteCells.size() << ": " << GRIDDY.poteCells[blockit].row << ", " << GRIDDY.poteCells[blockit].col << "\n";
            blockit += 1;
        }
    }
    return GRIDDY;
}

void printGrid(grid GRIDDY) {
    for (int row = 0; row < 9; row++){
        if (row % 3 == 0) cout << "+---------+---------+---------+\n";
        for (int col = 0; col < 9; col++) {
            if (col % 3 == 0) cout << "|";
            if (debug || printCompleted) cout << "[" << GRIDDY.GRIDRF[row][col]->number << "]"; //'->' is how a pointer for a class points to an attribute of that class
            else {
                cout << "[";
                if (!(GRIDDY.GRIDRF[row][col]->byNec)) cout << GRIDDY.GRIDRF[row][col]->number; //'->' is how a pointer for a class points to an attribute of that class
                else cout << " ";
                cout << "]";
            }
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
		if (debug) {
            cout << "Putting [" << truthindex + 1 << "] in cell [" << CELL.row << ", " << CELL.column << "] (lone potential)\n";
            printGrid(__GRID__);
            cin.ignore();
            cin.get();
		}
		__GRID__.poteCells.erase(__GRID__.poteCells.begin() + findPoteCellIndex(CELL, __GRID__));
		changePotentials(CELL, __GRID__);
	}
}

void changePotentials(cell &CELL, grid &__GRID__) {
	for (short i = 0; i < 9; i++) CELL.pote[i] = false;
	for (short c = 0; c < 9; c++) { //Checking the row
		if (__GRID__.GRIDRF[CELL.row][c]->number == 0) {
			__GRID__.GRIDRF[CELL.row][c]->pote[CELL.number - 1] = false;
			//if (debug) cout << "Calling testPotentials for cell: [R- " << CELL.row << ", C- " << c << ", B- " << __GRID__.GRIDRF[CELL.row][c]->blockNum << "]\n";
			testPotentials(*__GRID__.GRIDRF[CELL.row][c], __GRID__);
		}
	}
	for (short r = 0; r < 9; r++) { //Checking the column
		if (__GRID__.GRIDRF[r][CELL.column]->number == 0) {
			__GRID__.GRIDRF[r][CELL.column]->pote[CELL.number - 1] = false;
			//if (debug) cout << "Calling testPotentials for cell: [R- " << r << ", C- " << CELL.column << ", B- " << __GRID__.GRIDRF[r][CELL.column]->blockNum << "]\n";
			testPotentials(*__GRID__.GRIDRF[r][CELL.column], __GRID__);
		}
	}
    for (short b = 0; b < 9; b++) { //Checking the block
        if (__GRID__.GRIDBB[CELL.blockNum][b]->number == 0) {
            __GRID__.GRIDBB[CELL.blockNum][b]->pote[CELL.number - 1] = false;
            //if (debug) cout << "Calling testPotentials for cell: [R- " << __GRID__.GRIDBB[CELL.blockNum][b]->row << ", C- " << __GRID__.GRIDBB[CELL.blockNum][b]->column << ", B- " << __GRID__.GRIDBB[CELL.blockNum][b]->blockNum << "]\n";
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
        //if (debug) cout << "For row [" << r << "] CforN contains [" << CforN[r].indecies.size() << "] values";
        for (short i = 0; i < 9; i++) { //Looking for isolated values
            if (CforN[i].counter == 1) {
                if (debug) cout << "From row check, putting [" << i + 1 << "] in the cell [" << CforN[i].indecies[0].row << ", " << CforN[i].indecies[0].col << "] (isolated value)\n";
                __GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col]->number = i + 1;
                __GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col]->byNec = true;
                if (debug) {
                    printGrid(__GRID__);
                    cin.ignore();
                    cin.get();
                }
                __GRID__.poteCells.erase(__GRID__.poteCells.begin() + findPoteCellIndex(*__GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col], __GRID__));
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
                if (debug) cout << "From column check, putting [" << i + 1 << "] in the cell [" << CforN[i].indecies[0].row << ", " << CforN[i].indecies[0].col << "] (isolated value)\n";
                __GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col]->number = i + 1;
                __GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col]->byNec = true;
                if (debug) {
                    printGrid(__GRID__);
                    cin.ignore();
                    cin.get();
                }
                __GRID__.poteCells.erase(__GRID__.poteCells.begin() + findPoteCellIndex(*__GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col], __GRID__));
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
                if (debug) cout <<  "From block check, putting [" << i + 1 << "] in the cell [" << CforN[i].indecies[0].row << ", " << CforN[i].indecies[0].col << "] (isolated value)\n";
                __GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col]->number = i + 1;
                __GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col]->byNec = true;
                if (debug) {
                    printGrid(__GRID__);
                    cin.ignore();
                    cin.get();
                }
                __GRID__.poteCells.erase(__GRID__.poteCells.begin() + findPoteCellIndex(*__GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col], __GRID__));
				changePotentials(*__GRID__.GRIDRF[CforN[i].indecies[0].row][CforN[i].indecies[0].col], __GRID__);
            }
        }
    }
}

void randomCellNum(grid &__GRID__) {
    short index, row, column;
    index = rand() % __GRID__.poteCells.size();
    row = __GRID__.poteCells[index].row;
    column = __GRID__.poteCells[index].col;
	vector<short> pote;
	for (short p = 0; p < 9; p++) {
        if (debug) cout << __GRID__.GRIDRF[row][column]->pote[p];
		if (__GRID__.GRIDRF[row][column]->pote[p]) {
			pote.push_back(p + 1);
		}
	}
	if (debug) {
        cout << "\nindex in poteCells: " << index << ", row: " << row << ", column: " << column << endl;
        cout << "poteCells.size(): " << __GRID__.poteCells.size() << ", pote.size(): " << pote.size() << endl;
	}
	int randomNumber = rand() % static_cast<int>(pote.size());
	if (debug) cout << "Putting number [" << pote[randomNumber] << "] in cell [" << row << ", " << column << "] (random potential)\n";
	__GRID__.GRIDRF[row][column]->number = pote[randomNumber]; //Program received signal SIGFPE, Arithmetic exception. A filled cell is being filled by the random thinger
	if (debug) cout << "Removing cell [" << __GRID__.poteCells[index].row << ", " << __GRID__.poteCells[index].col << "] from poteCells\n";
	__GRID__.poteCells.erase(__GRID__.poteCells.begin() + index);
    if (debug) {
        printGrid(__GRID__);
        cin.ignore();
        cin.get();
    }
	//if (debug) cout << "Calling change Potentials for cell: [R- " << row << ", C- " << column << ", N- " << __GRID__.GRIDRF[row][column]->number << "]\n";
	changePotentials(*__GRID__.GRIDRF[row][column], __GRID__);
}

/*
void debugPrintOut(grid GRIDDY) {
    for (short index = 0; index < 81; index++){
        cout << "\nCell [" << index << "]: row- " << GRIDDY.GRID[index].row << ", column- " << GRIDDY.GRID[index].column << ", number- " << GRIDDY.GRID[index].number << endl;
        cout << "Potentials:\n\t";
        for (short p = 0; p < 9; p++) cout << p + 1 << ":" << GRIDDY.GRID[index].pote[p] << " ";
    }
    cout << endl;
}
*/

/*
RowCol* poteCells = new RowCol[];
GRIDDY.poteCells[blockit] = tempRowCol;

    do { //Some filled cells were persisting in poteCells
        index = rand() % __GRID__.poteCells.size();
        row = __GRID__.poteCells[index].row;
        column = __GRID__.poteCells[index].col;
        if (__GRID__.GRIDRF[row][column]->number != 0)
            __GRID__.poteCells.erase(__GRID__.poteCells.begin() + index);
    } while(__GRID__.GRIDRF[row][column]->number != 0);

short iter = 0;
while (iter < theGrid.poteCells.size()) {
    if (theGrid.GRIDRF[theGrid.poteCells[iter].row][theGrid.poteCells[iter].row]->number != 0) {
        theGrid.poteCells.erase(theGrid.poteCells.begin() + findPoteCellIndex(theGrid.GRIDRF[theGrid.poteCells[iter].row][theGrid.poteCells[iter].row], theGrid));
    }
    else iter += 1;
}
*/
