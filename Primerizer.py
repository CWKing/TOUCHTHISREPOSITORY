# -*- coding: utf-8 -*-
"""
Created on Fri Dec 23 17:42:45 2016

@author: Cody
"""

from math import sqrt, log

directory = r'C:\Users\Cody\Desktop\Comp Programming\Python\Progs'

PrimeFileRead = open(directory + r'\PrimeTable.txt', 'rb')
PrimeFileWrite = open(directory + r'\PrimeTable.txt', 'ab')

def checkPrime(toCheck):
    S2C = sqrt(toCheck)
    PrimeFileRead.seek(0)
    for ele in PrimeFileRead.readlines():
        if toCheck == int(ele):
            return True, True
        elif int(ele) <= S2C:
            test = float(toCheck) / float(ele)
            if test == int(test):
                return False, False
    return True, False

PrimeFileRead.seek(-1, 2)
PrimeFileRead.seek(-int(log(PrimeFileRead.tell())), 2)
Start = int(PrimeFileRead.readlines()[-1])
End = 1000000000
print(Start, End)
    
while True:
    Start += 2
    test2 = checkPrime(Start)
    if test2[0] and not test2[1]:
        PrimeFileWrite.seek(0, 2)
        PrimeFileWrite.write('\n' + str(Start))
        PrimeFileRead.seek(0)
    if Start >= End:
        break

PrimeFileRead.close()
PrimeFileWrite.close()