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


#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <json.h>

#include "vorf.h"
#include "vorf_tree.h"
#include "vorf_refinary.h"


vorf_forest_t*
vorf_forest_load(const char *filename) {
  json_object* root = json_object_from_file(filename);
  assert(root);
  
  array_list* array = json_object_get_array(root);
  assert(array);
  
  vorf_forest_t* f = calloc(1, sizeof(vorf_forest_t));
  assert(f);
  
  f->nb_trees = array_list_length(array);
  f->trees = calloc(f->nb_trees, sizeof(vorf_tree_t*));
  assert(f->trees);
  
  for(size_t i=0; i<f->nb_trees; i++) {
    json_object *el = array_list_get_idx(array, i);
    f->trees[i] = vorf_tree_parse(el);

    if(i == 0) {
      f->nb_inputs = f->trees[i]->nb_inputs;
      f->nb_outputs = f->trees[i]->nb_outputs;
    } else {
      assert(f->nb_inputs == f->trees[i]->nb_inputs);
      assert(f->nb_outputs == f->trees[i]->nb_outputs);
    }
    f->nb_nodes += f->trees[i]->nb_nodes;
  }

  //This actually deletes the json_object. Possibly the worst
  //name choices in the history of computing.
  json_object_put(root);
  
  return f;
}



void
vorf_forest_del(vorf_forest_t* f) {
  for(size_t i=0; i<f->nb_trees; i++) {
    vorf_tree_del(f->trees[i]);
  }

  free(f->trees);
  free(f);
}


/**
 * Keep track of data used in the final pipeline.
 **/
typedef struct vorf_forest_callout {
  vorf_mapping_cb_t *user_cb;
  void              *user_ctx;
  size_t             divisor;
} vorf_forest_callout_t;


/**
 * Final pipeline which divide outputs and calls out to the user.
 **/
static bool
pipe_forall_callout(void* ctx, vorf_mapping_t *m) {
  vorf_forest_callout_t *co = ctx;

  for(size_t i=0; i<m->nb_outputs; i++) {
    m->outputs[i].lower /= co->divisor;
    m->outputs[i].upper /= co->divisor;
  }

  return co->user_cb(co->user_ctx, m);
}


/**
 * Intermediate pipeline between trees in the forest.
 **/
static bool
pipe_forall_emit(void* ctx, vorf_mapping_t *m) {
  vorf_refinery_t* r = (vorf_refinery_t*)ctx;
  return vorf_refinery_emit(r, m, 0);
}


bool
vorf_forest_forall(const vorf_forest_t *f, const vorf_bound_t* input_region,
		   vorf_mapping_cb_t *user_cb, void* user_ctx) {
  vorf_forest_callout_t co;
  vorf_refinery_t **r;
  vorf_mapping_cb_t *cb;
  vorf_mapping_t* m;
  bool res;
  void *ctx;
  
  co.user_cb = user_cb;
  co.user_ctx = user_ctx;
  co.divisor = f->nb_trees;
  
  cb = pipe_forall_callout;
  ctx = &co;

  r = calloc(f->nb_trees, sizeof(vorf_refinery_t*));
  assert(r);

  for(size_t i=0; i<f->nb_trees; i++) {
    ctx = r[i] = vorf_refinery_new(f->trees[i], cb, ctx);
    cb = pipe_forall_emit;
  }

  m = vorf_mapping_new(f->nb_inputs, f->nb_outputs);
  memcpy(m->inputs, input_region, f->nb_inputs * sizeof(vorf_bound_t));
  res = cb(ctx, m);
  
  vorf_mapping_del(m);
  for(size_t i=0; i<f->nb_trees; i++) {
    vorf_refinery_del(r[i]);
  }
  free(r);
  
  return res;
}


/**
 * Fetch the output from an mapping when a concrete, fully-refined instance is
 * encountered.
 **/
static bool
pipe_eval_compute(void *ctx, vorf_mapping_t* m) {
  real_t *outputs = ctx;

  for(size_t i=0; i<m->nb_outputs; i++) {
    assert(m->outputs[i].lower == m->outputs[i].upper);
    outputs[i] = m->outputs[i].lower;
  }

  return false;
}


void
vorf_forest_eval(const vorf_forest_t* f, const real_t *inputs, real_t *outputs) {
  vorf_bound_t bounds[f->nb_inputs];

  for(size_t i=0; i<f->nb_inputs; i++) {
    bounds[i].lower = inputs[i];
    bounds[i].upper = inputs[i];
  }

  vorf_forest_forall(f, bounds, pipe_eval_compute, outputs);
}


static bool
pipe_bound_join(void *ctx, vorf_mapping_t* m) {
  vorf_mapping_t** join = (vorf_mapping_t**)ctx;
  if(!*join) {
    *join = vorf_mapping_copy(m);
  } else {
    vorf_mapping_join(m, *join);
  }
  
  return true;
}


vorf_mapping_t *
vorf_forest_approximate(const vorf_forest_t *f, const vorf_bound_t* input_region) {
  vorf_mapping_t* join = NULL;
  for(size_t i=0; i<f->nb_trees; i++) {
    vorf_refinery_t *r = vorf_refinery_new(f->trees[i], pipe_bound_join, &join);
    vorf_mapping_t *m = vorf_mapping_new(f->nb_inputs, f->nb_outputs);
    memcpy(m->inputs, input_region, f->nb_inputs * sizeof(vorf_bound_t));
    vorf_refinery_emit(r, m, 0);
    vorf_refinery_del(r);
    vorf_mapping_del(m);
  }

  return join;
}
