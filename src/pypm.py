#!/usr/bin/env python
# $Id$

from ctypes import *

cdll.LoadLibrary("../target/debug/libpm.so")
_libpm = CDLL("libpm.so")

"""
struct string_t {
    int memsize;
    int length;
    char *text;
    int error;
};
typedef struct string_t string;
"""
class ST_STRING(Structure):
    _fields_ = [("memsize", c_int),
                ("length", c_int),
                ("text", c_char_p),
                ("error", c_int)]
    
    def __str__(self):
        return self.text
    
    def __repr__(self):
        return repr(self.text)
    
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

# string str_readfile(char *filename, int bs)
def str_readfile(filename):
    _str_readfile = _libpm.str_readfile
    _str_readfile.argtypes = [c_char_p, c_int]
    _str_readfile.restype = ST_STRING

    ret = _str_readfile(filename, 512)
    
    return ret
    """
    return [("memsize", ret.memsize),
            ("length", ret.length),
            ("text", ret.text),
            ("error", ret.error)]
    """
