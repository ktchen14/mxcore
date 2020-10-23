ACLOCAL_AMFLAGS = -I tool

SUBDIRS = header

lib_LTLIBRARIES = libvector.la
libvector_la_SOURCES = source/vector/access.c \
		       source/vector/common.c \
		       source/vector/comparison.c \
		       source/vector/create.c \
		       source/vector/debug.c \
		       source/vector/delete.c \
		       source/vector/insert.c \
		       source/vector/move.c \
		       source/vector/remove.c \
		       source/vector/resize.c \
		       source/vector/search.c \
		       source/vector/shift.c \
		       source/vector/sort.c \
		       source/vector.c
libvector_la_CFLAGS = -I$(top_srcdir)/header -Wall

pkgconfig_DATA = data/vector.pc
