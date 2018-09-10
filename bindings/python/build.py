#!/usr/bin/env python
# encoding: utf-8
# Copyright (C) 2018 John Törnblom
#
# This file is part of VoRF (Verifier of Random Forests).
#
# VoRF is free software: you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# VoRF is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
# for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with VoRF; see the files COPYING and COPYING.LESSER. If not,
# see <http://www.gnu.org/licenses/>.

import os
from cffi import FFI

ffibuilder = FFI()

path = os.path.dirname(__file__) or '.'
path = os.path.abspath(path)
libdir = path + '/../../lib/.libs'
incdir = path + '/../../inc'

with open(incdir + '/vorf.h') as f:
    vorf_h = f.read()
    ffibuilder.set_source('_vorf', vorf_h,
                          library_dirs=[libdir],
                          libraries=['m', 'json-c', 'vorf'])

vorf_h = ''.join([line for line in vorf_h.splitlines()
                  if not line.startswith('#')])

ffibuilder.cdef(vorf_h + '''
extern "Python" bool vorf_mapping_python_cb(void *, vorf_mapping_t*);
''')

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
    
