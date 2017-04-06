# -*- mode: makefile -*-

include $(top_srcdir)/Makefile.common

AM_CPPFLAGS += $(OPENCV_CPPFLAGS)

AM_LDFLAGS +=									\
    $(OPENCV_HIGHGUI_LDFLAGS)					\
    $(OPENCV_IMGPROC_LDFLAGS)					\
    $(OPENCV_IMGCODECS_LDFLAGS)					\
    $(OPENCV_CORE_LDFLAGS)

LIBS += $(OPENCV_LIBS)

bin_PROGRAMS = blob subgraph
blob_SOURCES = blob.cpp
subgraph_SOURCES = subgraph.cpp
