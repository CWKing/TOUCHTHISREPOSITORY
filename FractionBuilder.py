#import time	#requires the time module for the usage of the time.sleep() funciton
print('Simple script to build a decimal number into a fraction!')
dec = input('Input Decimal to build: ')
p, q, n = 1.0, 1.0, 0	#Appropriate vars need to be initialized as floats otherwise integer division and bad things happen
while p/q != dec:
	print(str(p) + "/" + str(q) + " " + str(p / q - dec))	#Usage of the preceding line greately increases run time
	if p / q < dec:
		p = p + 1
	else:
		q = q + 1
	n = n + 1
	if n % 100000000 == 0:
		print('Still working..\nPassed step ' + str(n))
print(str(p) + "/" + str(q) + " Fraction found in " + str(n) + " steps")
#time.sleep(5)
raw_input('Press ENTER to exit')