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
#### your code here ####

####
#### divide the work over processes
####
#### your code here ####

for myfactor in range(
#### your code here ####
        ):
    if bignum%myfactor==0:
        print("Processor %d found factor %d" % (procno,myfactor))



