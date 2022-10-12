#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-9
####
#### countdown.py : implementing shared memory through one-sided
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
random.seed(procno)

if nprocs<2:
    print("Need at least 2 procs")
    sys.exit(1)

counter_process = nprocs-1

##
## Determine the size of the window unit in bytes,
## which will be used as displacement.
##
intsize = np.dtype('int').itemsize
if procno==counter_process:
    window_data = np.zeros(1,dtype=np.int)
    window_data[0] = nprocs-1
else:
    window_data = np.zeros(0,dtype=np.int)
win = MPI.Win.Create(
    window_data,intsize,
    comm=comm)

##
## Loop forever:
## - at random times update the counter on the counter process
## - and read out the counter to see if we stop
##
minone = np.zeros(1,dtype=np.int)
minone[0] = -1
step = 1
while True:
    ##
    ## dynamic condition to determine whether to update the global counter
    ##
    i_must_update = random.random()<.5/nprocs
    win.Fence()
    if i_must_update:
        ##
        ## Exercise:
        ## - subtract one from the global counter
        ## - do you use Get/Put or Accumulate:
#### your code here ####
    is_zero = np.zeros(1,dtype=np.int)
    ##
    ## Exercise:
    ## - Now read out the global counter to see if it's time to stop
    ## - The problem is having your data synchronized.
    ##   Is the fence call at the top of the loop enough?
    ##
#### your code here ####
    if procno==counter_process:
        print("Step: %d, counter at %d" % (step,window_data))
    if is_zero<=0:
        final_value = is_zero; break
    step += 1

final_min = np.zeros(1,dtype=np.int)
final_max = np.zeros(1,dtype=np.int)
comm.Allreduce(final_value,final_min,MPI.MIN)
comm.Allreduce(final_value,final_max,MPI.MAX)

if procno==0:
    if final_min==final_max:
        print("Success: everyone agrees on the final value")
    else:
        printf("Failure: someone exits with %d, someone with %d" % (final_min,final_max))
    
win.Free()
