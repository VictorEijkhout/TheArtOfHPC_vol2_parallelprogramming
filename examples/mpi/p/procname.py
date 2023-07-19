#!/usr/bin/env python3
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2023
####
#### procname.py : get processor name
####
################################################################
################################################################

##codesnippet procnamep
from mpi4py import MPI
procname = MPI.Get_processor_name()
##codesnippet end
comm = MPI.COMM_WORLD
nprocs = comm.Get_size()
procno = comm.Get_rank()

print( f"Process {procno} runs on {procname}" )
