#!/usr/bin/env python

import sys
sys.path.append("../src/")

import pypm

c = pypm.read_file("./test_pypm.py")
#print repr(c)
print c


