#def sudokuGen():
from __future__ import print_function
from random import randint, seed
from math import floor


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
#	def __hash__(self):
#		return hash(10 * self.row + self.column)
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
	GRID = []
	def __init__(self):
		def populate(_GRID_):
			rowNum, blockit = 0, 0
			while rowNum < 9:
				_GRID_.append([])
				colNum = 0
				while colNum < 9:
					blockNum = int((floor(blockit / 3) % 3) + 3 * floor(floor(blockit / 9) / 3))
					_GRID_[rowNum].append(cell(rowNum, colNum, blockNum))
					blockit += 1
					colNum += 1
				rowNum += 1
		populate(self.GRID)
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
def testPotentials(CELL, _GRID_):
	truths = 0
	for i, ele in enumerate(CELL.potentials):
		if ele:
			truths += 1
			truthindex = i
	if truths == 1:
		CELL.number[0] = truthindex + 1
		CELL.byNecessity[0] = True
		changePotentials(CELL, _GRID_)

'''
changePotentials function. Pass it a cell and it turns the potential of the number
in the passed cell in every other cell in the same row, column, and block to false.
For each cell it perfroms this change on, it calls testPotentials with the argument
of the cell whose potential it just changed.
'''
def changePotentials(CELL, _GRID_):
	for i, NULL in enumerate(CELL.potentials):
		CELL.potentials[i] = False
	for cell in _GRID_[CELL.row]:
		if not cell.number:
			cell.potentials[CELL.number - 1] = False
			testPotentials(cell, _GRID_)
	for row in _GRID_:
		if not row[CELL.column].number:
			row[CELL.column].potentials[CELL.number - 1] = False
			testPotentials(row[CELL.column], _GRID_)
		for cell in row:
			if not cell.number:
				if cell.block == CELL.block:
					cell.potentials[CELL.number - 1] = False
					testPotentials(cell, _GRID_)

'''
randomCellNum function. When called, it picks a random cell and, if not populated,
assigns it a number from one of its potentials; then calls changePotentials with the
argument of the cell whose number it changed and the grid
'''
def randomCellNum(_GRID_):
	row, column, pote = randint(0,8), randint(0,8), []
	while _GRID_[row][column].number:
		row, column = randint(0,8), randint(0,8)
	for number, potential in enumerate(_GRID_[row][column].potentials):
		if potential:
			pote.append(number + 1)
	_GRID_[row][column].number = pote[randint(0, len(pote))]
	changePotentials(_GRID_[row][column], _GRID_)

'''
printGrid function. When passed a grid object, prints a stylized version
'''
def printGrid(_GRID_):
	n = 0
	while (n < 9):
		if (n % 3 == 0):
			print("+---------+---------+---------+")
		m = 0
		while (m < 9):
			if (m % 3 == 0):
				print("|", end = '')
			print("[" + str(_GRID_[n][m].number) + "]", end = '')
			m += 1
		print('|\0')
		n += 1
	print("+---------+---------+---------+")

'''
test function. Just a short test function to do testing things.
'''
def test(_GRID_):
	for row in _GRID_:
		for cell in row:
			print('r:' + str(cell.row), 'c:' + str(cell.column), 'b:' + str(cell.block), 'n:' + str(cell.number), 'NEC:' + str(cell.byNecessity), 'Pote:' + str(cell.potentials))
	printGrid(_GRID_)

#Establish theGrid
#theGrid.GRID[ROW][COLUMN] -> cell at that row and in that column
theGrid = grid()

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

'''
