#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-8
####
#### commrank.py : communicator rank and size
####
################################################################
################################################################

from mpi4py import MPI

comm = MPI.COMM_WORLD

####
#### Exercise part 1:
#### - use the Get_size and Get_rank methods of the COMM class
#### - let each processor print out a message like
####   "Hello from processor 7 out of 12"
####

##solution
nprocs = comm.Get_size()
procno = comm.Get_rank()
print("Hello from processor %d out of %d" % (procno,nprocs))
##solved

####
#### Exercise part 2:
#### - print out the number of processes from process zero
####

##solution
if procno==0:
    print("Running with %d processes" % nprocs)
##solved


