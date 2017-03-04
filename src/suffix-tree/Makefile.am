# -*- mode: makefile -*-

include $(top_srcdir)/Makefile.common

bin_PROGRAMS = t other

t_SOURCES = t.cpp
other_SOURCES = other.cpp

AM_CPPFLAGS += $(BENCHMARK_CPPFLAGS)
AM_LDFLAGS  += $(BENCHMARK_LDFLAGS)

LIBS += $(BENCHMARK_LIBS)
