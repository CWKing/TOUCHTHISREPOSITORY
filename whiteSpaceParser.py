'''
	Takes a loosly formated string and reformats it to have only spaces as its white-
	space characters. Returns said string as well as a list of index positions in that
	string where a whitespace had been separating elements in that string.
	
	Author: Cody "This is the third time I've changed what it does" King
	LastUpdated: October 31, 2016
	Python Version: 2.7.12
'''
#Defining whiteSpaceParser
def whitePaceSparser(master):

	#To hold the index position of whitespaces
	whiteSpaceLocal=[]

	#Appends a space at the end of master to prevent future index problems
	#in whiteSpaceLocal provided one isn't already there
	if not master.endswith(' '):
		master=master + ' '
	if not master.startswith(' '):
		master=' ' + master

	#Deletes all tabs, newlines, and carriage returns (\r) replacing them with whitespaces
	master=master.replace('\t',' ')
	master=master.replace('\n',' ')
	master=master.replace('\r',' ')

	#Deletes any redundant whitespaces by replacing all double whitespace with a single one
	#loops through master until all of them are gone
	while master.find('  ') != -1:
		master=master.replace('  ',' ')
	
	#Iterates through the master string and appends to whiteSpaceLocal the index
	#position of a found whitespace
	for i, ele in enumerate(master):
		if ele == ' ':
			whiteSpaceLocal.append(i)

	#Deletes all the whitespaces
	master=master.translate(None,' ')

	#Deleting all the whitespaces causes master to shrink and reposition where
	#the whitespaces where by an amount equal to the index of whiteSpaceLocal
	#We compensate with the following
	for i, val in enumerate(whiteSpaceLocal):
		whiteSpaceLocal[i]=val-i
	
	#Returns a tuple of the reformated string and a list of the index positions of the whitespaces
	#that where in that string
	return master, whiteSpaceLocal