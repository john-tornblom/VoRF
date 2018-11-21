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

lib_LTLIBRARIES = libvorf.la

libvorf_la_CFLAGS = -I../inc -std=c99 -D_DEFAULT_SOURCE -Wextra -Werror \
                    -pedantic -Wdouble-promotion -Wfloat-conversion \
                    @LIBJSON_CFLAGS@
libvorf_la_SOURCES = vorf_forest.c \
                     vorf_mapping.c \
                     vorf_refinary.c \
                     vorf_tree.c \
                     vorf_utils.c

libvorf_la_LIBADD = @LIBJSON_LIBS@ -lm
libvorf_la_LDFLAGS = -version-info 0:0:0

