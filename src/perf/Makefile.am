# -*- mode: makefile -*-

include $(top_srcdir)/Makefile.common

bin_PROGRAMS = find

find_SOURCES = find.cpp

find_CPPFLAGS = $(AM_CPPFLAGS) $(BENCHMARK_CPPFLAGS)
find_LDFLAGS  = $(AM_LDFLAGS)  $(BENCHMARK_LDFLAGS)

find_LDADD = $(BENCHMARK_LIBS)
