#!/usr/bin/env python
# encoding: utf-8
# Copyright (C) 2018 John Törnblom
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING. If not see
# <http://www.gnu.org/licenses/>.
'''
Ensure that the global safey property is satisfied, i.e. that output
scalars are within a stated safety boundary.
'''

import sys
import vorf
  
def global_safety(mapping, alpha=0, beta=1):
    minval = min([mapping.outputs[dim].lower
                  for dim in range(mapping.nb_outputs)])
                   
    maxval = max([mapping.outputs[dim].upper
                  for dim in range(mapping.nb_outputs)])
                   
    return (minval >= alpha) and (maxval <= beta)
    
f = vorf.Forest(sys.argv[1]) # load model from disk
assert f.forall(global_safety)


