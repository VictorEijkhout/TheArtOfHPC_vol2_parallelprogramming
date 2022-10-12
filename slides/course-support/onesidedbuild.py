#!/usr/bin/env python3

import re
import sys

debug = False

if len(sys.argv)<3:
    print("onesidedbuild test script needs arguments NP OUTFILE")
    sys.exit(1)

np = int(sys.argv[1])
file = sys.argv[2]
if not re.search(".out_total$",file):
    print("onesidedbuild test script needs argument with extension out_total")
    sys.exit(1)

reconstruct = [ [] for i in range(np) ]
reportstruct = [ [] for i in range(np) ]
with open(file,"r") as trace:
    for line in trace:
        wrote = re.search("\[([0-9]+)\] wrote its id to:([0-9 ]+)$",line)
        if wrote:
            wrote = wrote.groups()
            source = int(wrote[0])
            targets = wrote[1].split()
            if debug: print("{} wrote to {}".format(source,targets))
            for t in targets:
                t = int(t)
                reconstruct[t].append(source)
            reconstruct[source].sort()
        receive = re.search("\[([0-9]+)\] received [0-9]+:([0-9 ]+)$",line)
        if receive:
            receive = receive.groups()
            target = int(receive[0])
            sources = receive[1].split()
            if debug: print("{} received {}".format(target,sources))
            for s in sources:
                s = int(s)
                reportstruct[target].append(s)
            reportstruct[target].sort()

if debug: 
    for p in range(len(reconstruct)):
        print("[{}] received: {}".format(p,reconstruct[p]))
    for p in range(len(reportstruct)):
        print("[{}] received: {}".format(p,reportstruct[p]))
import functools
import operator
for p in range(len(reportstruct)):
    matchp = all( [ lambda x,y:x==y for x,y in zip(reconstruct,reportstruct) ] )
if matchp:
    print("All matches")
else:
    print("Mismatch"); sys.exit(1)
