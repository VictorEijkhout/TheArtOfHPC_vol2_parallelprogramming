#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### allreduce.py : note the necessity for equal send/recv 
####     buffer sizes
####
################################################################
################################################################

import numpy as np
import random
from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

random.seed(procid)
random_bound = nprocs*nprocs
##codesnippet allreducep
random_number = random.randint(1,random_bound)
##codesnippet end
#print("[%d] random=%d" % (procid,random_number))

##codesnippet allreducep
# native mode send
max_random = comm.allreduce(random_number,op=MPI.MAX)
##codesnippet end

if procid==0:
    print("Python native:\n  max=%d" % max_random)

##codesnippet allreducenump
myrandom = np.empty(1,dtype=int)
myrandom[0] = random_number
allrandom = np.empty(nprocs,dtype=int)
# numpy mode send
comm.Allreduce(myrandom,allrandom[:1],op=MPI.MAX)
##codesnippet end

if procid==0:
    print("Python numpy:\n  max=%d" % allrandom[0])

sumrandom = np.zeros(1,dtype=int)
sumrandom[0] = myrandom[0]
#### WRONG polymorphic use does not work
#comm.Allreduce(sumrandom[:1])
comm.Allreduce(MPI.IN_PLACE,sumrandom[:1],op=MPI.SUM)

if procid==0:
    print( "Sum of randoms: %d, compare %d" % (sumrandom[0],nprocs*random_bound/2) )
