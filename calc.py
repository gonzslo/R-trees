import numpy as np
import math

for i in range(10, 26):
    print("Altura del arbol " + str(i) + ": " + str(np.ceil((np.log10(pow(2,i+10))/np.log10(1024)))+1))


# print("hoal\n")

# print(np.log2(500000))