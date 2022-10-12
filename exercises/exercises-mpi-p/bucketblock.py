#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2019
####
#### bucketblock.py : blocking bucket brigade send
####
################################################################
################################################################

import math

from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

N = 10

mydata = np.zeros(N,dtype=np.float64)
mydata[:] = procno
leftdata = np.zeros(N,dtype=np.float64)

#
# set the `sendto' and `recvfrom' processes
#
if procno<nprocs-1:
    sendto = procno+1
else: sendto = MPI.PROC_NULL
if procno>0:
    recvfrom = procno-1
else: recvfrom = MPI.PROC_NULL

##
## Exercise:
## -- do the MPI_Send and MPI_Recv calls

#### your code here ####

##
## Check correctness
##

p1 = procno+1.
my_sum_of_squares = p1*p1*p1/3 + p1*p1/2 + p1/6
# why did this say math.max?
max_of_errors \
    = max ( [ abs( (my_sum_of_squares-mydata[i])/my_sum_of_squares) for i in range(N) ] )

error = np.zeros(1,dtype=np.int)
if max_of_errors>1.e-12:
    error[0] = procno
else:
    error[0] = nprocs
errors = np.zeros(1,dtype=np.int)
comm.Allreduce(error,errors,MPI.MIN)
if procno==0:
    if errors==nprocs:
        print("Finished; all results correct");
    else:
      print("First error occurred on proc %d" % errors)
