// -*- mode: c++ -*-

#include <cassert>

#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

static size_t
count_distinct_factors (const suffix_tree_t& suffix_tree) {
    stack< pair< size_t, size_t > > st;

    size_t i = 0, j = 0, n = 0;

    while (true) {
        while (j < suffix_tree.nodes [i].edges.size ()) {
            const auto& edge = suffix_tree.nodes [i].edges [j];

            if (edge.len) {
                if (edge.end) {
                    n += edge.len;

                    st.emplace (i, j);

                    i = edge.end;
                    j = 0;

                    continue;
                }

                n += suffix_tree.text.size () - edge.pos;
            }

            ++j;
        }

        if (st.empty ())
            break;

        tie (i, j) = st.top ();
        st.pop ();

        ++j;
    }

    return n;
}

int main (int, char** argv) {
    const auto suffix_tree = make_suffix_tree (argv [1]);
    cout << count_distinct_factors (suffix_tree) << endl;

    return 0;
}
