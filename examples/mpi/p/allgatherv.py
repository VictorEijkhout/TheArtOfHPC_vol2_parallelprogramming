#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-6
####
#### allgatherv.py : vector allgather, task t contributes t+1 data
####
################################################################
################################################################

import numpy as np
import random # random.randint(1,N), random.random()
import sys
from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

##codesnippet bufallocp
mycount = procid+1
my_array = np.empty(mycount,dtype=np.float64)
##codesnippet end

for i in range(mycount):
    my_array[i] = procid
recv_counts = np.empty(nprocs,dtype=int)
recv_displs = np.empty(nprocs,dtype=int)

##codesnippet allgathervp
my_count = np.empty(1,dtype=int)
my_count[0] = mycount
comm.Allgather( my_count,recv_counts )

accumulate = 0
for p in range(nprocs):
    recv_displs[p] = accumulate; accumulate += recv_counts[p]
global_array = np.empty(accumulate,dtype=np.float64)
comm.Allgatherv( my_array, [global_array,recv_counts,recv_displs,MPI.DOUBLE] )
##codesnippet end

# other syntax:
# comm.Allgatherv( [my_array,mycount,0,MPI.DOUBLE], [global_array,recv_counts,recv_displs,MPI.DOUBLE] )

if procid==0:
    #print(procid,global_array)
    for p in range(nprocs):
        if recv_counts[p]!=p+1:
            print( "recv count[%d] should be %d, not %d" \
                % (p,p+1,recv_counts[p]) )
    c = 0
    for p in range(nprocs):
        for q in range(p+1):
            if global_array[c]!=p:
                print( "p=%d, q=%d should be %d, not %d" \
                    % (p,q,p,global_array[c]) )
            c += 1
    print "finished"
    
