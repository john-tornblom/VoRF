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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "vorf.h"


size_t
vorf_argmax(const float* fvec, size_t length) {
  size_t m = 0;

  for(size_t i=1; i<length; i++) {
    if(fvec[i] > fvec[m]) {
      m = i;
    }
  }

  return m;
}


bool
vorf_load_csv(const char* filename, float **data, size_t *rows, size_t *cols) {
  char* line = NULL;
  size_t linelen;
  size_t nb_rows = 0;
  size_t nb_cols = 1;
  size_t count = 0;
  FILE *fp = fopen(filename, "r");
  
  *rows = 0;
  *cols = 0;
  *data = NULL;
  
  if(!fp) {
    return false;
  }

  // determine an upper bound of required memory
  while(!feof(fp)) {
    char ch = fgetc(fp);
    nb_rows += (ch == '\n');
    nb_cols += (ch == ',' && !nb_rows);
  }
  rewind(fp);

  *data = calloc(nb_rows * nb_cols, sizeof(float));
  assert(*data);
  
  while(getline(&line, &linelen, fp) > 0) {
    char* p = line;
    do {
      assert(count <= nb_rows * nb_cols);
      (*data)[count++] = atof(p);
      if((p = strstr(p, ","))) {
	p++;
      }
    } while(p);
    (*rows)++;
  }

  *cols = nb_cols;
  
  free(line);
  fclose(fp);

  return true;
}


const char*
vorf_version(void) {
  return VERSION;
}
