/* Copyright (C) 2018 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */


#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <math.h>
#include <vorf.h>


/**
 * Print the input/output space of a forest to stdout.
 **/
int main(int argc, char** argv) {
  if(argc < 2) {
    printf("usage: %s <model file>\n", argv[0]);
    return 1;
  }

  vorf_forest_t* f = vorf_forest_load(argv[1]);
  vorf_bound_t domain[f->nb_inputs];

  for(size_t i=0; i<f->nb_inputs; i++) {
    domain[i].lower = -INFINITY;
    domain[i].upper = INFINITY;
  }

  vorf_mapping_t *m = vorf_forest_approximate(f, domain);
  assert(m);
  
  for(size_t i=0; i<m->nb_inputs; i++) {
    printf("x%ld in [%f, %f]\n", i, m->inputs[i].lower, m->inputs[i].upper);
  }
  
  for(size_t i=0; i<m->nb_outputs; i++) {
    printf("y%ld in [%f, %f]\n", i, m->outputs[i].lower, m->outputs[i].upper);
  }
  
  vorf_forest_del(f);
  vorf_mapping_del(m);

  return 0;
}

