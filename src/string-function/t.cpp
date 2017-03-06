// -*- mode: c++ -*-

#include <cassert>

#include <functional>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

struct state_t {
    size_t len, area;
};

static void
calculate_string_function (
    const suffix_tree_t& tree, size_t node, state_t& state) {

    for (const auto& edge : tree.nodes [node].edges) {
        if (0 == edge.len)
            continue;

        if (edge.end) {
            const auto save = state.len;

            state.len += edge.len;

            {
                const auto area = state.len * count_leaves (tree, edge.end);

                if (area > state.area)
                    state.area = area;
            }

            calculate_string_function (tree, edge.end, state);

            state.len = save;
        }
        else {
            const auto n = state.len + tree.text.size () - edge.pos;

            if (n > state.area)
                state.area = n;
        }
    }
}

static size_t
calculate_string_function (const suffix_tree_t& tree) {
    state_t state { };
    calculate_string_function (tree, 0, state);
    return state.area;
}

int main (int, char** argv) {
    const auto suffix_tree = make_suffix_tree (argv [1]);
    cout << calculate_string_function (suffix_tree) << endl;

    return 0;
}
