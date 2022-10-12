#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2019-2021
####
#### setdiff.py : set difference through bucket brigade
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import math
import random

def setdiff(mydata,filter):
    n = mydata.size
    for i_my in range(n):
        my = mydata[i_my]
        for i_ot in range(n):
            ot = filter[i_ot]
            if my==ot:
                mydata[i_my] = -1
                break

def print_distarray( local,comm,sort=False ):
    nprocs = comm.Get_size(); procno = comm.Get_rank()
    nlocal = local.size; nglobal = nprocs * nlocal
    global_values = np.zeros( nglobal,dtype=np.intc )
    comm.Gather( local,global_values, 0 )
    if procno==0:
        if sort:
            global_values.sort()
        str = ""
        for i in global_values:
            str += " %3d" % i
        print(str)

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

N = 10
PARTS = 2

mydata = np.zeros(N,dtype=np.intc) # random.Generator.random
for i in range(N):
    mydata[i] = int( random.random() * 4*N*nprocs )
filter = np.zeros(N,dtype=np.intc)
for i in range(N):
    filter[i] = int( random.random() * 4*N*nprocs )
result = mydata.copy()

#
# print initial arrays
#
print("Initial data:")
print_distarray( result,comm )
print("Filter:")
print_distarray( filter,comm,sort=True )

#
# set the `sendto' and `recvfrom' processes
#
sendto   = (procno+1) % nprocs
recvfrom = (procno-1+nprocs) % nprocs

##
## Exercise:
## -- compute the global set difference
##    by passing the filter around the ring in a bucket brigade
##
for p in range(nprocs):
#### your code here ####
#### your code here ####
