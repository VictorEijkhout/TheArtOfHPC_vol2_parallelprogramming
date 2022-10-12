#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-2022
####
#### randomput.py : put in random neighbor by one-sided
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import random
import sys

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

if nprocs<3:
    print("Need at least 3 procs")
    sys.exit(1)

window_data = np.zeros(1,dtype=np.int)
my_number = np.empty(1,dtype=np.int)
my_number[0] = 1

##
## Exercise 1:
## -- Determine the size of the window unit in bytes,
##    which will be used as displacement.
## Exercise 2:
## -- replace Win.Create by Win.Allocate
##
intsize = np.dtype('int').itemsize
win = MPI.Win.Create(
    ## specify window data and displacement
#### your code here ####
    comm=comm)

my_sum = 0
for iteration in range(20):
    ##
    ## Determine where to put data
    ##
    if random.random()<.5:
        other = 1
    else:
        other = 2

    ##
    ## Receiving processes set their window to zero
    ##
    window_data[0] = 0

    ##
    ## Now we have an epoch where 0 puts data in either 1 or 2
    ##
    win.Fence()
    if procno==0:
        win.Put(
#### your code here ####
        )
    win.Fence()
    my_sum += window_data[0]

win.Free()

if procno in [1,2]:
    print("Sum on %d: %d" % (procno,my_sum))
if procno==0:
    print("Sums on 1 and 2 together should be: 20")

