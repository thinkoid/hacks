# -*- mode: makefile -*-

include $(top_srcdir)/Makefile.common

bin_PROGRAMS = t count-leaves count-factors count-prefixes

t_SOURCES = t.cpp
t_LDADD = $(top_srcdir)/src/suffix-tree/libsuffix-tree.a

count_leaves_SOURCES = count-leaves.cpp
count_leaves_LDADD = $(top_srcdir)/src/suffix-tree/libsuffix-tree.a

count_factors_SOURCES = count-factors.cpp
count_factors_LDADD = $(top_srcdir)/src/suffix-tree/libsuffix-tree.a

count_prefixes_SOURCES = count-prefixes.cpp
count_prefixes_LDADD = $(top_srcdir)/src/suffix-tree/libsuffix-tree.a

