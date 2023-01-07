#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2021
####
#### inttype.py : illustrating integer types
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import sys

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

if nprocs<2:
    print("Needs 2 procs for send/recv test....")
    sys.exit(1)

## set up sender and receiver
sender = 0; receiver = nprocs-1
## how many elements to each process?
count = 6

## Send a contiguous buffer as numpy ints
if procno==sender:
    data = np.empty(count,dtype='int32')
    for i in range(count):
        data[i] = i
    comm.Send( data, receiver )
elif procno==receiver:
    data = np.empty(count,dtype='int32')
    comm.Recv( data, sender )
    print(data)

## Send a strided buffer as numpy ints
## this is wrong because numpy ints are not C ints
if procno==sender:
    print("Int Size")
##codesnippet npintc
    sizeofint = np.dtype('int32').itemsize
    print("Size of numpy int32: {}".format(sizeofint))
##codesnippet end
    print(" .. int size")

    data = np.empty(2*count,dtype='int32')
    for i in range(2*count):
        data[i] = i
    vectortype = MPI.INT.Create_vector(count,1,2)
    vectortype.Commit()
    comm.Send( [data,1,vectortype], receiver )
elif procno==receiver:
    data = np.empty(count,dtype='int32')
    comm.Recv( data, sender )
    print( f"int32 data: {data}" )
        
## Send strided buffer as C ints
if procno==sender:
    print("Int Size")
##codesnippet npintc
    sizeofint = np.dtype('intc').itemsize
    print("Size of C int: {}".format(sizeofint))
##codesnippet end
    print(" .. int size")

    data = np.empty(2*count,dtype='intc')
    for i in range(2*count):
        data[i] = i
    vectortype = MPI.INT.Create_vector(count,1,2)
    vectortype.Commit()
    comm.Send( [data,1,vectortype], receiver )
elif procno==receiver:
    data = np.empty(count,dtype='intc')
    comm.Recv( data, sender )
    print( f"intc data: {data}" )
