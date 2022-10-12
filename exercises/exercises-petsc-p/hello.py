#!/usr/bin/env python
################################################################
####
#### This makefile is part of the tutorial
#### `Introduction to the PETSc library'
#### by Victor Eijkhout eijkhout@tacc.utexas.edu
####
#### copyright Victor Eijkhout 2012-6
####
################################################################

from petsc4py import PETSc
from mpi4py import MPI

comm = PETSc.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

####
#### Exercise 1:
#### - print out the number of processes from process zero
####
#### your code here ####

####
#### Exercise 2:
#### - Let all processes print out their number in sequence
####
#### your code here ####
PETSc.Sys.syncFlush(comm=comm)




