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
 * Count number of mappings.
 */
static bool
count_mapping(void *ctx, vorf_mapping_t *m) {
  VORF_UNUSED(m);
  size_t *nb_mappings = (size_t*)ctx;
  (*nb_mappings)++;
  return true;
}


/**
 * Print the number of mappings of a forest to stdout.
 **/
int main(int argc, char** argv) {
  if(argc < 2) {
    printf("usage: %s <model file>\n", argv[0]);
    return 1;
  }

  size_t nb_mappings = 0;
  vorf_forest_t* f = vorf_forest_load(argv[1]);
  assert(f);

  printf("cardinality:filename:    %s\n", argv[1]);
  printf("cardinality:nb_inputs:   %ld\n", f->nb_inputs);
  printf("cardinality:nb_outputs:  %ld\n", f->nb_outputs);
  printf("cardinality:nb_trees:    %ld\n", f->nb_trees);
  printf("cardinality:nb_nodes:    %ld\n", f->nb_nodes);
  
  vorf_bound_t domain[f->nb_inputs];
  for(size_t i=0; i<f->nb_inputs; i++) {
    domain[i].lower = -INFINITY;
    domain[i].upper = INFINITY;
  }
  
  vorf_forest_forall(f, domain, count_mapping, &nb_mappings);
  vorf_forest_del(f);

  printf("cardinality:nb_mappings: %ld\n", nb_mappings);

  return 0;
}

