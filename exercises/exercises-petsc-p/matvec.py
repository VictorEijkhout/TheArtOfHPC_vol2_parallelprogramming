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
from petsc4py import init
init(sys.argv)
from petsc4py import PETSc

comm = PETSc.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## Function to create the matrix
## Let's pretend that you do not know what happens here,
## so you have no information about the matrix size and such
##
def CreateMatrix(comm,nlocal):
    A = PETSc.Mat().create(comm=comm)
    A.setType(PETSc.Mat.Type.MPIAIJ)
    procno = comm.Get_rank()
    if procno==0:
        nlocal = 1
    A.setSizes( ( (nlocal,None), (nlocal,None) ) )
    A.setPreallocationNNZ( (3,1) )
    A.assemblyBegin(PETSc.Mat.AssemblyType.FLUSH)
    A.assemblyEnd(PETSc.Mat.AssemblyType.FLUSH)

    _,nglobal = A.getSize()
    first,last = A.getOwnershipRange()
    for row in range(first,last):
        col = row; v = 2.
        A.setValue(row,col,v)
        col = row-1; v = -1.
        if col>=0:
            A[row,col] = v
        col = row+1; v = -1;
        if col<nglobal:
            A[row,col] = v
    A.assemblyBegin(PETSc.Mat.AssemblyType.FINAL)
    A.assemblyEnd(PETSc.Mat.AssemblyType.FINAL)
    return A

##
## Get a commandline argument for the size of the problem
##
nlocal = PETSc.Options().getInt("n",10) # default is not optional

##
## Call the matrix creation routine
##
A = CreateMatrix(comm,nlocal)

# just checking: screen output
##A.view()

####
#### Exercise:
#### - create a vector by finding the local size of the matrix
####
x = PETSc.Vec().create(comm=comm)
x.setType(PETSc.Vec.Type.MPI)
#### your code here ####

x.setFromOptions()
x.assemblyBegin()
x.assemblyEnd()
#print("Vec local size on %d: %s" % (procno,str(x.getSizes())))

y = x.duplicate()
x.set(1)

####
#### Exercise:
#### - fill in the correct parameters to compute
####    the matrix vector product y = Ax
####
A.mult(\
#### your code here ####
)
#y.view()

nrm = y.norm(PETSc.NormType.NORM_1)
if abs(2-nrm)>1.e-14:
    PETSc.Sys.Print("Wrong norm %e: should be 2." % nrm)
else:
    PETSc.Sys.Print("Global norm succeeds")

####
#### Second test, using local arrays
####

localvec = PETSc.Vec().create(comm=PETSc.COMM_SELF)
localvec.setType(PETSc.Vec.Type.SEQ)
localsize = y.getLocalSize()
localvec.setSizes(localsize)

####
#### Exercise:
#### -- put y's data into the local vector
####    and compute the local norm
#### Hint:
#### -- use GetArray to get the local data
#### -- use PlaceArray to set it in the localvec
####
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
