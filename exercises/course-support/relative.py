#!/usr/bin/env python

####
#### relative.py
####
#### return 1 if two commandline arguments are relatively close
#### return 0 if not, or if incorrectly called
####

import sys

if len(sys.argv)<3:
    print("Need two numerical arguments, not <<{}>>".\
          format(sys.argv))
    print(0); sys.exit(1)

try :
    x = float(sys.argv[1])
    y = float(sys.argv[2])
except:
    print("Could not convert arguments <<{}>>".\
          format(sys.argv))
    print(0); sys.exit(1)

if abs( (x-y)/max(abs(x),abs(y)) )<1.e-5:
    print(1)
else:
    print(0)
    
