#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-2022
####
#### pingpong.py : MPI python exercise for send/recv
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import time

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()
ntests = 10000

if procno==0 or procno==nprocs-1:
    print("Node name:",MPI.Get_processor_name())

## Determine who will be communicating:
## set processA, processB
#### your code here ####

##
## Do a range of test sizes
##

for s in [1,10,100,1000,10000,100000,1000000]:
    if procno==processA:
        ####
        #### Originating process:
        ####
        ##
        ## create send_data and recv_data numpy arrays
        ##
#### your code here ####
        ## initialize send data buffer
        for i in range(s):
            send_data[i] = i+1
        starttime = MPI.Wtime()
        for test in range(ntests):
            ##
            ## Originating process:
            ## send the `ping' and receive the `pong'
            comm.Send([send_data,MPI.DOUBLE],dest=
#### your code here ####
            )
            comm.Recv([recv_data,MPI.DOUBLE],source=
#### your code here ####
            )
        elapsed = MPI.Wtime()-starttime
        ##
        ## Report time and detect errors
        ##
        print("Size=%d, elapsed time: %e" % (s,elapsed))
        c = send_data==recv_data
        if not c.all():
            print("oops",send_data,recv_data)
    elif procno==processB:
        ####
        #### Other process
        ####
        ##
        ## create thru_data array
        ##
#### your code here ####
        for test in range(ntests):
            ##
            ## receive the `ping' and send `pong'
            ##
            comm.Recv([thru_data,MPI.DOUBLE],source=
#### your code here ####
            )
            comm.Send([thru_data,MPI.DOUBLE],dest=
#### your code here ####
            )
