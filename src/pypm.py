#!/usr/bin/env python

from ctypes import *

cdll.LoadLibrary("../target/debug/libpm.so")
_libpm = CDLL("libpm.so")

class ST(Structure):
    _fields_ = [("memsize", c_int),
                ("length", c_int),
                ("text", c_char_p),
                ("error", c_int)]

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

def str_readfile(filename):
    _str_readfile = _libpm.str_readfile
    _str_readfile.argtypes = [c_char_p, c_int]
    _str_readfile.restype = ST
    
    ret = _str_readfile(filename, 0)
    print repr(ret.text)
    
    #return 0