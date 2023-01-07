################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2022
####
#### randomupdatefence.c : put random data everywhere, with fence sync
####
################################################################
################################################################

import numpy as np
import random
import sys

from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<3:
    print("Needs at least 3 procs")
    sys.exit(1)

intsize = np.dtype('intc').itemsize
my_number = np.zeros(1,dtype=intc)
my_number[0] = procid
window_data = np.zeros(1,dtype=intc)
win = MPI.Win.Create(window_data,intsize,comm=comm)

count = np.zeros(2,dtype=int)
one = np.ones(1,dtype=intc)
for iwrite in range(10):
    other_process = random.randint(0,1)
    count[other_process] += 1
    win.Fence()
    if procid==2:
        win.Accumulate(one,other_process)
    win.Fence()
if procid==2: print("Done flipping")

win.Fence()
if procid==2:
    collect = np.zeros(2,dtype=intc)
    win.Get( collect[...,0], 0 )
    win.Get( collect[...,1], 1 )
win.Fence()
if procid==2:
    print(f"Send to 0:{count[0]} 1:{count[1]}, received: 0:{collect[0]} 1:{collect[1]}")
