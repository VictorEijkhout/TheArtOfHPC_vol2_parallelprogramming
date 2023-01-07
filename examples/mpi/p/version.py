#!/usr/bin/env python3
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2022
####
#### version.py : MPI version information
####
################################################################
################################################################

from mpi4py import MPI

##codesnippet mpiversionp
print(MPI.Get_version())
print(MPI.VERSION)
print(MPI.SUBVERSION)
##codesnippet end

