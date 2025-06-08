#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-8
####
#### prime.py : prime number finding
####
################################################################
################################################################

from mpi4py import MPI

comm = MPI.COMM_WORLD

bignum = 2000000111
maxfactor = 45200

####
#### Exercise:
#### - parallelize the loop so that each process
####   tries different candidate numbers
#### - if a process finds a factor, print it to the screen
####

####
#### set the variables nprocs / procno
####
##solution
nprocs = comm.Get_size()
procno = comm.Get_rank()
##solved

####
#### divide the work over processes
####
##solution
mylow = procno*(maxfactor//nprocs)
myhigh = (procno+1)*(maxfactor//nprocs)
if mylow<2:
    mylow = 3
if procno==nprocs-1:
    myhigh  = maxfactor
##solved

for myfactor in range(
        ##solution
        mylow,myhigh
        ##solved
        ):
    if bignum%myfactor==0:
        print("Processor %d found factor %d" % (procno,myfactor))



