#!/usr/bin/env python

import sys
sys.path.append("../src/")

import pypm

c = pypm.str_readfile("./Makefile")
print repr(c)
#print c



