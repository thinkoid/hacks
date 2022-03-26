// -*- mode: c++ -*-

#ifndef HACKS_SUFFIX_ARRAY_HH
#define HACKS_SUFFIX_ARRAY_HH

#include <hacks/defs.hh>
#include <hacks/suffix-tree.hh>

struct string_view_t {
    size_t pos, len;
};

struct suffix_array_t {
    const string text;
    vector< string_view_t > arr;
    vector< size_t > lcp;
};

suffix_array_t
make_suffix_array (const string&);

#endif // HACKS_SUFFIX_ARRAY_HH
