#---------------------------------------------
# Example code for debugging training at TACC
#
# v1.0, 2018-06-27, dmcdougall@tacc.utexas.edu
#---------------------------------------------
import numpy as np

a = np.ones(5)
b = a * 2.0

# Array is accessing elements beyond allocated boundary
for i in range(5):
  a[i+1] = 2.0 * b[i]
  print("a["+str(i)+"] = "+str(a[i]))
