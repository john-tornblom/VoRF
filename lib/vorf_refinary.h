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


#ifndef VORF_REFINERY_H
#define VORF_REFINERY_H

#include <stddef.h>
#include <stdbool.h>

#include "vorf.h"
#include "vorf_tree.h"


/**
 * A refinery evaluates a tree, and emits mappings.
 **/
typedef struct vorf_refinery {
  const vorf_tree_t *tree;
  vorf_mapping_cb_t *emit_cb;
  void              *emit_ctx;
} vorf_refinery_t;


/**
 * Create a new refinery.
 **/
vorf_refinery_t* vorf_refinery_new(const vorf_tree_t *t, vorf_mapping_cb_t *cb,
				   void* ctx);


/**
 * Delete a refinery.
 **/
void vorf_refinery_del(vorf_refinery_t* r);


/**
 * Refine and emit mappings.
 **/
bool vorf_refinery_emit(vorf_refinery_t* r, vorf_mapping_t *m, size_t node_id);


#endif //VORF_REFINERY_H
