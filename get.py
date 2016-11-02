'''
	Functions which get info about numerical table files
	
	Author: Cody "Why did I make this so complicated" King
	LastUpdated: October 31, 2016
	Python Version: 2.7.12
'''
#Import the necessary components from whiteSpaceParser provided they haven't already been
from whiteSpaceParser import whitePaceSparser as wsp

'''
	Takes in a file object linked to a loosly formatted numerical table and
	returns a long of the index of where the table starts in that file.
	This file object should be opened in binary mode ('rb') to avoid reading
	issues which occur on the windows OS.
'''
def getStart(tFile):

	#Defining the boolean variables and a list to contain the start of each line
	first, second, lineIndex=False, False, [0L]
	
	#Reset the position of the reader incase the file has been read previously
	tFile.seek(0L)
	
	#The idea is if two subsequent lines of the file only contain elements that can be
	#converted to floats, then its likely that those lines are the start of the table.
	#This while loop will find it
	while (not first) and (not second):
		'''
		Reads in one line from the tFile object and calls wsp to generate a tuple of
		the parsed line (a string) and a list of where whitespaces had been slicing up
		that string. Then appends to lineIndex where the reader is (which is the beginning
		of the next line).
		'''
		toTest=tFile.readline()
		if toTest == '':
			print('Start of table not found')
			return None
		toTest=wsp(toTest)
		lineIndex.append(tFile.tell())

		#Debugging code
		#print(toTest)
		#print(lineIndex)
		#print(first, second)

		'''
		This while loop takes the toTest tuple and tests the elements in the string portion
		of said tuple using the list of index positions in the list portion. Starting from both
		first and second being false, it finds the first line in which all elements in the string
		portion are floatable and sets first to true before trying the next line. If the next line
		contains some non-floatable elements it resets first to false and starts the next line. Once
		both first and second are true, it's likely we've found the start of the table so we take
		lineIndex and return the second to last value in that list; this is the line position in
		tFile in which the table starts.
		'''
		i=0
		while i < len(toTest[1])-1:
		
			#Checking to see if the elements are floatable
			try:
				float(toTest[0][toTest[1][i]:toTest[1][i+1]])
				i+=1
			
			#If not we start over from the next line
			except ValueError:
				if first:
					first, second=False, False
				break
			if first:
				second=True
			else:
				first=True
		continue
	#If we've made it out of the loop then we've found the starting position (probably)
	del(tFile, first, second, i)
	return lineIndex[-2]
	
'''
	Takes in a file object linked to a numerical table and uses getStart to find the start of the table
	then finds the number of columns designated in this table.
'''
def getCol(tFile):
	tFile.seek(getStart(tFile))
	toTest=wsp(tFile.readline())
	del(tFile)
	return len(toTest[1])-1