// -*- mode: c++ -*-

#include <cassert>

#include <functional>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

using node_type = suffix_tree_t::node_t;
using edge_type = suffix_tree_t::edge_t;

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

    stack< size_t > overlap_stack;
    overlap_stack.emplace (0);

    stack< pair< size_t, size_t > > traversal_stack;

    size_t i = 0, j = 0;

    while (1) {
        while (j < t.nodes [i].edges.size ()) {
            const auto& edge = t.edges [t.nodes [i].edges [j].second];

            if (edge.len) {
                assert (t.text.size () > edge.pos);

                //
                // Overlap with previous string is at stack top:a
                //
                auto overlap = overlap_stack.top ();
                assert (edge.pos >= overlap);

                //
                // Actual starting position of the substring that contains this
                // edge is overlap characters behind:
                //
                const auto pos = edge.pos - overlap;

                //
                // Insert all substrings that end in the current edge:
                //
                auto from = t.text.begin (), to = from, last = from;

                advance (from, pos);
                advance (to, pos + overlap + 1);

                auto len = overlap;

                if (edge.end)
                    len += edge.len;
                else
                    len += t.text.size () - edge.pos;

                advance (last, pos + len);

                for (; to <= last; ++to, ++overlap) {
                    suffix_array.array.emplace_back (string_view_t { from, to });
                    suffix_array.lcp.emplace_back (overlap);
                }

                if (edge.end) {
                    //
                    // Visit children of intermediate edges:
                    //
                    traversal_stack.emplace (i, j);

                    i = edge.end;
                    j = 0;

                    overlap_stack.emplace (overlap_stack.top () + edge.len);

                    continue;
                }
            }

            ++j;
        }

        if (traversal_stack.empty ())
            break;

        tie (i, j) = traversal_stack.top ();
        traversal_stack.pop ();

        ++j;

        overlap_stack.pop ();
    }

    suffix_array.lcp [0] = npos;

    return suffix_array;
}

int main (int, char** argv) {
    const auto suffix_tree = make_suffix_tree (argv [1]);
    cout << dot_graph_t (suffix_tree, argv [1]).value () << endl;

    const auto suffix_array = make_suffix_array (suffix_tree);

    for (size_t i = 0; i < suffix_array.array.size (); ++i) {
        const string s (suffix_array.array [i].first,
                        suffix_array.array [i].last);

        cout << " --> " << int (suffix_array.lcp [i]) << " : " << s
             << "\n";
    }

    return 0;
}
