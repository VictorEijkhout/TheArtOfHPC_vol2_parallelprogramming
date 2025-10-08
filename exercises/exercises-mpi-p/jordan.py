#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing in MPI and OpenMP"
#### by Victor Eijkhout, copyright 2017/8
####
#### jordan.py : Gauss-Jordan elimination
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

import random
random.seed(procno)

N = nprocs
# allocate one column of the matrix
matrix   = np.zeros(N,dtype=np.float64)
scalings = np.zeros(N,dtype=np.float64)
# right-hand side and solution are duplicated
# on each processor
rhs      = np.zeros(N,dtype=np.float64)
solution = np.zeros(N,dtype=np.float64)

#
# Set the matrix to random, 
# with a little diagonal dominance
#
for row in range(N):
    matrix[row] = random.random()
    if row==procno:
        matrix[row] += .5

#
# Set the right-hand side to row sums,
# so that the solution is identically one.
# We use one long reduction.
#
comm.Allreduce(matrix,rhs,MPI.SUM)

#
# Now iterate over the columns,
# using the diagonal element as pivot
#
for pivot_column in range(N):
    if pivot_column==procno:
        pivot = matrix[pivot_column]
        scalings = matrix/pivot
    #
    # Exercise:
    # make sure that everyone knows the scaling factors
    #
    ##solution
    comm.Bcast(scalings,root=pivot_column)
    ##solved

    #
    # Now update the matrix.
    # Answer for yourself: why is there no loop over the columns?
    #
    for row in range(N):
        if row==pivot_column:
            continue
        matrix[row] -= scalings[row]*matrix[pivot_column]
        # also update the right-hand side
        rhs[row] -= scalings[row]*rhs[pivot_column]

#
# Check that we have swept the matrix clean
#
for row in range(N):
    if row==procno:
        continue
    if abs(matrix[row])>1.e-14:
        print("Wrong value at [%d,%d]: %e" % (row,procno,matrix[row]))

#
# Check correctness of the solution
#
local_solution = rhs[procno]/matrix[procno]
comm.Gather(local_solution,solution,root=0)
if procno==0:
    success = 1
    for row in range(N):
        if abs(solution[row]-1)>1.e-13:
            print("Wrong solution at [%d]: %e" % (row,solution[row]))
            success = 0
    if success>0:
        print("Success: solution correctly computed everywhere")
