#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-2020
####
#### cubegatherp.py : use subarray type
####
################################################################
################################################################

from mpi4py import MPI
import math
import numpy as np
import random
import sys

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## See if you can arrange the processes
## in a perfect cube. If not, quit.
##
procs_per_side = int( math.pow(nprocs+1,1./3) )
if pow(procs_per_side,3) != nprocs:
    if procno==0:
        print("Number of processors needs to be a perfect square")
    comm.Barrier()
    comm.Abort(0)

##
## Every process gets its own data and sends it to process zero.
## Exercise: why are we using Isend here?

mydata = np.zeros( 1,dtype=np.intc )
mydata[0] = procno+1
send_request = comm.Isend( mydata,0 )

if procno==0:
    cubedata = np.zeros(nprocs,dtype=np.intc)
    sizes    = np.array( [procs_per_side,procs_per_side,procs_per_side] )
    subsizes = np.array( [1,1,1] )
    for proci in range(procs_per_side):
        for procj in range(procs_per_side):
            for prock in range(procs_per_side):
                ##
                ## Set up a subarray type describing there the data
                ## from process (proci,procj,prock) goes in the cube
                ##
#### your code here ####
                insert_type.Commit()
                sender = ( proci*procs_per_side + procj )*procs_per_side + prock
                comm.Recv( [cubedata,1,insert_type],sender )
    print("Cube received: {}".format(cubedata))
    error = 0
    for p in range(nprocs):
        if cubedata[p]!=p+1:
            error += 1
    if error==0:
        print("Finished. Cube correctly gathered")
    else:
        print("There were errors")

##
## Conclude the sends
##
MPI.Request.Wait( send_request )
                
