'''
    The idea is that all even numbers E are expressible in the form:
                        E = P_1+-P_2
    That is, all even numbers may be expressed as the sum or difference
    of two primes

    
'''
PrimeTableFileP_1 = open(r'C:\Users\Cody\Desktop\Mathemphetamines\PrimeTable.txt', 'rb')
PrimeTableFileP_2 = open(r'C:\Users\Cody\Desktop\Mathemphetamines\PrimeTable.txt', 'rb')

def checkForm(toCheck):
    PrimeTableFileP_1.seek(0)
    PrimeTableFileP_2.seek(0)
    for P_1 in PrimeTableFileP_1:
        P_1 = int(P_1)
        PrimeTableFileP_2.seek(0)
        for P_2 in PrimeTableFileP_2:
            P_2 = int(P_2)
            if P_1 > P_2:
                continue
            elif P_2 - P_1 > toCheck:
                break
            elif P_2 - P_1 == toCheck:
                return str(P_2) + ' - ' + str(P_1) + ' = ' + str(toCheck)
            elif P_1 + P_2 == toCheck:
                return str(P_1) + ' + ' + str(P_2) + ' = ' + str(toCheck)
            else:
                continue

n = 0
while True:
    print(checkForm(n))
    n+=2
    
        


