#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-9
####
#### bcast.py : broadcast of numpy float
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np

from functools import reduce
import sys

comm = MPI.COMM_WORLD

procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

##codesnippet pyfrombuf
datatype = np.intc
elementsize = datatype().itemsize
##codesnippet end
## or np.dtype('intc').itemsize
##codesnippet pyfrombuf
typechar = datatype().dtype.char
##codesnippet end
if procid==0:
    print("int size:",elementsize)
    print(typechar)

##codesnippet pyfrombuf
buffer = np.zeros( [nprocs,nprocs], dtype=datatype)
buffer[:,:] = -1
for proc in range(nprocs):
    if procid==proc:
        buffer[proc,:] = proc
    comm.Bcast\
        ( [ np.frombuffer\
            ( buffer.data,
              dtype=datatype,
              offset=(proc*nprocs+proc)*elementsize ),
            nprocs-proc, typechar ],
          root=proc )
##codesnippet end
if procid==nprocs-1:
    print(buffer)
