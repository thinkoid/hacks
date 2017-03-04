# -*- mode: makefile -*-

include $(top_srcdir)/Makefile.common

bin_PROGRAMS = t

t_SOURCES = t.cpp

AM_CPPFLAGS += $(BENCHMARK_CPPFLAGS)
AM_LDFLAGS  += $(BENCHMARK_LDFLAGS)

LIBS += $(BENCHMARK_LIBS)
