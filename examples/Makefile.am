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
# along with this program; see the file COPYING. If not, see
# <http://www.gnu.org/licenses/>.

noinst_PROGRAMS = vorf_mnist_window_robustness

vorf_mnist_window_robustness_SOURCES = mnist.c
vorf_mnist_window_robustness_CFLAGS = -std=c99 -I../inc @LIBJSON_CFLAGS@
vorf_mnist_window_robustness_LDADD = ../lib/libvorf.la @LIBJSON_LIBS@ -lm
