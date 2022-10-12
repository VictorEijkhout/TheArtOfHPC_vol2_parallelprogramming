#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-2020
####
#### onesidedbuild.py : put random data everywhere
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import random
import sys

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

repo=0; supervisor = nprocs-1

zero = np.zeros(1,dtype=np.intc)
one  = np.ones( 1,dtype=np.intc )

##
## make a window that is only nonzero on procno==repo
##
intsize = np.dtype('int').itemsize
winsize = 0
if procno==repo:
    winsize = intsize
the_window = MPI.Win.Allocate( winsize,intsize,comm=comm)
#mview = memoryview(the_window)
win_buffer = np.frombuffer(the_window, dtype='i4')

#  MPI_Aint zero_disp = 0;

##
## The supervisor rank initializes the window
##
the_window.Fence()
if procno==supervisor:
    the_window.Put( zero,repo )
the_window.Fence()

##
## Passive target epoch
##
readout = np.zeros( 1,dtype=np.intc )
if procno == supervisor:
    while readout[0]<nprocs-1:
        the_window.Lock(repo,MPI.LOCK_EXCLUSIVE)
        ##
        ## Exercise: read out the window's content using an atomic operation
        ##
#### your code here ####
        the_window.Unlock(repo)
        print("Supervisor readout: %d" % readout)
    print("Supervisor is done!");
else:
    ##
    ## Atomic update of the repo
    ##
    the_window.Lock(repo,MPI.LOCK_EXCLUSIVE)
    the_window.Fetch_and_op( one,readout,repo )
    the_window.Unlock(repo)
    print("[%d] adding 1 to %d" % (procno,readout) )

if procno==supervisor:
    print("Supervisor final readout %d" % readout)

##
## Illustrating that the window and the window buffer
## are not coherent, we read it out twice:
## 1. as a straight read
##
if procno==repo:
    bare_read = win_buffer[0]
    get_read = np.zeros(1,dtype=np.intc)

##
## 2. by a Win_get
##
the_window.Fence()
if procno==repo:
    the_window.Get( get_read,repo )
the_window.Fence()
if procno==repo:
    print("Window buffer unsynced=%d, synced=%d" % (bare_read,get_read) )

the_window.Free()
