#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2021
####
#### mpi.py : initializing comm world
####
################################################################
################################################################

import numpy as np
import random # random.randint(1,N), random.random()
##codesnippet pympiimport
from mpi4py import MPI
##codesnippet end

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

print("Comm size:",comm.Get_size())
