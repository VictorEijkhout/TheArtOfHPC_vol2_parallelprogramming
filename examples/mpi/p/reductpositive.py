################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2020
####
#### reductpositive.py : user-defined reduction
####
################################################################
################################################################

import numpy as np
import random
from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

##codesnippet pmpirwz
def reduceWithoutZero(in_buf, inout_buf, datatype):
    typecode = MPI._typecode(datatype)
    assert typecode is not None ## check MPI datatype is built-in
    dtype = np.dtype(typecode)

    in_array = np.frombuffer(in_buf, dtype)
    inout_array = np.frombuffer(inout_buf, dtype)

    n = in_array[0]; r = inout_array[0]
    if n==0:
        m = r
    elif r==0:
        m = n
    elif n<r:
        m = n
    else:
        m = r
    inout_array[0] = m
##codesnippet end

ndata = 10
data = np.zeros(10,dtype=intc)
data[:] = [2,3,0,5,0,1,8,12,4,0]

if nprocs>ndata:
    print("Too many procs for this example: at most %d\n" %ndata)
    sys.exit(1)
    
#
# compute reduction by hand
#
mreduct=2000000000
for i in range(nprocs):
    if data[i]<mreduct and data[i]>0:
        mreduct = data[i]

##codesnippet opcreatep
rwz = MPI.Op.Create(reduceWithoutZero)
positive_minimum = np.zeros(1,dtype=intc)
comm.Allreduce(data[procid],positive_minimum,rwz);
##codesnippet end

#
# check that the distributed result is the same as sequential
#
if mreduct!=positive_minimum:
    print("[%d] Result %d should be %d\n" % \
          procid,positive_minimum,mreduct)
elif procid==0:
    print("User-defined reduction successful: %d\n" % positive_minimum)
