# -*- mode: makefile -*-

include $(top_srcdir)/Makefile.common

noinst_LIBRARIES = libsuffix-array.a
libsuffix_array_a_SOURCES = suffix-array.cpp

bin_PROGRAMS = t

t_SOURCES = t.cpp
t_LDADD = libsuffix-array.a
