#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-2020
####
#### blockwrite.py : MPI python exercise for MPI I/O
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
intsize = 4
for iw in range(nwords):
    output_data[iw] = procno*nwords + iw + 1

##
## Exercise:
## -- open a file for writing;
##    if it doesn't exist yet, it needs to be created
##
filename = "blockwrite.dat"
mpifile = MPI.File.Open \
          (comm,filename,
#### your code here ####
          )

##
## Exercise:
## -- Let each process compute an offset (in bytes!) so that the
##    processes write to disjoint, successive parts of the file.
##
nwriters = 2
if procno<nwriters:
    ##
    ## Set the right offset, in ints
    ##
#### your code here ####
    ## convert to bytes by multiplying by the size of an int
    offset = offset * np.dtype('intc').itemsize

    mpifile.Write_at \
        (offset,output_data)

mpifile.Close()

##
## Check correctness of the output file
##
if procno==0:
    error = nprocs
    with open(filename,"rb") as timefile:
        print(f"Trying to read {nwriters*nwords} integers")
        location = 0
        for ip in range(nwriters):
            for iw in range(nwords):
                bytes = timefile.read(intsize)
                line  = struct.unpack('i',bytes)[0]
                #print(line)
                iline = int(line)
                if iline!=location+1:
                    error = ip
                    print("Error: location {} contains {} instead of {}"\
                          .format(location,iline,location+1))
                location += 1
    errors = error
    if errors==nprocs:
        print("Execution finished correctly")
    else:
        print("Finished: first error occurred on writer %d" % errors)
