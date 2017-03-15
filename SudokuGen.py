'''
WIP that will probably be abandoned in Python since Python doesn't seem to lend itself to forcing multiple names to simultaneously
/POINT/ to the same object instance. POINTERS. FORCED REFRENCES. WHY NOT HAVE? WHY RELY ON CHANCE?
WHY CAN'T I JUST MAKE TWO LISTS WHERE listA[n][m] AND listB[m][n] REFERENCE THE SAME OBJECT?

Unless I can figure out a clever way to /quickly/ do checking on block sets
'''

#def sudokuGen():
from __future__ import print_function
from random import randint, seed
from math import floor
'''
TODO:
-Build in seed input prompt
-Build in debugging
-Finish building new check which identifies cell as only cell in a row/column/block that a specific number may go
-Figure out how to assign the same object instance to different indecies of multiple lists
'''
#---------------------
#- Define cell class -
#---------------------
class cell:
	def __init__(self, rowNum, colNum, blockNum):
		#Row the cell belongs to
		self.row = rowNum
		#Column the cell belongs to
		self.column = colNum
		#Block the cell belongs to
		self.block = blockNum
		#Potential numbers for this cell
		self.potentials = [True] * 9
		#The value in a cell. Zero if empty
		self.number = 0
		#Whether the cell was filled by necessity or randomly
		self.byNecessity = False
#-----------------------------
#- End cell class definition -
#-----------------------------
#---------------------
#- Define Grid class -
#---------------------
class grid:
	def __init__(self):
		GRID, poteCells = [], []
		def populate(__GRID__):
			rowNum, blockit = 0, 0
			while rowNum < 9:
				GRID.append([])
				colNum = 0
				while colNum < 9:
					blockNum = int((floor(blockit / 3) % 3) + 3 * floor(floor(blockit / 9) / 3))
					GRID[rowNum].append(cell(rowNum, colNum, blockNum))
					poteCells.append((rowNum, colNum))
					blockit += 1
					colNum += 1
				rowNum += 1
		populate(self)
#-----------------------------
#- End Grid class definition -
#-----------------------------
#------------------------------
#- Define necessary functions -
#------------------------------
'''
testPotentials function. Pass it a cell and it checks the potentials in it.
If it finds the cell only has one potential, it sets that value for the cell
then calls the changePotentials function with the argument of the passed cell.
'''
def testPotentials(CELL, __GRID__):
	truths = 0
	for i, ele in enumerate(CELL.potentials):
		if ele:
			truths += 1
			truthindex = i
	if truths == 1:
		CELL.number = truthindex + 1
		CELL.byNecessity = True
		del(__GRID__.poteCells[findPoteCellIndex(CELL, __GRID__)])
		changePotentials(CELL, __GRID__)

'''
changePotentials function. Pass it a cell and it turns the potential of the number
in the passed cell in every other cell in the same row, column, and block to false.
For each cell it perfroms this change on, it calls testPotentials with the argument
of the cell whose potential it just changed.
'''
def changePotentials(CELL, __GRID__):
	for i, NULL in enumerate(CELL.potentials):
		CELL.potentials[i] = False
	for cell in __GRID__.GRID[CELL.row]:
		if not cell.number:
			cell.potentials[CELL.number - 1] = False
			testPotentials(cell, __GRID__)
	for row in __GRID__.GRID:
		if not row[CELL.column].number:
			row[CELL.column].potentials[CELL.number - 1] = False
			testPotentials(row[CELL.column], __GRID__)
		for cell in row:
			if not cell.number:
				if cell.block == CELL.block:
					cell.potentials[CELL.number - 1] = False
					testPotentials(cell, __GRID__)

'''
soleCell function. Pass it the grid and it will check every row/column/block for cells
that are the only possible place for a specific number (even if other numbers are
possible in said cell). First check every row; then every column, then every block.
CforN stands for "Cell for Number" and is a list of lists; the exterior list is comprised
of nine sublists, the index of each sublist representing a number - 1. For each interior
list, the 0th index is the number of times the number has been found in the potentials of
a given row/column/block, each subsequent element is a tuple composed of the row and column
location of each potential. After an iteration over an entire row/column/block, if there
exists an interior list in CforN with 1 as its 0th element, then the 1st element of that
same interior list is the row and column location of where the number associated by the
index of that interior list must go. Alternatively could do the check by cell and check
the row, column, and block for the same criteria; not sure if it would be faster.
'''
def soleCell(__GRID__):
	#Do the check for the rows
	for row in __GRID__.GRID:
		CforN = [[0]] * 9
		for cell in row:
			for index, potential in enumerate(cell.potentials):
				if potential:
					CforN[index][0] += 1
					CforN[index].append((cell.row, cell.column))
		for subListIndex, subList in enumerate(CforN):
			if subList[0] == 1:
				row, column = subList[1]
				__GRID__.GRID[row][column].number = subListIndex + 1
				changePotentials(__GRID__.GRID[row][column], __GRID__)

	#Do the check for the columns
	ROW, COLUMN = 0, 0
	while COLUMN < 9:
		CforN =[[0]] * 9
		while ROW < 9:
			cell = __GRID__.GRID[ROW][COLUMN]
			for index, potential in enumerate(cell.potentials):
				if potential:
					CforN[index][0] += 1
					CforN[index].append((cell.row, cell.column))
			ROW += 1
		for subListIndex, subList in enumerate(CforN):
			if subList[0] == 1:
				row, column = subList[1]
				__GRID__.GRID[row][column].number = subListIndex + 1
				changePotentials(__GRID__.GRID[row][column], __GRID__)
		COLUMN += 1

	#Do the check for the block
	#it's not here!


'''
randomCellNum function. When called, it picks a random cell and, if not populated,
assigns it a number from one of its potentials; then calls changePotentials with the
argument of the cell whose number it changed and the grid
'''
def randomCellNum(__GRID__):
	index = randint(0,len(__GRID__.poteCells) - 1)
	row, column = __GRID__.poteCells[index]
	pote = []
	for number, potential in enumerate(__GRID__.GRID[row][column].potentials):
		if potential:
			pote.append(number + 1)
	__GRID__.GRID[row][column].number = pote[randint(0, len(pote) - 1)]
	del(__GRID__.poteCells[index])
	changePotentials(__GRID__.GRID[row][column], __GRID__)

'''
findPoteCellIndex function. Probably unecessary.
'''
def findPoteCellIndex(CELL, __GRID__):
	for index, poteCell in enumerate(__GRID__.poteCells):
		if poteCell == (CELL.row, CELL.column):
			return index

'''
printGrid function. When passed a grid object, prints a stylized version.
'''
def printGrid(__GRID__):
	n = 0
	while (n < 9):
		if (n % 3 == 0):
			print("+---------+---------+---------+")
		m = 0
		while (m < 9):
			if (m % 3 == 0):
				print("|", end = '')
			print("[" + str(__GRID__.GRID[n][m].number) + "]", end = '')
			m += 1
		print('|\0')
		n += 1
	print("+---------+---------+---------+")

'''
test function. Just a short test function to do testing things.
'''
def test(__GRID__):
	for row in __GRID__.GRID:
		for cell in row:
			print('r:' + str(cell.row), 'c:' + str(cell.column), 'b:' + str(cell.block), 'n:' + str(cell.number), 'NEC:' + str(cell.byNecessity), 'Pote:' + str(cell.potentials))
	printGrid(__GRID__)

#Establish theGrid
#theGrid.GRID[ROW][COLUMN] -> cell at that row and in that column
theGrid = grid()

#while len(theGrid.poteCells) > 2:
#	randomCellNum(theGrid)
	#soleCell(theGrid)

#printGrid(theGrid.GRID)
'''
Degenerate Code

grid = [cell()] * 81

for i, ele in enumerate(grid):
	#Set the row
	ele.row = int(floor(i / 9))
	#Set the column
	ele.column = i % 9
	#Set the block
	ele.block = int((floor(i / 3) % 3) + 3 * floor(floor(i / 9) / 3))

def randomCellNum(_GRID_):
	row, column, pote = randint(0,8), randint(0,8), []
	while _GRID_[row][column].number:
		row, column = randint(0,8), randint(0,8)
	for number, potential in enumerate(_GRID_[row][column].potentials):
		if potential:
			pote.append(number + 1)
	_GRID_[row][column].number = pote[randint(0, len(pote))]
	changePotentials(_GRID_[row][column], _GRID_)

class grid:
	GRID, GRIDRF, GRIDCF, GRIDBB, poteCells = [], [], [], [], []
	def __init__(self):
		def populate(__GRID__):
			rowNum, blockit = 0, 0
			while rowNum < 9:
				__GRID__.GRIDRF.append([])
				colNum = 0
				while colNum < 9:
					if not colNum % 3: 
						__GRID__.GRIDBB.append([])
					if rowNum == 0:
						__GRID__.GRIDCF.append([])
					blockNum = int((floor(blockit / 3) % 3) + 3 * floor(floor(blockit / 9) / 3))
					__GRID__.GRIDRF[rowNum].append(__GRID__.GRIDCF[colNum].append(__GRID__.GRID.append(cell(rowNum, colNum, blockNum))))
					__GRID__.poteCells.append((rowNum, colNum))
					blockit += 1
					colNum += 1
				rowNum += 1
		populate(self)

R, C, B, cells = 0, 0, 0, []
while R < 9:
	C = 0
	while C < 9:
		B2 = int((B/3.)%3)+3*int(int(B/9.)/3.)
		cells.append((R, C, B2))
		C += 1
		B += 1
	R += 1
'''
