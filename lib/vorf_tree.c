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
#include <json.h>

#include "vorf.h"
#include "vorf_tree.h"


/**
 * Parse a JSON float array.
 **/
static void
vorf_parse_floats(json_object *obj, real_t** mem, size_t* length) {
  array_list* array = json_object_get_array(obj);
  size_t _length = array_list_length(array);
  real_t* _mem = calloc(_length, sizeof(real_t));
  
  assert(_mem);
  
  for(size_t i=0; i<_length; i++) {
    json_object *el = array_list_get_idx(array, i);
    _mem[i] = (real_t)json_object_get_double(el);
  }

  *length = _length;
  *mem = _mem;
}


/**
 * Parse a JSON integer array.
 **/
static void
vorf_parse_ints(json_object *obj, int** mem, size_t* length) {
  array_list* array = json_object_get_array(obj);
  size_t _length = array_list_length(array);
  int* _mem = calloc(_length, sizeof(int));
  
  assert(_mem);
  
  for(size_t i=0; i<_length; i++) {
    json_object *el = array_list_get_idx(array, i);
    _mem[i] = json_object_get_int(el);
  }

  *length = _length;
  *mem = _mem;
}


/**
 * Parse a JSON dictionary into a tree.
 **/
vorf_tree_t *
vorf_tree_parse(json_object *root) {
  json_object *obj;
  size_t length;
  
  vorf_tree_t* tree = calloc(1, sizeof(vorf_tree_t));
  assert(tree);
  
  json_object_object_get_ex(root, "nb_inputs", &obj);
  tree->nb_inputs = json_object_get_int(obj);

  json_object_object_get_ex(root, "nb_outputs", &obj);
  tree->nb_outputs = json_object_get_int(obj);

  json_object_object_get_ex(root, "left", &obj);
  vorf_parse_ints(obj, &tree->left, &length);
  tree->nb_nodes = length;
    
  json_object_object_get_ex(root, "right", &obj);
  vorf_parse_ints(obj, &tree->right, &length);
  assert(length == tree->nb_nodes);
 
  json_object_object_get_ex(root, "feature", &obj);
  vorf_parse_ints(obj, &tree->feature, &length);
  assert(length == tree->nb_nodes);
    
  json_object_object_get_ex(root, "threshold", &obj);
  vorf_parse_floats(obj, &tree->threshold, &length);
  assert(length == tree->nb_nodes);
    
  json_object_object_get_ex(root, "value", &obj);
  array_list* v_array = json_object_get_array(obj);
  size_t v_length = array_list_length(v_array);
  assert(v_length == tree->nb_nodes);
  
  tree->value = calloc(v_length, sizeof(real_t*));
  assert(tree->value);
  
  for(size_t i=0; i<v_length; i++) {
    json_object *v_el = array_list_get_idx(v_array, i);

    array_list* o_array = json_object_get_array(v_el);
    size_t o_length = array_list_length(o_array);
    assert(o_length == 1); //TODO: support multioutput
    
    for(size_t j=0; j<o_length; j++) {
      json_object *o_el = array_list_get_idx(o_array, j);
      vorf_parse_floats(o_el, &tree->value[i], &length);
      assert(length == tree->nb_outputs);
    }
    
  }

  //python and sklearn use negative indices sometimes,
  //recompute them to be c-compatible.
  for(size_t i=0; i<tree->nb_nodes; i++) {
    if(tree->feature[i] < 0) {
      tree->feature[i] += tree->nb_inputs;
    }
  }
  
  return tree;
}


void
vorf_tree_del(vorf_tree_t* t) {
  for(size_t i=0; i<t->nb_nodes; i++) {
    free(t->value[i]);
  }
  
  free(t->left);
  free(t->right);
  free(t->feature);
  free(t->threshold);
  free(t->value);
  free(t);
}

