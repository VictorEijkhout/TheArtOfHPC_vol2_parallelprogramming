#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-8
####
#### stridesend.py : MPI python exercise for vector type
####
################################################################
################################################################

from mpi4py import MPI
import numpy as np
import sys

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

if nprocs<2:
    print("C'mon, get real....")
    sys.exit(1)

## process zero will do the sending
sender = 0
## how many elements to each process?
count = 6

## allocate source data only on the sender
if procno==sender:
    source = np.empty(nprocs*count,dtype=np.intc)
    for i in range(nprocs*count):
        source[i] = i
else: source = None

## everyone gets a target array
target = np.zeros(count,dtype=np.intc)

##
## Now set up the vector type and send to everyone but yourself
##
if procno==sender:
    ##
    ## Make a vector type for sending `count' strided elements
    ## Important hint: the same type can be used for all sends
    ##
    vectortype = MPI.INT.Create_vector(
#### your code here ####
    )
    vectortype.Commit()
    for p in range(nprocs):
        ##
        ## Do not send to yourself
        ## Exercise: how would you send to yourself
        ##     without risk of deadlock?
        ##
        if p==sender:
            continue
        ##
        ## Send to process p
        ## Exercise: how do you determine what data to send?
        ##
        comm.Send(
            [
                ##
                ## Fill in the buffer/count/datatype triple
                ## Note: buffer needs to be an array !!!
                ##
#### your code here ####
                ],
            p
            )
    vectortype.Free()
else:
    ##
    ## If you are not the sender, receive data and check correctness
    ##
    comm.Recv([target,count,MPI.INT],0)
    ## disabled test
    for i in range(count):
        if target[i]!=nprocs*i+procno:
            print("Mismatch: proc %d, elt %d is %d s/b %d" % \
                  (procno,i,target[i],nprocs*i+procno))

if procno==0:
    print("Finished")
