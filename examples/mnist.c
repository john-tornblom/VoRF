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
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <vorf.h>


#define IMG_HEIGHT 28
#define IMG_WIDTH  28


typedef union image {
  float vector[IMG_HEIGHT*IMG_WIDTH];
  float matrix[IMG_HEIGHT][IMG_WIDTH];
} image_t;


typedef union image_bound {
  vorf_bound_t vector[IMG_HEIGHT*IMG_WIDTH];
  vorf_bound_t matrix[IMG_HEIGHT][IMG_WIDTH];
} image_bound_t;


typedef struct noise_window {
  vorf_bound_t x;
  vorf_bound_t y;
  size_t threshold;
} noise_window_t;


typedef struct image_metadata {
  size_t id;
  int    label;
  image_t *image;
} image_metadata_t;


/**
 * Save a point from input of a mapping as a .pgm image.
 **/
static void
save_image(const image_metadata_t *md, vorf_mapping_t *m) {
  char filename[30];
  sprintf(filename, "%05ld.pgm", md->id);
  
  FILE *fp = fopen(filename, "wb");
  assert(fp);
			 
  fprintf(fp, "P2\n");
  fprintf(fp, "# Label: %d\n", md->label);
  for(size_t i=0; i<m->nb_outputs; i++) {
    fprintf(fp, "# Digit %ld: [%f, %f]\n", i,
	    m->outputs[i].lower, m->outputs[i].upper);
  }

  fprintf(fp, "# Original:");
  for(size_t i=0; i<IMG_HEIGHT*IMG_WIDTH; i++) {
    if(i%IMG_WIDTH == 0) {
      fprintf(fp, "\n# ");
    }
    fprintf(fp, "%03d ", (int)roundf(md->image->vector[i]));
  }

  fprintf(fp, "\n# Diff:");
  for(size_t i=0; i<IMG_HEIGHT*IMG_WIDTH; i++) {
    if(i%IMG_WIDTH == 0) {
      fprintf(fp, "\n# ");
    }
    int px1 = (int)roundf(m->inputs[i].lower);
    int px2 = (int)roundf(md->image->vector[i]);
    fprintf(fp, "%03d ", 255 - abs(px1 - px2));
  }
  
  fprintf(fp, "\n%d %d\n", IMG_WIDTH, IMG_HEIGHT);
  fprintf(fp, "%d", 255);
  
  for(size_t i=0; i<m->nb_inputs; i++) {
    if(i%IMG_WIDTH == 0) {
      fprintf(fp, "\n");
    }
    fprintf(fp, "%03d ", (int)roundf(m->inputs[i].lower));
  }
  fclose(fp);
}


static void
init_image_bound(image_bound_t *b, const image_t *img, const noise_window_t *w) {
  for(size_t y=0; y<IMG_HEIGHT; y++) {
    for(size_t x=0; x<IMG_WIDTH; x++) {
      b->matrix[y][x].lower = img->matrix[y][x];
      b->matrix[y][x].upper = img->matrix[y][x];

      if(w->x.lower <= x && x <= w->x.upper && w->y.lower <= y && y <= w->y.upper) {
	b->matrix[y][x].lower -= w->threshold;
	b->matrix[y][x].upper += w->threshold;
      }

      b->matrix[y][x].lower = fmaxf(b->matrix[y][x].lower, 0);
      b->matrix[y][x].upper = fminf(b->matrix[y][x].upper, 255);
    }
  }
}


static bool
is_correct(void *ctx, vorf_mapping_t *m) {
  image_metadata_t *md = ctx;
  if(md->label != vorf_mapping_argmax(m)) {
    if(0) save_image(md, m);
    return false;
  }
  return true;
}


/**
 * Parse command line arguments and launch program.
 **/
int main(int argc, char** argv) {
  float *data;
  size_t nb_rows, nb_cols;
  float score = 0;
  vorf_forest_t* f;
  size_t threshold;
  size_t window;
  
  if(argc < 5) {
    printf("usage: %s <model file> <csv file> <window> <threshold>\n", argv[0]);
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

  window = atoi(argv[3]);
  threshold = atoi(argv[4]);

  printf("mnist:filename:    %s\n", argv[1]);
  printf("mnist:nb_inputs:  %ld\n", f->nb_inputs);
  printf("mnist:nb_outputs: %ld\n", f->nb_outputs);
  printf("mnist:nb_trees:   %ld\n", f->nb_trees);
  printf("mnist:nb_nodes:   %ld\n", f->nb_nodes);
  printf("mnist:nb_samples: %ld\n", nb_rows);
  printf("mnist:window:     %ld\n", window);
  printf("mnist:threshold:  %ld\n", threshold);
  
  time_t t = time(NULL);
  for(size_t row=0; row<nb_rows; row++) {
    float *sample = &data[row * nb_cols];
    image_t* img = (image_t*)sample;
    int label = (int)sample[f->nb_inputs];
    bool res = true;

    fprintf(stderr, "mnist:progress:   %ld/%ld", row, nb_rows);
    fflush(stderr);
    for(size_t y=0; y<IMG_HEIGHT-window; y++) {
      for(size_t x=0; x<IMG_WIDTH-window; x++) {
	image_metadata_t md;
	image_bound_t bound;
	noise_window_t noise;

	noise.x.lower = x;
	noise.x.upper = x + window;
	noise.y.lower = y;
	noise.y.upper = y + window;
	noise.threshold = threshold;
	init_image_bound(&bound, img, &noise);

	md.id = row;
	md.label = label;
	md.image = img;
	if(!vorf_forest_forall(f, bound.vector, is_correct, &md)) {
	  res = false;
	  break;
	}
      }
    }
    fprintf(stderr, "\r");
    
    if(res) {
      score++;
    }
  }
  
  vorf_forest_del(f);
  free(data);

  printf("mnist:score:      %f\n", score/nb_rows);
  printf("mnist:runtime:    %lds\n", time(NULL) - t);
  
  return 0;
}
