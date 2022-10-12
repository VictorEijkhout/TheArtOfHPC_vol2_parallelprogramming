#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-2022
####
#### isendirecv.py : MPI python exercise for isend/irecv
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import time

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## Initialize local data and buffers
##
mydata = np.empty(1,dtype=np.float64); mydata[0] = procno
leftdata = np.empty(1,dtype=np.float64); leftdata[0] = 0
rightdata = np.empty(1,dtype=np.float64); rightdata[0] = 0

## make an array to catch four requests
requests = [ None ] * 4

##
## Exercise:
## -- set `sendto' and `recvfrom' twice:
##    once to get data from the left, once from the right
## -- for first/last process use MPI_PROC_NULL as neighbor
##

## first specify communication with left neighbor
#### your code here ####
requests[
#### your code here ####
    ] = comm.Isend(mydata,sendto)
requests[
#### your code here ####
    ] = comm.Irecv(leftdata,recvfrom)

## then with right neighbor
#### your code here ####
requests[
#### your code here ####
    ] = comm.Isend(mydata,sendto)
requests[
#### your code here ####
    ] = comm.Irecv(rightdata,recvfrom)

##
## Then make sure all Isend/Irecv operations are completed
##
MPI.Request.Waitall(
#### your code here ####
    )

####
#### Check correctness
####
mydata = mydata+leftdata+rightdata

error = np.empty(1,dtype='int')
error[0] = nprocs
errors = np.empty(1,dtype='int')

if procno==0:
    check = 2*procno+1
elif procno==nprocs-1:
    check = 2*procno-1
else:
    check = 3*procno

def approxeq(x,y):
    return ( x==0. and abs(y)<1.e-14 ) \
        or ( y==0. and abs(x)<1.e-14 ) \
        or abs(x-y)/abs(x)<1.e-14

if not approxeq(mydata,check):
    print("Data on %d should be %e, not %e" % (procno,check,mydata))
    error[0] = procno

comm.Allreduce(error,errors,op=MPI.MIN)
if procno==0:
    if errors[0]<nprocs:
        print("First error appeared on proc",errors[0])
    else:
        print("Finished: all results correct")
