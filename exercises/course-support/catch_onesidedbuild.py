#!/usr/bin/env python

import re
import sys

indict = {}; outdict = {}
for line in sys.stdin:
    line = line.strip()
    if re.match('[0-9]+$',line):
        try :
            NP = int(line)
        except:
            print("Could not parse line for int: <<%s>>" % line)
            sys.exit(1)
        print("Analyzing %d processes" % NP)
        continue
    if not re.match(r'\[',line): continue
    number = re.sub(r'\[',"",re.sub(r'\]',"",line)).split()[0]
    if re.search("wrote",line):
        outputs = line.split(":")[1].split()
        #print number,"wrote",outputs
        for o in outputs:
            if not o in outdict.keys():
                outdict[o] = {}
            if not number in outdict[o].keys():
                outdict[o][number] = 1
            else:
                outdict[o][number] += 1
    elif re.search("received",line):
        inputs = line.split(":")[1].split()
        #print number,"received",inputs
        p = str(number)
        indict[p] = {}
        for i in inputs:
            if not i in indict[p].keys():
                indict[p][i] = 1
            else:
                indict[p][i] += 1
#print "Sends:",outdict
#print "Recvs:",indict
    
if not indict==outdict:
    print("Success")
    sys.exit(1)
else:
    print("Mismatch:")
    for p in outdict.keys():
        ip = int(p)
        print("Proc %d was written %s times; claims %s" \
              % (ip, str(outdict[p]), str(indict[p]) ) )
    sys.exit(0)
