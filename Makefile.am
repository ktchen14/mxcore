VECTOR_HEADER_LIST = header/vector/access.c \
		     header/vector/access.h \
		     header/vector/common.c \
		     header/vector/common.h \
		     header/vector/common.c \
		     header/vector/comparison.c \
		     header/vector/comparison.h \
		     header/vector/create.c \
		     header/vector/create.h \
		     header/vector/debug.c \
		     header/vector/debug.h \
		     header/vector/delete.c \
		     header/vector/delete.h \
		     header/vector/insert.c \
		     header/vector/insert.h \
		     header/vector/move.c \
		     header/vector/move.h \
		     header/vector/remove.c \
		     header/vector/remove.h \
		     header/vector/resize.c \
		     header/vector/resize.h \
		     header/vector/search.c \
		     header/vector/search.h \
		     header/vector/shift.c \
		     header/vector/shift.h \
		     header/vector/sort.c \
		     header/vector/sort.h \
		     header/vector.h
VECTOR_SOURCE_LIST = source/vector/access.c \
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

include_HEADERS = $(VECTOR_HEADER_LIST)
lib_LTLIBRARIES = libvector.la
libvector_la_SOURCES = $(VECTOR_SOURCE_LIST)
libvector_la_CFLAGS = -I$(top_srcdir)/header
