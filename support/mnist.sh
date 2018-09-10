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

GLOBAL_SAFETY_ARG=""
for i in {1..784}; do
    GLOBAL_SAFETY_ARG="0 1 $GLOBAL_SAFETY_ARG"
done

for d in {10,20}; do
    for t in {10,15,20,25}; do
	$SCRIPTDIR/train-sklearn.py \
	    $SCRIPTDIR/data/mnist.train.csv \
	    -v \
	    -t $t \
	    -d $d \
	    -o mnist.t${t}.d${d}.json || exit 1

	$SCRIPTDIR/../src/vorf_accuracy \
	    mnist.t${t}.d${d}.json \
	    $SCRIPTDIR/data/mnist.test.csv \
	    > mnist.t${t}.d${d}.log || exit 1

	$SCRIPTDIR/../src/vorf_global_safety \
	    mnist.t${t}.d${d}.json \
	    $GLOBAL_SAFETY_ARG \
	    >> mnist.t${t}.d${d}.log || exit 1
	
	$SCRIPTDIR/../examples/vorf_mnist_window_robustness \
	    mnist.t${t}.d${d}.json \
	    $SCRIPTDIR/data/mnist.test.csv \
	    5 \
	    1 \
	    >> mnist.t${t}.d${d}.log || exit 1

	#$SCRIPTDIR/../src/vorf_cardinality \
	#    mnist.t${t}.d${d}.json \
	#    >> mnist.t${t}.d${d}.log || exit 1
    done
done
