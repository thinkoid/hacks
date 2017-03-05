// -*- mode: c++ -*-

#include <cassert>

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

using node_type = suffix_tree_t::node_t;
using edge_type = suffix_tree_t::edge_t;

using node_ref = reference_wrapper< const node_type >;

inline bool
operator< (const node_ref& lhs, const node_ref& rhs) {
    return &lhs.get () < &rhs.get ();
}

static const auto npos = size_t (-1);

struct state_t {
    map< node_ref, size_t > leaves;
    size_t len, maxlen;
};



static size_t
count_leaves (const suffix_tree_t& tree,
              const suffix_tree_t::node_t& node,
              state_t& state) {

    const auto ref = cref (node);

    auto& n = state.leaves [ref];

    if (0 == n) {
        for (const auto& edge : node.edges) {
            if (edge.len) {
                if (0 == edge.end) {
                    ++n;
                }
                else {
                    n += count_leaves (tree, tree.nodes [edge.end], state);
                }
            }
        }
    }

    return n;
}

static void
recursive_visit (const suffix_tree_t& tree,
                 const node_type& node,
                 state_t& state) {

    for (const auto& edge : node.edges) {
        if (0 == edge.len)
            continue;

        if (edge.end) {
            const auto save = state.len;

            state.len += edge.len;

            const auto& node = tree.nodes [edge.end];

            //
            // String function is the product between string length and number
            // of occurrences in the text:
            //
            const auto n = state.len * count_leaves (tree, node, state);

            if (n > state.maxlen)
                state.maxlen = n;

            recursive_visit (tree, node, state);

            state.len = save;
        }
        else {
            const auto n = state.len + tree.text.size () - edge.pos;

            if (n > state.maxlen)
                state.maxlen = n;
        }
    }
}

int main (int, char** argv) {
    const auto suffix_tree = make_suffix_tree (argv [1]);

    state_t state { };
    recursive_visit (suffix_tree, suffix_tree.nodes [0], state);

    cout << state.maxlen << endl;

    return 0;
}
