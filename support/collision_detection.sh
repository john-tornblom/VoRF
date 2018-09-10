#!/usr/bin/env bash
#   Copyright (C) 2018 John Törnblom
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING. If not see
# <http://www.gnu.org/licenses/>.

SCRIPTDIR="${BASH_SOURCE[0]}"
SCRIPTDIR="$(dirname "${SCRIPTDIR}")"

for d in {10,20}; do
    for t in {10,15,20,25}; do
	$SCRIPTDIR/train-sklearn.py \
	    $SCRIPTDIR/data/collision_detection.train.csv \
	    -v \
	    -t $t \
	    -d $d \
	    -o collision_detection.t${t}.d${d}.json || exit 1

	$SCRIPTDIR/../src/vorf_accuracy \
	    collision_detection.t${t}.d${d}.json \
	    $SCRIPTDIR/data/collision_detection.test.csv \
	    > collision_detection.t${t}.d${d}.log || exit 1

	$SCRIPTDIR/../src/vorf_global_safety \
	    collision_detection.t${t}.d${d}.json \
	    0 1 \
	    0 1 \
	    0 1 \
	    0 1 \
	    0 1 \
	    0 1 \
	    >> collision_detection.t${t}.d${d}.log || exit 1
	
	$SCRIPTDIR/../src/vorf_robustness \
	    collision_detection.t${t}.d${d}.json \
	    $SCRIPTDIR/data/collision_detection.test.csv \
	    0.05 \
	    >> collision_detection.t${t}.d${d}.log || exit 1

	#$SCRIPTDIR/../src/vorf_cardinality \
	#    collision_detection.t${t}.d${d}.json \
	#    >> collision_detection.t${t}.d${d}.log || exit 1
    done
done
