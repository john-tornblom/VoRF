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
#include <string.h>
#include <vorf.h>


/**
 * Print the accuracy of a model for a set of samples to stdout.
 **/
int main(int argc, char** argv) {
  real_t *data;
  size_t nb_rows, nb_cols;
  vorf_forest_t* f;
  real_t score = 0;
  
  if(argc < 3) {
    printf("usage: %s <model file> <csv file>\n", argv[0]);
    return 1;
  }
  
  if(!(f = vorf_forest_load(argv[1]))) {
    printf("Unable to load model from %s\n", argv[1]);
    exit(1);
  }
  
  if(!vorf_load_csv(argv[2], &data, &nb_rows, &nb_cols)) {
    printf("Unable to load data from %s\n", argv[2]);
    exit(1);
  }
  
  if(nb_cols != f->nb_inputs + 1) {
    printf("Unexpected number of columns in %s\n", argv[2]);
    exit(1);
  }

  printf("accuracy:filename:   %s\n", argv[1]);
  printf("accuracy:nb_inputs:  %ld\n", f->nb_inputs);
  printf("accuracy:nb_outputs: %ld\n", f->nb_outputs);
  printf("accuracy:nb_trees:   %ld\n", f->nb_trees);
  printf("accuracy:nb_nodes:   %ld\n", f->nb_nodes);
  printf("accuracy:nb_samples: %ld\n", nb_rows);
  
  for(size_t row=0; row<nb_rows; row++) {
    real_t *sample = &data[row * nb_cols];
    real_t prob[f->nb_outputs];
    
    memset(prob, 0, sizeof(prob));
    vorf_forest_eval(f, &data[row * nb_cols], prob);
    
    size_t pred = vorf_argmax(prob, f->nb_outputs);
    size_t label = sample[f->nb_inputs];
    score += (pred == label);
  }
    
  vorf_forest_del(f);
  free(data);
  
  printf("accuracy:score:      %f\n", score/nb_rows);

  return 0;
}

