#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2022
####
#### pingpongbig.py : large count pingpong
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import time

comm = MPI.COMM_WORLD

ntids = comm.Get_size()
mytid = comm.Get_rank()
ntests = 100

if ntids==1:
    comm.Abort()

if mytid==0 or mytid==ntids-1:
    print("Node name:",MPI.Get_processor_name())

for megs in [10,100,1000,10000]:
    s = megs * 1000000
    if mytid==0:
        print(f"Send up to {s}")
        sdata = np.arange(s, dtype=np.float64)
        rdata = np.zeros(1, dtype=np.float64)
        starttime = MPI.Wtime()
        comm.Send( [sdata,s-1,MPI.DOUBLE],dest=ntids-1 )
        comm.Recv( rdata,source=ntids-1)
        elapsed = MPI.Wtime()-starttime
        print(f"Size={s}, elapsed time: {elapsed}")
    elif mytid==ntids-1:
        print(f"Receive up to {s}")        
        rdata = np.arange(s, dtype=np.float64)
        ##codesnippet statuscountp
        status = MPI.Status()
        comm.Recv( rdata,source=0,status=status)
        count = status.Get_count(MPI.DOUBLE)
        ##codesnippet end
        print(f"Actual count: {count}")
        # pong!
        sdata = np.zeros(1, dtype=np.float64)
        comm.Send( sdata,dest=0 )
