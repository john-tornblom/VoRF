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

from _vorf import ffi, lib


__version__ = ffi.string(lib.vorf_version())


def argmax(iterable):
    fvec = [float(el) for el in iterable]
    return lib.vorf_argmax(fvec, len(fvec))


def mapping_argmax(mapping):
    return lib.vorf_mapping_argmax(mapping)


@ffi.def_extern()
def vorf_mapping_python_cb(ctx, mapping):
    callback = ffi.from_handle(ctx)
    return callback(mapping)


class Forest(object):
    ptr = None

    def __init__(self, filename):
        self.ptr = lib.vorf_forest_load(filename)
        assert self.ptr
        
    def __del__(self):
        if self.ptr:
            lib.vorf_forest_del(self.ptr)

    @property
    def nb_inputs(self):
        return self.ptr.nb_inputs

    @property
    def nb_outputs(self):
        return self.ptr.nb_outputs

    def eval(self, *args):
        inputs = ffi.new('real_t[%d]' % self.nb_inputs, args)
        outputs = ffi.new('real_t[%d]' % self.nb_outputs)

        lib.vorf_forest_eval(self.ptr, inputs, outputs)
        return list(outputs)

    def forall(self, callback, domain=None):
        ctx = ffi.NULL
        bounds = ffi.new('vorf_bound_t[%d]' % self.nb_inputs)
        cb = ffi.callback('vorf_mapping_cb_t')
        
        for ind in range(self.nb_inputs):
            bounds[ind].lower = -float('inf')
            bounds[ind].upper = float('inf')

        domain = domain or list()
        for ind, bound in enumerate(domain):
            bounds[ind].lower = bound[0]
            bounds[ind].upper = bound[1]
            
        ctx = ffi.new_handle(callback)
        cb = lib.vorf_mapping_python_cb
        return lib.vorf_forest_forall(self.ptr, bounds, cb, ctx)
    
    def any(self, callback, domain=None):
        found = False
        
        def cb(mapping):
            found = callback(mapping)
            return not found

        self.forall(cb, domain)
        return found

    def approximate(self, domain=None):
        bounds = ffi.new('vorf_bound_t[%d]' % self.nb_inputs)
        
        for ind in range(self.nb_inputs):
            bounds[ind].lower = -float('inf')
            bounds[ind].upper = float('inf')

        domain = domain or list()
        for ind, bound in enumerate(domain):
            bounds[ind].lower = bound[0]
            bounds[ind].upper = bound[1]
            
        return lib.vorf_forest_approximate(self.ptr, bounds)

