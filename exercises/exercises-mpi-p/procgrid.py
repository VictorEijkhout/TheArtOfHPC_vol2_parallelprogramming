#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2020
####
#### procgrid.py : MPI python exercise for multiple nonblocking collectives
####
################################################################
################################################################

from mpi4py import MPI
import math
import numpy as np
import sys

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

##
## Try to arrange the processes in a grid
##
for nrows in range( int( math.sqrt(nprocs+1) ),0,-1 ):
    if nprocs%nrows==0:
        break
ncols = nprocs/nrows

if nrows==1:
    if procno==0:
        print("Number of processors is prime")
    comm.Barrier()
    comm.Abort(-1)
else:
    if procno==0:
        print("Processor grid %d x %d" % (nrows,ncols))

##
## Find the coordinates of this process
##
row_no = int(procno/ncols)
col_no = procno - ncols*row_no

##
## Make a `col_comm' communicator with all the processes in this colum
##
col_comm = comm.Split(col_no)
col_length = np.zeros(1,dtype=np.intc)
col_length[0] = col_comm.Get_size()


##
## Make a `row_comm' communicator with all the processes in this row
##
row_comm = comm.Split(row_no)
row_length = np.zeros(1,dtype=np.intc)
row_length[0] = row_comm.Get_size()

##
## Find the `col_rank': my process number in my column
##
col_rank = col_comm.Get_rank()

##
## Find the `row_rank': my process number in my row
##
row_rank = row_comm.Get_rank()

row_comm.Bcast(row_length)
col_comm.Bcast(col_length)


if row_rank!=col_no or col_rank!=row_no:
    print("[%d] = (%d,%d) wrong, should be (%d,%d)" % \
          (procno,col_rank,row_rank,row_no,col_no))
    
if procno==0:
    print("Finished")
