#!/usr/bin/env python

# this is for python > 2.6

from ctypes import *
cdll.LoadLibrary("libpm.so")
libpm = CDLL("libpm.so")

file   = ""
ret    = ""
c_file = c_char_p(file)
c_ret  = c_char_p(ret)
c_file = "./pm"
c_ret  = ""


