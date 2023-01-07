#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2022
####
#### commsplit.py : illustration of splitting communicators
####
################################################################
################################################################

import numpy as np
import random # random.randint(1,N), random.random()
from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<4:
    prin( "Need 4 procs at least")
    sys.exit(1)

#snippet commsplitp
mydata = procid

# communicator modulo 2
color = procid%2
mod2comm = comm.Split(color)
procid2 = mod2comm.Get_rank()

# communicator modulo 4 recursively
color = procid2 % 2
mod4comm = mod2comm.Split(color)
procid4 = mod4comm.Get_rank()
#snippet end

if mydata//4!=procid4:
    print("Error",procid,procid4,mydata/4)
else:
    print(f"Proc {procid} -> {procid2} -> {procid4}")

# if procid==0:
#     print("Finished")
