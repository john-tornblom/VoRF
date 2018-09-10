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


#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <vorf.h>


/**
 * Check if there are points in the ouput that fall
 * outside a given range.
 */
static bool
is_globally_safe(void *ctx, vorf_mapping_t *m) {
  vorf_bound_t *range = (vorf_bound_t *)ctx;
  
  for(size_t i=0; i<m->nb_outputs; i++) {
    if(m->outputs[i].lower < range[i].lower) {
      return false;
    }

    if(m->outputs[i].upper > range[i].upper) {
      return false;
    }
  }
  
  return true;
}
 

/**
 * Check the global safey property.
 **/
int main(int argc, char** argv) {
  vorf_forest_t* f;
  vorf_mapping_t* m;
  bool b;
  
  if(argc < 2) {
    printf("usage: %s <model file> <min y1> <max y1> <min y2> <max y2>...\n",
	   argv[0]);
    return 1;
  }

  if(!(f = vorf_forest_load(argv[1]))) {
    printf("Unable to load model from %s\n", argv[1]);
    exit(1);
  }

  if(argc < (f->nb_outputs * 2) + 2) {
    printf("Expected %ld min/max arguments, got %d\n", f->nb_outputs * 2, argc - 2);
    exit(1);
  }

  printf("global_safety:filename:   %s\n", argv[1]);
  printf("global_safety:nb_inputs:  %ld\n", f->nb_inputs);
  printf("global_safety:nb_outputs: %ld\n", f->nb_outputs);
  printf("global_safety:nb_trees:   %ld\n", f->nb_trees);
  printf("global_safety:nb_nodes:   %ld\n", f->nb_nodes);

  time_t t = time(NULL);
  vorf_bound_t domain[f->nb_inputs];
  for(size_t i=0; i<f->nb_inputs; i++) {
    domain[i].lower = -INFINITY;
    domain[i].upper = INFINITY;
  }

  vorf_bound_t range[f->nb_outputs];
  for(size_t i=0; i<f->nb_outputs; i++) {
    range[i].lower = atof(argv[2+(i*2)]);
    range[i].upper = atof(argv[3+(i*2)]);
  }

  m = vorf_forest_approximate(f, domain);
  assert(m);
  
  b = (is_globally_safe(range, m) ||
       vorf_forest_forall(f, domain, is_globally_safe, range));
       
  vorf_mapping_del(m);
  vorf_forest_del(f);

  printf("global_safety:result:     %s\n", b ? "pass" : "fail");
  printf("global_safety:runtime:    %lds\n", time(NULL) - t);
  
  return !b;
}

