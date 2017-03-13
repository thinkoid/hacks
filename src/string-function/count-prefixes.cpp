// -*- mode: c++ -*-

#include <cassert>

#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

using state_type = map< size_t, size_t >;

static void
count_prefixes (const suffix_tree_t& t, size_t n) {
    cout << n << ":\n";

    const auto& node = t.nodes [n];

    for (const auto& p : node.edges) {
        const auto& edge = t.edges [p.second];

        if (0 == edge.len)
            continue;

        cout << " --> " << edge.end << " : "
             << t.text.substr (edge.pos, edge.len) << " : " << edge.counter
             << "\n";

        if (edge.end)
            count_prefixes (t, edge.end);
    }
}

int main (int, char** argv) {
    count_prefixes (make_suffix_tree (argv [1]), 0);
    return 0;
}
