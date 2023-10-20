import numpy as np
import math


M=1024
for i in range(10, 26):
    print("Altura del arbol " + str(i) + ": " + str(np.ceil((np.log10(pow(2,i))/np.log10(M)))+1))


# print("hoal\n")

# print(np.log2(500000))

