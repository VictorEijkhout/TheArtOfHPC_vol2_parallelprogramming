#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### putfence.py : point-to-point through put & fence
####
################################################################
################################################################

import numpy as np
import random # random.randint(1,N), random.random()
import sys

from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

#snippet windispp
intsize = np.dtype('int').itemsize
window_data = np.zeros(2,dtype=int)
win = MPI.Win.Create(window_data,intsize,comm=comm)
#snippet end

#snippet putblockp
my_number = np.empty(1,dtype=int)
src = 0; tgt = nprocs-1
if procid==src:
    my_number[0] = 37
else:
    my_number[0] = 1
win.Fence()
if procid==src:
    # put data in the second element of the window
    win.Put(my_number,tgt,target=1)
win.Fence()
#snippet end

if procid==tgt:
    print("Window after put:",window_data)


