#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-8
####
#### commrank.py : communicator rank and size
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

mydata = np.empty(1,dtype=np.float32)
mydata[0] = procno
leftdata = np.empty(1,dtype=np.float32)
leftdata[0] = -2

##
## Exercise part 1:
## -- set `sendto` and `recvfrom'
## -- make sure the first and last processor are handled right
##

#### your code here ####

## Exercise 2:
## -- now do the MPI_Send and MPI_Recv calls

## Exercise 3:
## -- use the MPI_Sendrecv call instead

#### your code here ####

##
## Check correctness
##

error = np.empty(1,dtype=np.intc)
errors = np.empty(1,dtype=np.intc)
if procno>0 and leftdata!=mydata-1:
    error[0] = procno
else: error[0] = nprocs
errors[0]=-1
comm.Allreduce(error,errors,MPI.MIN)
if procno==0:
    if errors==nprocs:
        print("Finished; all results correct");
    else:
      print("First error occurred on proc %d" % errors)
