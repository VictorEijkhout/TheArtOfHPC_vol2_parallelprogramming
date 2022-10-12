#!/usr/bin/env python

import fileinput
import re
import sys

success = 1
for line in fileinput.input():
    line = line.strip()
    print(line)
    final = re.search(r'counter at ([\-]?[0-9]*)',line)
    if final:
        count = final.groups()[0]
        count = int(count)
        print("  counter={}".format(count))
        success = success and count>=0

sys.exit(success)


