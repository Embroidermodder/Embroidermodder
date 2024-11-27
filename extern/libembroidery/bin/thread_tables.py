#!/usr/bin/env python3

"""
Tool for making EmbThread tables from csv data files.

This file is part of libembroidery.
"""

import sys
import csv

fname = sys.argv[-1]

print("""/*
 * This file is part of libembroidery.
 * Copyright 2018-2024 The Embroidermodder Team
 */

#include "../embroidery.h"

const EmbThread %s[] = {
""" % (fname[5:-4]))

a = csv.reader(open(fname, "r"))
for line in a:
    if line[0] == "Catalog Code":
        continue
    if len(line[2]) != 8:
        print("ERROR")
        break
    r = int(line[2][2:4], 16)
    g = int(line[2][4:6], 16)
    b = int(line[2][6:], 16)
    print("    {{%d, %d, %d}, \"%s\", \"%s\"}," % (r, g, b, line[1], line[0]))
print("""
};
""")
