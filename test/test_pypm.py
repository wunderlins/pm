#!/usr/bin/env python

import sys
sys.path.append("../src/")

import pypm

c = pypm.read_file("./test_pypm.py")
print repr(c)

c1 = pypm.str_readfile("./test_pypm.py")
print c1 # or print c1.text, or print repr(c1)

