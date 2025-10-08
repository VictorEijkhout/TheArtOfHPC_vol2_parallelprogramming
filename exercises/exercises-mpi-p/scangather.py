#!/usr/bin/env python
################################################################
################################################################
####
#### This program file is part of the book and course
####   Parallel Computing for Science and Engineering
#### by Victor Eijkhout, copyright 2013-2023
####
#### scangatherp.py : scanning and gathering
####
################################################################
################################################################

from mpi4py import MPI
import math
import numpy as np
import random
import sys

comm = MPI.COMM_WORLD

nprocs = comm.Get_size()
procno = comm.Get_rank()

##
## How many elements do I have locally?
#### Allocate space.
##
random.seed(procno)
my_number_of_elements = np.zeros(1,dtype=np.intc)
my_number_of_elements[0] = int( random.random() * nprocs )
my_first_index = np.zeros(1,dtype=np.intc)
my_first_index[0] = 0

## 
##  Exercise:
##  - determine the index of your first element.
##    Which routine do you use here?
##

## **** your code here **** ##
#comm.YourScan(my_number_of_elements,my_first_index,MPI.SUM)
print( f"Proc {procno} has {my_number_of_elements[0]} elements, range [ {my_first_index[0]},{my_first_index[0]+my_number_of_elements[0]} )" )

## 
##  Create a local array of size `my_number_of_elements'
##  Fill in this local array
##

total_number_of_elements = np.zeros(1,dtype=np.intc)
my_elements = np.zeros(my_number_of_elements,dtype=np.intc)
for i_element in range(my_number_of_elements[0]):
    my_elements[i_element] = my_first_index+i_element

comm.Reduce( my_number_of_elements,total_number_of_elements,MPI.SUM,0)
if procno==0:
    print( f"Total number of elements: {total_number_of_elements}" )

## 
##  Gather all local arrays into one global
##
## how many elements from each process?
size_buffer = np.zeros(nprocs,dtype=np.intc)
comm.Gather(
## **** your code here **** ##
   );
## where are they going to go in the big buffer?
displ_buffer = np.zeros( nprocs,dtype=np.intc )
comm.Gather(
## **** your code here **** ##
   );
## now create the big buffer
gather_buffer = np.zeros(total_number_of_elements,dtype=np.intc)

## 
##  Use Gatherv to collect the small buffers into a big one
##
comm.Gatherv(
## **** your code here **** ##
   )

## 
##  Print the gathered material
##
if procno==0:
  print(f"Gathered: {gather_buffer}")
  # for i_element in range(total_number_of_elements[0]):
  #   print(" {gather_buffer[i_element]}")


