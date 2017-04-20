// -*- mode: c++ -*-

#ifndef HACKS_SUFFIX_ARRAY_HPP
#define HACKS_SUFFIX_ARRAY_HPP

#include <hacks/defs.hpp>
#include <hacks/suffix-tree.hpp>

struct string_view_t {
    size_t pos, len;
};

struct suffix_array_t {
    const string text;
    vector< string_view_t > arr;
    vector< size_t > lcp;
};

suffix_array_t
make_suffix_array (const suffix_tree_t&);

#endif // HACKS_SUFFIX_ARRAY_HPP
