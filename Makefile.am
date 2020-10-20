include_HEADERS = header/vector/access_impl.h \
		  header/vector/access.h \
		  header/vector/common_impl.h \
		  header/vector/common.h \
		  header/vector/common.o \
		  header/vector/comparison_impl.h \
		  header/vector/comparison.h \
		  header/vector/create_impl.h \
		  header/vector/create.h \
		  header/vector/debug_impl.h \
		  header/vector/debug.h \
		  header/vector/delete_impl.h \
		  header/vector/delete.h \
		  header/vector/insert_impl.h \
		  header/vector/insert.h \
		  header/vector/move_impl.h \
		  header/vector/move.h \
		  header/vector/remove_impl.h \
		  header/vector/remove.h \
		  header/vector/resize_impl.h \
		  header/vector/resize.h \
		  header/vector/search_impl.h \
		  header/vector/search.h \
		  header/vector/shift_impl.h \
		  header/vector/shift.h \
		  header/vector/sort_impl.h \
		  header/vector/sort.h \
		  header/vector.h

lib_LTLIBRARIES = libvector.la
libvector_la_CFLAGS = -I$(top_srcdir)/header
libvector_la_SOURCES = $(include_HEADERS) \
		       source/vector/access.c \
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

STUB_CFLAGS = -Dmalloc=stub_malloc \
	      -Drealloc=stub_realloc \
	      -Dfree=stub_free
lib_LTLIBRARIES += libstub.la
libstub_la_SOURCES = test/stub.c

lib_LTLIBRARIES += libvector_test.la
libvector_test_la_CFLAGS = $(libvector_la_CFLAGS) \
			   -DVECTOR_TEST \
			   $(STUB_CFLAGS)
libvector_test_la_LDFLAGS = -Wl,-flat_namespace
libvector_test_la_SOURCES = $(libvector_la_SOURCES)
libvector_test_la_LIBADD = libstub.la

check_PROGRAMS = test_vector_create
TESTS = test_vector_create
test_vector_create_CFLAGS = -D_GNU_SOURCE \
			    -DVECTOR_TEST \
			    -Wall \
			    -I$(top_srcdir)/header
test_vector_create_SOURCES = test/vector_create.c \
			     test/test.h
test_vector_create_LDADD = libvector_test.la -ldl
