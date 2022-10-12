#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-2020
####
#### scatterwrite.py : MPI python exercise for MPI I/O
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import struct
import time

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## Each process has a buffer of length `nwords'
## containing consecutive integers
##
nwords = 3
output_data = np.empty(nwords,dtype=np.intc)
for iw in range(nwords):
    output_data[iw] = procno*nwords + iw + 1


##
## Create the output file and open for write
##
mpifile = MPI.File.Open\
          (comm,"scatterwrite.dat",
           amode=MPI.MODE_CREATE+MPI.MODE_WRONLY)


##
## Exercise:
## -- set a file view so that each processor writes at a different offset
## -- note: offsets are in bytes
##
scattertype = MPI.INT.Create_vector(
#### your code here ####
)
scattertype.Commit()

sizeofint = np.dtype('intc').itemsize

mpifile.Set_view(
#### your code here ####
    scattertype)

nwriters = nprocs
if procno<nwriters:
    mpifile.Write( [output_data,nwords,MPI.INT] )

scattertype.Free()
mpifile.Close()

if procno==0:
    f = open("scatterwrite.dat","rb")
    location = 0
    for iw in range(nwords):
        for ip in range(nwriters):
            bytestring = f.read(sizeofint)
            ## python 3:
            #### fromfile = int.from_bytes( bytestring )
            ## python 2
            fromfile = struct.unpack("<L",bytestring)[0]

            itest = iw+1 + nwords*ip
            if fromfile!=itest:
                print("Error: @location %d, got %d, s/b %d" % \
                      (location,itest,fromfile))
            location += 1
    f.close()

if procno==0:
    print("Finished: all tasks correctly concluded.")
