#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### hello.py : just init and hello world
####
################################################################
################################################################

from mpi4py import MPI
comm = MPI.COMM_WORLD
nprocs = comm.Get_size()
procno = comm.Get_rank()

print("Starting processes %d out of %d" % (procno,nprocs))
