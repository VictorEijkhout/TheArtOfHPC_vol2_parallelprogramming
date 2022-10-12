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

import math
import sys
from petsc4py import init
init(sys.argv)
from petsc4py import PETSc

comm = PETSc.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()


hasn = PETSc.Options().hasName("n")
if hasn:
    PETSc.Sys.Print("detecting n option")
n = PETSc.Options().getInt("n",10) # default is not optional

x = PETSc.Vec().create(comm=comm)
x.setType(PETSc.Vec.Type.MPI)
x.setSizes([n,PETSc.DECIDE])
x.setFromOptions()

y = x.duplicate()

x.set(1)
#x.view()
y.set(2)
#y.view()

# inprod = np.zeros(1,dtype=np.float64)

####
#### Exercise 1:
#### - compute the inner product of x and y
#### - print the result, only from process zero
####
#### your code here ####

####
#### Exercise 2:
#### - scale x down by the just computed inner product
#### - take the 2-norm of the scaled vector x, print the result
####
scaling = 1./inprod
#### your code here ####

