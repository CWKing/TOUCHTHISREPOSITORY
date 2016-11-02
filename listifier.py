'''
	Takes in a tuple formatted by whiteSpaceParser and finds all the numerical values
	in that string; returning a list made of those values

	Author: Cody "I made this one too" King
	LastUpdated: October 14, 2016
	Python Version: 2.7.12
'''
def listifier(tuply):
	i,masterList=0,[]
	while i < len(tuply[1])-1:
		#print(i)   #Testing stuff to see if the module was getting hung-up
		try:
			#Tries to append to masterList a float made from the substring in tuply[0] (a string)
			#designated by the boundaries tuply[1][i] and tuply[1][i+1] (both ints in the list tuply[1])
			masterList.append(float(tuply[0][tuply[1][i]:tuply[1][i+1]]))
		except ValueError:
			#If the substring isn't floatable we ignore it
			pass
		finally:
			i+=1
	del(tuply, i)
	return masterList