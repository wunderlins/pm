#!/usr/bin/env python

from ctypes import *

cdll.LoadLibrary("../src/libpm.so")
_libpm = CDLL("libpm.so")

# int read_file(char* filename, char** contents);
def read_file(s):
    _append = _libpm.read_file
    _append.argtypes = [c_char_p, c_char_p]
    _append.restype = c_int
    return _append(s)