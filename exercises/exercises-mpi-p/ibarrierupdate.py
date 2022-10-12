#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-8
####
#### ibarrierupdate.py : exercise for ibarrier testing
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

## Start the random number generator
random.seed(procno)

##
## How many processes are we going to send to?
##
n_destinations = int( random.random() * nprocs )
send_data = np.zeros(n_destinations,dtype=np.float32)
receive_data = np.zeros(1,dtype=np.float32)
send_requests = [ None ] * n_destinations

##
## Pick random processes to send to, and post Isend
##
for idestination in range(n_destinations):
    destination = int( random.random() * nprocs )
    print("[%d] send to %d" % (procno,destination))
    send_requests[idestination] \
        = comm.Isend( send_data[idestination], destination )

##
## When you're done sending, post a non-blocking barrier
print("[%d] posting barrier" % procno)
final_barrier = [ None ]
final_barrier = comm.Ibarrier()

step = 0
while True:
    step += 1
    ##
    ## Exercise part 1:
    ## -- use MPI_Test to determine when the barrier is completed;
    ##    in that case you can quite
    ##
    all_done_flag = False
    all_done_flag = final_barrier.Test()
#### your code here ####

    ##
    ## Exercise
    ## -- use MPI_Iprobe to test if there is a message
    ## part 1: if there is no message, skip to the next iteration
    ##
    message_flag = False
    status=MPI.Status()
    message_flag = comm.Iprobe(source=MPI.ANY_SOURCE,tag=MPI.ANY_TAG,status=status)
#### your code here ####

    ##
    ## part 2: the message can come from anywhere, so
    ##    you need to inspect the status to find the source and tag
    ##
#### your code here ####
    comm.Recv(receive_data,source,tag=MPI.ANY_TAG)
    print("[%d] received from %d" % (procno,source))

print(f"[{procno}] concluded after {step} steps")

if procno==0:
    print("Finished")

