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
#include <math.h>
#include <vorf.h>


/**
 * Print a mapping to stdout.
 */
static bool
dump_mapping(void *ctx, vorf_mapping_t *m) {
  VORF_UNUSED(ctx);

  for(size_t i=0; i<m->nb_inputs; i++) {
    if(i) {
      printf(", ");
    }
    printf("x%ld in [%f, %f]", i, m->inputs[i].lower, m->inputs[i].upper);
  }
  printf(" --> ");
  
  for(size_t i=0; i<m->nb_outputs; i++) {
    if(i) {
      printf(", ");
    }
    printf("y%ld in [%f, %f]", i, m->outputs[i].lower, m->outputs[i].upper);
  }
  printf("\n");

  return true;
}
 

/**
 * Print all mappings of a forest to stdout.
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

  vorf_forest_forall(f, domain, dump_mapping, NULL);
  vorf_forest_del(f);
  
  return 0;
}

