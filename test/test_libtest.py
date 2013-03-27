#!/usr/bin/env python

import sys
import pylibtest

ret = pylibtest.struct_test(4, "blah")
print ret

ret2 = pylibtest.str_readfile("Makefile")
print ret2