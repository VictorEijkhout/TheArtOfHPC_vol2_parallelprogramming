#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2019
####
#### rightput.py : put data right by one-sided
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## Create window from buffer
##
window_data = np.zeros(1,dtype=np.float32)
floatsize = np.dtype('float32').itemsize
win = MPI.Win.Create(
    window_data,floatsize,
    comm=comm)

##
## Allocate local data, and space to receive data
##
mydata = np.empty(1,dtype=np.float32)
mydata[0] = procno
leftdata = np.empty(1,dtype=np.float32)
leftdata[0] = -2

##
## Exercise:
## - put your data in the next process
##

##
## part 1: compute the `other' process number
##
#### your code here ####

##
## part 2: put `mydata' in the `other' window
##
win.Fence()
win.Put(
#### your code here ####
)
win.Fence()
leftdata[0] = window_data[0]

##
## Check correctness
##

error = np.empty(1,dtype=np.intc)
errors = np.empty(1,dtype=np.intc)
if procno>0 and leftdata[0]!=mydata[0]-1:
    print("[%d] received %d, s/b %d" % (procno,leftdata[0],mydata[0]-1))
    error[0] = procno
else: error[0] = nprocs
errors[0]=-1
comm.Allreduce(error,errors,MPI.MIN)
if procno==0:
    if errors==nprocs:
        print("Finished; all results correct");
    else:
      print("First error occurred on proc %d" % errors)
