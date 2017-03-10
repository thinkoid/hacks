// -*- mode: c++ -*-

#include <cassert>

#include <functional>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

const size_t npos = size_t (-1);

struct string_view_t {
    string::const_iterator first, last;
};

struct suffix_array_t {
    const string text;
    vector< string_view_t > array;
    vector< size_t > lcp;
};

static suffix_array_t
make_suffix_array (const suffix_tree_t& t) {
    suffix_array_t suffix_array { t.text };

    stack< size_t > overlaps;
    overlaps.emplace (0);

    stack< pair< size_t, size_t > > state;

    size_t i = 0, j = 0;

    while (1) {
        while (j < t.nodes [i].edges.size ()) {
            const auto& edge = t.nodes [i].edges [j];

            if (edge.len) {
                //
                // Leaf edge:
                //
                const auto overlap = overlaps.top ();

                assert (edge.pos >= overlap);
                const auto pos = edge.pos - overlap;

                assert (t.text.size () > edge.pos);

                auto len = overlap;

                if (edge.end)
                    len += edge.len;
                else
                    len += t.text.size () - edge.pos;

                string::const_iterator first = t.text.begin (), last = first;

                advance (first, pos);
                advance (last, pos + len);

                suffix_array.array.emplace_back (string_view_t { first, last });

                suffix_array.lcp.emplace_back (overlap);

                if (edge.end) {
                    //
                    // Intermediate edge:
                    //
                    state.emplace (i, j);

                    i = edge.end;
                    j = 0;

                    overlaps.emplace (overlap + edge.len);

                    continue;
                }
            }

            ++j;
        }

        if (state.empty ())
            break;

        tie (i, j) = state.top ();
        state.pop ();

        ++j;

        overlaps.pop ();
    }

    return suffix_array;
}

int main (int, char** argv) {
    const auto arr = make_suffix_array (make_suffix_tree (argv [1]));

    for (const auto& view : arr.array)
        cout << string (view.first, view.last) << "\n";

    return 0;
}
