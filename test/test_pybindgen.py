#! /usr/bin/env python

import sys
sys.path.append("../libs/pybindgen-0.16.0/")

import pybindgen
from pybindgen import FileCodeSink
from pybindgen.gccxmlparser import ModuleParser

def my_module_gen():
    #module_parser = ModuleParser('a1', '::')
    module_parser = ModuleParser('pypm', '::')
    module = module_parser.parse(['libpm.h'], ["/usr/include/", "lib/"])
    module.add_include('"libpm.h"')
    
    pybindgen.write_preamble(FileCodeSink(sys.stdout))
    module.generate(FileCodeSink(sys.stdout))

if __name__ == '__main__':
    my_module_gen()
