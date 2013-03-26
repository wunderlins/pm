#!/usr/bin/env python

from ctypes import *

cdll.LoadLibrary("./libtest.so")
_libtest = CDLL("libtest.so")

class ST_TEST(Structure):
    _fields_ = [("id", c_int),
                ("string", c_char_p)]

def struct_test(id, str):
    _struct_test = _libtest.struct_test
    _struct_test.argtypes = [c_int, c_char_p]
    _struct_test.restype = ST_TEST
    
    ret = _struct_test(id, str)
    
    print ret.string