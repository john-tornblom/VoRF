/* Copyright (C) 2018 John Törnblom

   This file is part of VoRF (Verifier of Random Forests).

VoRF is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

VoRF is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
for more details.

You should have received a copy of the GNU Lesser General Public
License along with VoRF; see the files COPYING and COPYING.LESSER. If not,
see <http://www.gnu.org/licenses/>.  */


#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "vorf.h"
#include "vorf_refinary.h"
#include "vorf_tree.h"


vorf_refinery_t*
vorf_refinery_new(const vorf_tree_t *t, vorf_mapping_cb_t *cb, void* ctx) {
  vorf_refinery_t* r = calloc(1, sizeof(vorf_refinery_t));
  assert(r);

  r->tree     = t;
  r->emit_cb  = cb;
  r->emit_ctx = ctx;
  
  return r;
}


void
vorf_refinery_del(vorf_refinery_t* r) {
  free(r);
}


bool
vorf_refinery_emit(vorf_refinery_t* r, vorf_mapping_t *m, size_t node_id) {
  int left_id = r->tree->left[node_id];
  int right_id = r->tree->right[node_id];

  if(left_id < 0 || right_id < 0) {
    for(size_t i=0; i<m->nb_outputs; i++) {
      m->outputs[i].upper += r->tree->value[node_id][i];
      m->outputs[i].lower += r->tree->value[node_id][i];
    }
    
    return r->emit_cb(r->emit_ctx, m);
  }

  float threshold = r->tree->threshold[node_id];
  int dim = r->tree->feature[node_id];

  //        left       right
  //   |-----------|-----------|
  // lower     threshold     upper
  //
    
  // left: [lower, threshold]
  if(m->inputs[dim].lower <= threshold) {
    vorf_bound_t inputs[m->nb_inputs];
    vorf_bound_t outputs[m->nb_outputs];
    vorf_mapping_t mleft;
  
    memcpy(inputs, m->inputs, m->nb_inputs * sizeof(vorf_bound_t));
    memcpy(outputs, m->outputs, m->nb_outputs * sizeof(vorf_bound_t));
  
    mleft.inputs = inputs;
    mleft.outputs = outputs;
    mleft.nb_inputs = m->nb_inputs;
    mleft.nb_outputs = m->nb_outputs;

    if(mleft.inputs[dim].upper > threshold) {
      mleft.inputs[dim].upper = threshold;
    }
    if(!vorf_refinery_emit(r, &mleft, left_id)) {
      return false;
    }
  }

  // right: (threshold, upper]
  if(m->inputs[dim].upper > threshold) {
    if(m->inputs[dim].lower < threshold) {
      m->inputs[dim].lower = nextafterf(threshold, INFINITY);
    }
    if(!vorf_refinery_emit(r, m, right_id)) {
      return false;
    }
  }

  return true;
}

