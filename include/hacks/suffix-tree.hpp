// -*- mode: c++ -*-

#ifndef HACKS_SUFFIX_TREE_HPP
#define HACKS_SUFFIX_TREE_HPP

#include <iostream>
#include <vector>
#include <tuple>

#include <hacks/defs.hpp>

struct suffix_tree_t {
    struct edge_t {
        operator tuple< size_t, int, int, size_t > () const {
            return { s, k, p, s_ };
        }

        size_t s, s_;
        int k, p;
    };

    struct node_t {
        size_t link;
        vector< pair< int, size_t > > edges;
    };

    string text;

    vector< node_t > nodes;
    vector< edge_t > edges;
};

suffix_tree_t
make_suffix_tree (const string&);

#endif // HACKS_SUFFIX_TREE_HPP
