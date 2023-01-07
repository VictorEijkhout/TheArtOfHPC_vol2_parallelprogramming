#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
#### "Parallel Computing"
#### by Victor Eijkhout, copyright 2013-5
####
#### persist.py : test of persistent communication
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

nexperiments = 10
nsizes = 6
times = np.empty(nsizes,dtype=np.float64)
src = 0; tgt = nprocs-1

#
# ordinary communication
#
size = 1
if procid==src:
    print("Ordinary send/recv")
for isize in range(nsizes):
    #snippet persistp
    requests = [ None ] * 2
    sendbuf = np.ones(size,dtype=int)
    recvbuf = np.ones(size,dtype=int)
    if procid==src:
        print("Size:",size)
        times[isize] = MPI.Wtime()
        for n in range(nexperiments):
            requests[0] = comm.Isend(sendbuf[0:size],dest=tgt)
            requests[1] = comm.Irecv(recvbuf[0:size],source=tgt)
            MPI.Request.Waitall(requests)
            sendbuf[0] = sendbuf[0]+1
        times[isize] = MPI.Wtime()-times[isize]
    elif procid==tgt:
        for n in range(nexperiments):
            comm.Recv(recvbuf[0:size],source=src)
            comm.Send(recvbuf[0:size],dest=src)
    #snippet end
    size *= 10
if procid==src:
    print("Timings:",times)

#
# ordinary communication
#
size = 1
requests = [ None ] * 2
if procid==src:
    print("Persistent send/recv")
for isize in range(nsizes):
#snippet persistpy
    sendbuf = np.ones(size,dtype=int)
    recvbuf = np.ones(size,dtype=int)
    if procid==src:
        print("Size:",size)
        requests[0] = comm.Send_init(sendbuf[0:size],dest=tgt)
        requests[1] = comm.Recv_init(recvbuf[0:size],source=tgt)
        times[isize] = MPI.Wtime()
        for n in range(nexperiments):
            MPI.Prequest.Startall(requests)
            MPI.Prequest.Waitall(requests)
            sendbuf[0] = sendbuf[0]+1
        times[isize] = MPI.Wtime()-times[isize]
    elif procid==tgt:
        for n in range(nexperiments):
            comm.Recv(recvbuf[0:size],source=src)
            comm.Send(recvbuf[0:size],dest=src)
#snippet end
    size *= 10
if procid==src:
    print("Timings:",times)

