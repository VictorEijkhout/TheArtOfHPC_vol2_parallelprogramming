#!/usr/bin/env python
################################################################
####
#### This program is part of the tutorial
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

import fivepoint

comm = PETSc.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## Get a commandline argument for the size of the problem
##
domain_size = PETSc.Options().getInt("n",100) # default is not optional
PETSc.Sys.Print( "Using domain size %d" % domain_size)

##
## Read the amount of unsymmetry to add to the matrix
##
unsymmetry = PETSc.Options().getReal("unsymmetry",0.)

##
## Call the matrix creation routine
##
A = fivepoint.FivePointMatrix(comm,domain_size,unsymmetry)

# just checking: screen output
#A.view()

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
