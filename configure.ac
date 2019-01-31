# -*- mode: autoconf -*-

AC_INIT([hacks],[0.0.1],)

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([m4])

AM_INIT_AUTOMAKE([foreign -Wno-portability subdir-objects dist-bzip2 nostdinc])
AC_CONFIG_HEADERS([include/hacks/_config.hpp:include/hacks/config.ac])

AC_PROG_CXX
AC_CONFIG_CXX_WARNINGS

AC_ENABLE_CXX_DIALECT([c++1z])

AC_PROG_LIBTOOL
AC_PROG_MAKE_SET(gmake)
AM_PROG_AS

AC_CONFIG_WITH_RANGE3
AC_CONFIG_WITH_BENCHMARK

BOOST_REQUIRE
BOOST_FILESYSTEM
BOOST_IOSTREAMS
BOOST_PROGRAM_OPTIONS
BOOST_SYSTEM
BOOST_THREADS
BOOST_TIMER

PKG_CHECK_MODULES([OPENCV], [opencv4])

AM_CONDITIONAL([DARWIN],[test `uname` == Darwin])
AM_CONDITIONAL([LINUX], [test `uname` == Linux])

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(include/Makefile)
AC_CONFIG_FILES(src/Makefile)

AC_CONFIG_FILES([src/suffix-tree/Makefile])
AC_CONFIG_FILES([src/suffix-array/Makefile])

AC_CONFIG_FILES([src/aho-corasick/Makefile])
AC_CONFIG_FILES([src/cluster/Makefile])
AC_CONFIG_FILES([src/cyclic-rotation/Makefile])
AC_CONFIG_FILES([src/cyclic-substring/Makefile])
AC_CONFIG_FILES([src/knuth-morris-pratt/Makefile])
AC_CONFIG_FILES([src/lcs/Makefile])
AC_CONFIG_FILES([src/lexicographical-merge/Makefile])
AC_CONFIG_FILES([src/longest-prefix/Makefile])
AC_CONFIG_FILES([src/longest-prefix-suffix/Makefile])
AC_CONFIG_FILES([src/lps/Makefile])
AC_CONFIG_FILES([src/lrf/Makefile])
AC_CONFIG_FILES([src/make-anagram/Makefile])
AC_CONFIG_FILES([src/manacher/Makefile])
AC_CONFIG_FILES([src/perf/Makefile])
AC_CONFIG_FILES([src/reduced-string/Makefile])
AC_CONFIG_FILES([src/tandem-arrays/Makefile])
AC_CONFIG_FILES([src/two-characters/Makefile])

AC_OUTPUT()

