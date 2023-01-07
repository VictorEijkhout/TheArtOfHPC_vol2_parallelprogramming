#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-2022
####
#### commdup.py : illustrating communicators and libraries
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

other = nprocs-procid-1
my_requests = [ None ] * 2
my_status = [ MPI.Status() ] * 2
sendbuffer = np.empty(1,dtype=int)
recvbuffer = np.empty(1,dtype=int)

#snippet catchlibp
class Library():
    def __init__(self,comm):
        # wrong: self.comm = comm
        self.comm = comm.Dup()
        self.other = self.comm.Get_size()-self.comm.Get_rank()-1
        self.requests = [ None ] * 2
    def __del__(self):
        if self.comm.Get_rank()==0: print(".. freeing communicator")
        self.comm.Free()
    def communication_start(self):
        sendbuf = np.empty(1,dtype=int); sendbuf[0] = 37
        recvbuf = np.empty(1,dtype=int)
        self.requests[0] = self.comm.Isend( sendbuf, dest=other,tag=2 )
        self.requests[1] = self.comm.Irecv( recvbuf, source=other )
    def communication_end(self):
        MPI.Request.Waitall(self.requests)

mylibrary = Library(comm)
my_requests[0] = comm.Isend( sendbuffer,dest=other,tag=1 )
mylibrary.communication_start()
my_requests[1] = comm.Irecv( recvbuffer,source=other )
MPI.Request.Waitall(my_requests,my_status)
mylibrary.communication_end()
#snippet end

if my_status[1].Get_tag()==2:
    print("Caught wrong message!")
