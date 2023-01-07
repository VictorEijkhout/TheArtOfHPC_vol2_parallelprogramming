#!/opt/local/bin/python2.7
#/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-9
####
#### spawn_manager.py : the manager part of a spawn example
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

#snippet spawnmanagerp
try :
    universe_size = comm.Get_attr(MPI.UNIVERSE_SIZE)
    if universe_size is None:
        print("Universe query returned None")
        universe_size = nprocs + 4
    else:
        print("World has {} ranks in a universe of {}"\
              .format(nprocs,universe_size))
except :
    print("Exception querying universe size")
    universe_size = nprocs + 4
nworkers = universe_size - nprocs

itercomm = comm.Spawn("./spawn_worker.py", maxprocs=nworkers)
#snippet end
