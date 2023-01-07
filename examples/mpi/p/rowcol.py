#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2021
####
#### rowcol.py : illustration of rows and column in a matrix
####
################################################################
################################################################

import numpy as np
import random # random.randint(1,N), random.random()
from mpi4py import MPI

comm = MPI.COMM_WORLD
procid = comm.Get_rank()
nprocs = comm.Get_size()
if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

sender = 0; receiver = 1; the_other = 1-procid

##codesnippet pmatcolsend
rowsize = 4; colsize = 5
coltype = MPI.INT.Create_vector(4, 1, 5)
coltype.Commit()
columntosend = 2
##codesnippet end

if procid == sender:
    matrix = np.array([[1, 2, 3, 4, 5],
                       [6, 7, 8, 9,10],
                       [11,12,13,14,15],
                       [16,17,18,19,20]], dtype=intc)
    ##codesnippet pmatcolsend
    comm.Send\
        ( [np.frombuffer(matrix.data, intc,
                         offset=columntosend*np.dtype('intc').itemsize),
           1,coltype],
          receiver)
    ##codesnippet end
elif procid==receiver:
    matrix = np.zeros([4,5],dtype=intc)
    comm.Recv\
        ( [np.frombuffer(matrix.data, intc, 
                         offset=columntosend*np.dtype('intc').itemsize),
           1,coltype],
          source=sender)
    print(matrix)
