#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### pingpong.py : a ping-pong program in python,
####               using  pickle'd communication
####
################################################################
################################################################

from mpi4py import MPI
import time

comm = MPI.COMM_WORLD

ntids = comm.Get_size()
mytid = comm.Get_rank()
ntests = 100

if mytid==0 or mytid==ntids-1:
    print("Node name:",MPI.Get_processor_name())

for s in [1,10,100,1000,10000,100000,1000000]:
    if mytid==0:
        data = [ 2.*i for i in range(s) ]
        starttime = MPI.Wtime()
        for test in range(ntests):
            comm.send(data,dest=ntids-1)
            rdata = comm.recv(source=ntids-1)
        elapsed = MPI.Wtime()-starttime
        print("Size=%d, elapsed time: %e" % (s,elapsed))
        c = data==rdata
        if not c:
            print("oops",data,rdata)
    elif mytid==ntids-1:
        for test in range(ntests):
            zdata = comm.recv(source=0)
            comm.send(zdata,dest=0)
