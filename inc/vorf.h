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


#ifndef VORF_H
#define VORF_H

#include <stddef.h>
#include <stdbool.h>


#define VORF_UNUSED(x) (void)(x)
#define USE_DOUBLE 0
typedef float real_t;


/**
 * The bound of a variable, i.e. its range.
 **/
typedef struct vorf_bound {
  real_t lower;
  real_t upper;
} vorf_bound_t;


/**
 * A mapping from an input region to an output range.
 **/
typedef struct vorf_mapping {
  vorf_bound_t* inputs;
  vorf_bound_t* outputs;
  size_t        nb_inputs;
  size_t        nb_outputs;
} vorf_mapping_t;


/**
 * Forward declaration of a tree, only used internally by vorf.
 **/
struct vorf_tree;
typedef struct vorf_tree vorf_tree_t;


/**
 * A forest is a collection of trees.
 **/
typedef struct vorf_forest {
  vorf_tree_t **trees;
  size_t        nb_trees;
  size_t        nb_inputs;
  size_t        nb_outputs;
  size_t        nb_nodes;
} vorf_forest_t;


/**
 * Callback function prototype used to iterate input/output mappings.
 * returning false cancels iterations.
 **/
typedef bool (vorf_mapping_cb_t)(void *ctx, vorf_mapping_t *mapping);


/**
 * Obtain the version number of VoRF.
 **/
const char* vorf_version(void);


/**
 * Compute the argmax of a vector with reals.
 **/
size_t vorf_argmax(const real_t* fvec, size_t length);


/**
 * Load a CSV file into memory. 
 *
 * A correctly formatted CSV file (using the comma delimiter) without
 * a header is assumed.
 **/
bool vorf_load_csv(const char* filename, real_t **data,
		   size_t *nb_rows, size_t *nb_cols);


/**
 * Create a new mapping with the given input/output dimensions. Input bounds
 * are initialized to [-∞, ∞], and output bounds are initialized to [0, 0].
 **/
vorf_mapping_t* vorf_mapping_new(size_t input_dim, size_t output_dim);


/**
 * Create a deep copy of a mapping.
 **/
vorf_mapping_t* vorf_mapping_copy(const vorf_mapping_t* m);


/**
 * Join two mappings.
 **/
void vorf_mapping_join(const vorf_mapping_t *m, vorf_mapping_t *j);


/**
 * Compute the argmax of a mapping.
 *
 * Note: Negative one (-1) indicate an inconclusive mapping.
 **/
int vorf_mapping_argmax(const vorf_mapping_t* m);


/**
 * Delete a mapping, including its bounds.
 **/
void vorf_mapping_del(vorf_mapping_t* m);


/**
 * Load a forest from disk persisted in a JSON-based format.
 **/
vorf_forest_t *vorf_forest_load(const char *filename);


/**
 * Delete a forest and all of its trees.
 **/
void vorf_forest_del(vorf_forest_t* f);


/**
 * Execute the forest on concrete values.
 **/
void vorf_forest_eval(const vorf_forest_t* f, const real_t *inputs, real_t *outputs);


/**
 * Iterate all feasible mappings of a forest for some input region
 * until the callback function returns false, or all mappings
 * have been iterated.
 *
 * Returns true if all mappings were accepted by the callback function, or false 
 * if the iteration was canceled.
 **/
bool vorf_forest_forall(const vorf_forest_t *f, const vorf_bound_t* input_region,
			vorf_mapping_cb_t *cb, void* ctx);


/**
 * Approximate a pessimistic and sound mapping for a given input region
 **/
vorf_mapping_t *vorf_forest_approximate(const vorf_forest_t *f,
					const vorf_bound_t* input_region);


#endif //VORF_H
