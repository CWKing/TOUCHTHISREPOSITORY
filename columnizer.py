'''
	Takes in a master list and returns a list of lists from
	the elements of the input list based on the number of columns
	specified.
	
	Author: Cody "WE NEED MORE MODULES" King
	LastUpdated: October 14, 2016
	Python Version: 2.7.12
'''

def columnizer(mList, col):

	#i serves as the iterator through the columns
	i, theList = 0, []
	while i < col:
		theList.append([])
		
		#j serves as the iterator through the indices; must be reset to zero after every run
		j=0
		while j < len(mList):
		
			#If the difference between the index (j) and the current column (i) is evenly
			#divisible by the number of columns (col) then the value of that index belongs
			#in that column
			if (j-i)%col == 0:
				theList[i].append(mList[j])
			j+=1
		i+=1
	del(i, j, mList, col)
	return theList