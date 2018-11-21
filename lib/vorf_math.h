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


#ifndef VORF_MATH_H
#define VORF_MATH_H

#include <math.h>

#include "vorf.h"


#if USE_DOUBLE
#define vorf_nextafter(x, y) nextafter(x, y)
#define vorf_max(x, y)       fmax(x, y)
#define vorf_min(x, y)       fmin(x, y)
#else
#define vorf_nextafter(x, y) nextafterf(x, y)
#define vorf_max(x, y)       fmaxf(x, y)
#define vorf_min(x, y)       fminf(x, y)
#endif


#endif //VORF_MATH_H
