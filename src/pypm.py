#!/usr/bin/env python

from ctypes import *

cdll.LoadLibrary("../target/debug/libpm.so")
_libpm = CDLL("libpm.so")

# int read_file(char* filename, char** contents);
def read_file(file):
    _read_file = _libpm.read_file
    _read_file.argtypes = [c_char_p, POINTER(c_char_p)]
    _read_file.restype = c_int
    
    contents = c_char_p("")
    ret = _read_file(file, byref(contents))
    
    if (ret != 0):
    	raise Exception("Error in libpm.so->read_file()", ret)
    
    #print "%s\n" % repr(contents.value)
    #print "%d, %s" % (ret, contents.value)
    
    # FIXME: there seems to be soem garbage bytes at the end of the 
    #        result string. It looks like it is aligning with 8 
    #        byte blocks
    
    return contents.value
