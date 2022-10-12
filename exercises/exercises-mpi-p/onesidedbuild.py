#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-8
####
#### onesidedbuild.py : put random data everywhere
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

## Start the random number generator
random.seed(procno)

##
## Create a table of fake shared memory
##
MAXFULL = 100
mytable = np.zeros(MAXFULL,dtype=np.int)
mytable[0] = 0; # initialize stack counter

the_window = MPI.Win.Create(
    mytable,disp_unit=intsize,
    comm=comm)

NUMBERS = 2
targets = np.zeros(NUMBERS,dtype=np.int)
for inum in range(NUMBERS):
    other_process = random.random() * nprocs
    targets[inum] = other_process

    ##
    ## - Lock the window on the other process
    ## - Get first open location, increment, and write
    ##
    the_window.lock(other_process)
    the_window.unlock(other_process)
    the_window.lock(other_process)
    the_window.unlock(other_process)

the_window.Free()

if procno==0:
    print("Finished")

