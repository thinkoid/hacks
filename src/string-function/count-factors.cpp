// -*- mode: c++ -*-

#include <cassert>

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

static size_t
count_distinct_factors (const suffix_tree_t& suffix_tree) {
    size_t n = 0;

    for (const auto& node : suffix_tree.nodes) {
        for (const auto& edge : node.edges) {
            if (0 == edge.len)
                continue;

            if (0 == edge.end)
                n += suffix_tree.text.size () - edge.pos;
            else
                n += edge.len;
        }
    }

    return n;
}

int main (int, char** argv) {
    const auto suffix_tree = make_suffix_tree (argv [1]);
    cout << count_distinct_factors (suffix_tree) << endl;

    return 0;
}
