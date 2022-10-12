#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-9
####
#### onenorm.py : use-defined one-norm
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import sys

def one_norm(in_buf, inout_buf, datatype):
    # This mapping MPI datatype -> NumPy dtype
    # only works for built-in MPI datatypes
    typecode = MPI._typecode(datatype)
    assert typecode is not None # check MPI datatype is built-in
    dtype = np.dtype(typecode)
    # `in_buf`, `inout_buf` are buffer-like objects exposing raw bytes
    # don't use them directly but convert them to numpy arrays
    in_array = np.frombuffer(in_buf, dtype) # `in_array` and `in_buf` share memory
    inout_array = np.frombuffer(inout_buf, dtype) # `inout_array` and `inout_buf` share memory
    # `inout_buf` is input-output, treat `inout` accordingly

    ##
    ## Exercise:
    ## -- write the correct reduction step
    ##    for computing a 1-norm
    ##
#### your code here ####

comm = MPI.COMM_WORLD

####
#### Exercise:
#### - use the Get_size and Get_rank methods of the COMM class
####

nprocs = comm.Get_size()
procno = comm.Get_rank()

#raise Exception("This code is incomplete")

####
#### This program can be called as sumsquares.py 12345
#### where the numeric argument is the number of elements
#### on each processor. There is a default if you don't
#### specify this
####
data = np.zeros(1,dtype=np.float32)
data[0] = -1.
data_one_norm = np.zeros(1,dtype=np.float32)

####
#### Create the operator,
#### do the reduction,
#### free the operator
####
calc_one = MPI.Op.Create(one_norm)
comm.Allreduce( data,data_one_norm,calc_one )
MPI.Op.Free(calc_one)

####
#### Correctness test:
#### check that the distributed result is the same as predicted
####

actual_norm = np.zeros(1,dtype=np.float32)
actual_norm[0] = nprocs
if data_one_norm[0]!=actual_norm[0]:
    print("[{}] Result {} should be {}"\
          .format(procno,data_one_norm[0],actual_norm[0]))
elif procno==0:
    print("User-defined reduction successful: {}".format(data_one_norm[0]))
