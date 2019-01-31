# -*- mode: makefile -*-

SUBDIRS =                                       \
    suffix-tree                                 \
    suffix-array                                \
    aho-corasick                                \
    cluster                                     \
    cyclic-rotation                             \
    cyclic-substring                            \
    knuth-morris-pratt                          \
    lexicographical-merge                       \
    longest-prefix                              \
    longest-prefix-suffix                       \
    lps                                         \
    lrf                                         \
    make-anagram                                \
    manacher                                    \
	perf                                        \
    reduced-string                              \
    tandem-arrays                               \
    two-characters

default: all

$(SUBDIRS)::
	$(MAKE) -C $@ $(MAKECMDGOALS)

all clean : $(SUBDIRS)
