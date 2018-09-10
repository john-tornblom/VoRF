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

bin_PROGRAMS = vorf_accuracy \
               vorf_cardinality \
               vorf_mappings \
               vorf_throughput \
               vorf_iospace \
               vorf_robustness \
               vorf_global_safety

vorf_accuracy_SOURCES = accuracy.c
vorf_accuracy_CFLAGS = -std=c99 -I../inc @LIBJSON_CFLAGS@
vorf_accuracy_LDADD = ../lib/libvorf.la @LIBJSON_LIBS@ -lm

vorf_cardinality_SOURCES = cardinality.c
vorf_cardinality_CFLAGS = -std=c99 -I../inc @LIBJSON_CFLAGS@
vorf_cardinality_LDADD = ../lib/libvorf.la @LIBJSON_LIBS@ -lm

vorf_mappings_SOURCES = mappings.c
vorf_mappings_CFLAGS = -std=c99 -I../inc @LIBJSON_CFLAGS@
vorf_mappings_LDADD = ../lib/libvorf.la @LIBJSON_LIBS@ -lm

vorf_throughput_SOURCES = throughput.c
vorf_throughput_CFLAGS = -std=c99 -I../inc @LIBJSON_CFLAGS@
vorf_throughput_LDADD = ../lib/libvorf.la @LIBJSON_LIBS@ -lm

vorf_iospace_SOURCES = iospace.c
vorf_iospace_CFLAGS = -std=c99 -I../inc @LIBJSON_CFLAGS@
vorf_iospace_LDADD = ../lib/libvorf.la @LIBJSON_LIBS@ -lm

vorf_robustness_SOURCES = robustness.c
vorf_robustness_CFLAGS = -std=c99 -I../inc @LIBJSON_CFLAGS@
vorf_robustness_LDADD = ../lib/libvorf.la @LIBJSON_LIBS@ -lm

vorf_global_safety_SOURCES = global_safety.c
vorf_global_safety_CFLAGS = -std=c99 -I../inc @LIBJSON_CFLAGS@
vorf_global_safety_LDADD = ../lib/libvorf.la @LIBJSON_LIBS@ -lm
