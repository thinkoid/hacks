# -*- mode: makefile -*-

include $(top_srcdir)/Makefile.common

bin_PROGRAMS = t

t_SOURCES = t.cpp

t_CPPFLAGS = $(AM_CPPFLAGS) $(BENCHMARK_CPPFLAGS)
t_LDFLAGS = $(AM_LDFLAGS)  $(BENCHMARK_LDFLAGS)
t_LDADD = $(BENCHMARK_LIBS)
