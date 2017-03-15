# -*- mode: makefile -*-

include $(top_srcdir)/Makefile.common

noinst_LIBRARIES = libsuffix-tree.a
libsuffix_tree_a_SOURCES = suffix-tree.cpp

bin_PROGRAMS = t

t_SOURCES = t.cpp

t_CPPFLAGS = $(AM_CPPFLAGS) $(BENCHMARK_CPPFLAGS)
t_LDFLAGS  = $(AM_LDFLAGS)  $(BENCHMARK_LDFLAGS)

t_LDADD = $(top_srcdir)/src/suffix-tree/libsuffix-tree.a \
          $(BENCHMARK_LIBS)
