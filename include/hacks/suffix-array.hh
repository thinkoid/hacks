// -*- mode: c++ -*-

#ifndef HACKS_SUFFIX_ARRAY_HH
#define HACKS_SUFFIX_ARRAY_HH

#include <hacks/defs.hh>
#include <hacks/suffix-tree.hh>

#include <string>
#include <vector>

struct string_view_t {
    size_t pos, len;
};

struct suffix_array_t {
    const std::string text;
    std::vector< string_view_t > arr;
    std::vector< size_t > lcp;
};

suffix_array_t
make_suffix_array (const std::string&);

#endif // HACKS_SUFFIX_ARRAY_HH
