#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### interleave.py : scattering a vector datatype
####
#### NOTE: this example does not work
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

print("Comm size:",comm.Get_size())

sender = 0
count = 5
if procid==sender:
    source = np.empty(nprocs*count,dtype=int)
    for i in range(nprocs*count):
        source[i] = i
else: source = None
target = np.empty(count,dtype=int)

counts = np.empty(nprocs,dtype=int)
displs = np.empty(nprocs,dtype=int)

# for i in range(nprocs*count,nprocs*nprocs*count):
#     source[i] = -37
for i in range(nprocs):
    counts[i] = 1; displs[i] = i;

vectortype = MPI.INT.Create_vector(count,1,nprocs)
vectortype.Commit()

##
## source contains [ 0, ... nprocs, ... 2*nprocs, ... ... count*nprocs ]
## each processor gets `count' elements, spaced `nprocs' apart:
## 0 : [ 0, nprocs, 2*nprocs, ... ]
## 1 : [ 1, 1+npocs, 1+2*npocs, ... ]
##
if procid==sender:
    print(source)
comm.Scatterv( [source,counts,displs,vectortype], [target,count,MPI.INT], sender)
print("[%d] %s" % (procid,str(target)))

for i in range(count):
    if target[i]!=nprocs*i+procid:
        print("Mismatch: proc %d, elt %d is %d s/b %d" % (procid,i,target[i],nprocs*i+procid))

if procid==0:
    vectortype.Free()
