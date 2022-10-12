#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-8
####
#### randommaxp.py : use collectives to find maximum of local values
####
################################################################
################################################################

from mpi4py import MPI
import math
import numpy as np
import random
import sys

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## Initialize random number generator
## and compute random number between 0-1
##
random.seed(procno)
myrandom = np.zeros(1,dtype=np.float32); myrandom[0] = random.random()
print("Process %3d has random value %7.5f" % (procno,myrandom))

##
## Exercise part 1:
## -- compute the sum of values, everywhere
## -- scale your number by the sum
## -- check that the sum of the scaled values is 1
##
sumrandom = np.zeros(1,dtype=np.float32)
scaled_random = np.zeros(1,dtype=np.float32)
sum_scaled_random = np.zeros(1,dtype=np.float32)
comm.Allreduce(
#### your code here ####
)
scaled_random = myrandom / sumrandom
comm.Allreduce(
#### your code here ####
)

##
## Correctness test:
## the sum of scaled numbers should be pretty close to 1
##
error = np.zeros(1,dtype=np.intc)
errors = np.zeros(1,dtype=np.intc)
error[0] = nprocs
if abs(sum_scaled_random-1.)>1.e-5:
    print("Suspicious sum %7.5f on process %3d" % (sum_scaled_random,procno))
    error[0] = procno
comm.Reduce(error,errors,MPI.MIN)
if procno==0:
    if errors[0]==nprocs:
        print("Part 1 finished; all results correct")
    else:
        print("Part 1: first error occurred on rank %d" % errors[0])
