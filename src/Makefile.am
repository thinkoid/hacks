# -*- mode: makefile -*-

SUBDIRS =                                       \
    cyclic-rotation                             \
    cyclic-substring                            \
    knuth-morris-pratt                          \
    lexicographical-merge                       \
    longest-prefix                              \
    longest-prefix-suffix                       \
    make-anagram                                \
    palindrome-index                            \
    palindromes                                 \
    reduced-string                              \
    string-function                             \
    suffix-tree                                 \
    tandem-arrays                               \
    two-characters

default: all

$(SUBDIRS)::
	$(MAKE) -C $@ $(MAKECMDGOALS)

all clean : $(SUBDIRS)
