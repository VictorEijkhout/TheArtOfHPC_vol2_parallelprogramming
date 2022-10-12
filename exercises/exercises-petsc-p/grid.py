#!/usr/bin/env python
################################################################
####
#### This makefile is part of the tutorial
#### `Introduction to the PETSc library'
#### by Victor Eijkhout eijkhout@tacc.utexas.edu
####
#### copyright Victor Eijkhout 2012-2020
####
################################################################

import math
import sys
import numpy as np
from petsc4py import init
init(sys.argv)
from petsc4py import PETSc

comm = PETSc.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()
if nprocs%2!=0:
    print("Number of processors needs to be even")
    sys.exit(1)

isize = 100; jsize = 100
grid = PETSc.DMDA().create\
       (dim=2,dof=1,sizes=[isize,jsize],
        stencil_type=PETSc.DMDA.StencilType.STAR,
        stencil_width=1,
        comm=comm)
grid.view()

A = grid.createMatrix()

## no DMDAGetLocalInfo ?

##
## Fill in the elements of the matrix
##
row = PETSc.Mat.Stencil() # (np.float64)
col = PETSc.Mat.Stencil() # (np.float64)
for i in range(isize):
    for j in range(jsize):
        row.index = (i,j,0)
        col.index = (i,j,0)
        value = 4.
        A.setValueStencil(row, col, value)
        value = -1.
        if i<isize-1:
            col.index = (i+1,j,0)
            A.setValueStencil(row, col, value)
        if i>0:
            col.index = (i-1,j,0)
            A.setValueStencil(row, col, value)
        if j<jsize-1:
            col.index = (i,j+1,0)
            A.setValueStencil(row, col, value)
        if j>0:
            col.index = (i,j-1,0)
            A.setValueStencil(row, col, value)

A.assemblyBegin(PETSc.Mat.AssemblyType.FINAL)
A.assemblyEnd(PETSc.Mat.AssemblyType.FINAL)

##
#### your code here ####
#### your code here ####

####
#### Compute the norm:
#### should be 1 on first & last process,
#### 0 elsewhere
####
nrm = localvec.norm(PETSc.NormType.NORM_1)
if procno==0 or procno==nprocs-1:
    if abs(nrm-1.)>1.e-14:
        print("Local norm incorrect on %d: %e" % (procno,nrm))
    else:
        print("Local norm succeeds on %d" % procno)
else:
    if abs(nrm)>1.e-14:
        print("Local norm incorrect on %d: %e" % (procno,nrm))
    else:
        print("Local norm succeeds on %d" % procno)
