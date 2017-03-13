'''
    The idea is that all primes are expressable in the form:
                        P = P_1^n+-P_2
    That is, all primes may be expressed as the sum of an integer
    power of one prime plus or minus another prime.

    Code has excessive run-time and needs imporvement.
    127 is the first number that doesn't seem to have a representation
'''

#from math import sqrt

PrimeTableFileP_1 = open(r'C:\Users\Cody\Desktop\Mathemphetamines\PrimeTable.txt', 'rb')
PrimeTableFileP_2 = open(r'C:\Users\Cody\Desktop\Mathemphetamines\PrimeTable.txt', 'rb')
PrimeTableFile = open(r'C:\Users\Cody\Desktop\Mathemphetamines\PrimeTable.txt', 'rb')


def checkForm(toCheck):
    PrimeTableFileP_1.seek(0)
    PrimeTableFileP_2.seek(0)
    for P_1 in PrimeTableFileP_1:
        P_1 = int(P_1)
        for P_2 in PrimeTableFileP_2:
            P_2 = int(P_2)
            n = 1
            while not P_1**n - P_2 > toCheck:
                if P_1**n + P_2 == toCheck:
                   return str(P_1) + '^' + str(n) + ' + ' + str(P_2) + ' = ' + str(toCheck)
                elif P_1**n - P_2 == toCheck:
                   return str(P_1) + '^' + str(n) + ' - ' + str(P_2) + ' = ' + str(toCheck)
                else:
                    n+=1
            #print(P_1, n, P_2, toCheck)
            continue

for ele in PrimeTableFile:
    print('Trying for ' + ele)
    ele = int(ele)
    print(checkForm(ele))
