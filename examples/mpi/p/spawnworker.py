#!/usr/bin/env python3
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### spawn_worker.py : the worker part of a spawn example
####
################################################################
################################################################

import numpy as np
import random # random.randint(1,N), random.random()

from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()

if procid==0:
    print("#workers:",nprocs)

#snippet spawnworkerp
parentcomm = comm.Get_parent()
nparents = parentcomm.Get_remote_size()
#snippet end

print("#parents=",nparents)

