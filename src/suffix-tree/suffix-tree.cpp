#include <cstddef>
#include <cassert>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

template< typename T >
inline auto size_cast (T value) {
    return size_t (make_unsigned_t< T > (value));
}

////////////////////////////////////////////////////////////////////////

using node_type = suffix_tree_t::node_t;
using edge_type = suffix_tree_t::edge_t;

struct active_point_t {
    size_t node, edge, off;
};

////////////////////////////////////////////////////////////////////////

static inline pair< size_t, bool >
edge_index (const node_type& node, size_t c) {
    const auto iter = find_if (
        node.edges.begin (), node.edges.end (), [&](const auto& arg) {
            return c == arg.first;
        });

    pair< size_t, bool > result { };

    if (iter != node.edges.end ())
        result = pair< size_t, bool > { iter->second, true };

    return result;
}

static active_point_t
canonize (const suffix_tree_t& t, active_point_t a) {
    const auto& text = t.text;

    while (a.off) {
        //
        // The edge information in the active point data is valid only with a
        // non-zero offset, i.e., a zero offset means the active point is at an
        // internal node:
        //
        const auto& edge = t.edges [a.edge];

        if (a.off < edge.len)
            break;

        a.off -= edge.len;

        assert (edge.end);
        a.node = edge.end;

        if (a.off) {
            assert (text.size () > edge.pos + edge.len);
            const auto c = text [edge.pos + edge.len];

            const auto& node = t.nodes [a.node];

            const auto result = edge_index (node, size_cast (c));
            assert (result.second);

            a.edge = result.first;
        }
    }

    if (0 == a.off)
        a.edge = 0;

    return a;
}

static active_point_t
traverse_suffix_border (const suffix_tree_t& t, active_point_t a) {
    const auto& text = t.text;

    const auto& nodes = t.nodes;
    const auto& edges = t.edges;

    a.node = t.nodes [a.node].link;

    const auto c = text [edges [a.edge].pos];

    const auto result = edge_index (nodes [a.node], size_cast (c));
    a.edge = result.second ? result.first : 0;

    return a;
}

suffix_tree_t
make_suffix_tree (const string& text) {
    static const auto npos = size_t (-1);

    suffix_tree_t t { text, { node_type { } }, { } };
    active_point_t a { };

    for (size_t pos = 0; pos < text.size (); ++pos) {
        size_t prev_node = npos, curr_node = npos;

        for (const auto c = text [pos]; ;) {
            curr_node = a.node;

            if (0 == a.off) {
                //
                // Active point is at explicit node:
                //
                const auto& node = t.nodes [a.node];

                const auto result = edge_index (node, size_cast (c));

                if (result.second) {
                    //
                    // Matching edge out of current node:
                    //
                    a.edge = result.first;

                    ++a.off;
                    a = canonize (t, a);

                    break;
                }
            }
            else {
                {
                    //
                    // Check if the character matches at the current active
                    // point:
                    //
                    const auto& edge = t.edges [a.edge];

                    assert (edge.len > a.off);
                    const auto next = text [edge.pos + a.off];

                    if (c == next) {
                        //
                        // Adjust active point when matching:
                        //
                        ++a.off;
                        a = canonize (t, a);

                        break;
                    }
                }

                {
                    const auto node_pos = t.nodes.size ();
                    const auto edge_pos = t.edges.size ();

                    {
                        const auto& edge = t.edges [a.edge];

                        //
                        // Insert explicit node at the implicit node position:
                        //
                        t.nodes.emplace_back (node_type { edge.beg, 0U, { } });

                        t.edges.emplace_back (edge_type {
                            node_pos, edge.end, edge.pos + a.off,
                            edge.len == npos ? npos : edge.len - a.off
                        });
                    }

                    {
                        auto& node = t.nodes.back ();
                        auto& edge = t.edges [a.edge];

                        const auto next = text [edge.pos + a.off];
                        node.edges.emplace_back (size_cast (next), edge_pos);

                        //
                        // Cut active edge:
                        //
                        edge.end = node_pos;
                        edge.len = a.off;
                    }

                    curr_node = node_pos;

                    //
                    // Suffix-link this node:
                    //
                    if (npos != prev_node)
                        t.nodes [prev_node].link = curr_node;

                    prev_node = curr_node;
                }
            }

            {
                //
                // Either an explicit node has no edge out matching the current
                // position, or the edge has been split at the implicit node --
                // a new edge is required at the current node:
                //
                const auto edge_pos = t.edges.size ();

                t.edges.emplace_back (
                    edge_type { curr_node, 0, pos, npos });

                auto& node = t.nodes [curr_node];
                node.edges.emplace_back (size_cast (c), edge_pos);
            }

            if (0 == a.node) {
                if (0 == a.off)
                    break;

                if (--a.off) {
                    //
                    // Recompute the active point edge:
                    //
                    const auto c = text [pos - a.off];

                    const auto result = edge_index (t.nodes [0], size_cast (c));
                    assert (result.second);

                    a.edge = result.first;
                }
            }
            else {
                //
                // Traverse alongside the suffix border line:
                //
                a = traverse_suffix_border (t, a);
            }

            a = canonize (t, a);
        }
    }

    return t;
}

////////////////////////////////////////////////////////////////////////

size_t
count_leaves (const suffix_tree_t& t, size_t n) {
    const auto& s = t.nodes [n].edges;

    return accumulate (
        s.begin (), s.end (), 0U,
        [&](const auto& accum, const auto& arg) {
            const auto& edge = t.edges [arg.second];
            return accum + edge.end ? count_leaves (t, edge.end) : 1;
        });
}

size_t
count_leaves (const suffix_tree_t& t) {
    return count_leaves (t, 0);
}

vector< size_t >
count_all_leaves (const suffix_tree_t& t) {
    vector< size_t > v (t.nodes.size (), { });

    stack< tuple< size_t, size_t, size_t > > state;
    size_t i = 0, j = 0, k = 0;

    while (1) {
        while (j < t.nodes [i].edges.size ()) {

            const auto& node = t.nodes [i];
            const auto& edge = t.edges [node.edges [j].second];

            if (edge.end) {
                state.emplace (i, j, k);

                i = edge.end;
                j = k = 0;

                continue;
            }
            else
                ++k;

            ++j;
        }

        v [i] = k;

        if (state.empty ())
            break;

        const size_t save = k;

        tie (i, j, k) = state.top ();
        state.pop ();

        ++j;

        k += save;
    }

    return v;
}

size_t
count_distinct_factors (const suffix_tree_t& t) {
    size_t n = 0;

    for (const auto& node : t.nodes) {
        for (const auto& p : node.edges) {
            const auto& edge = t.edges [p.second];

            if (0 == edge.len)
                continue;

            if (0 == edge.end)
                n += t.text.size () - edge.pos;
            else
                n += edge.len;
        }
    }

    return n;
}

vector< size_t >
match (const suffix_tree_t& t, const string& s) {
    vector< size_t > path;

    size_t pos = 0;

    for (auto iter = s.begin (), last = s.end (); iter != last;) {
        //
        // Record node path:
        //
        path.emplace_back (pos);

        //
        // Find the matching edge:
        //
        const auto result = edge_index (t.nodes [pos], *iter);

        if (!result.second) {
            path.clear ();
            break;
        }

        //
        // Match alongside the edge:
        //
        const auto& edge = t.edges [result.first];

        auto iter2 = t.text.begin ();
        advance (iter2, edge.pos);

        auto last2 = iter2;

        if (edge.end)
            advance (last2, edge.len);
        else
            last2 = t.text.end ();

        for (; iter != last && iter2 != last2 && *iter == *iter2;
             ++iter, ++iter2) ;

        if (iter == last) {
            //
            // Matched the whole string:
            //
            path.emplace_back (edge.end);
        }
        else {
            if (iter2 == last2) {
                //
                // Matched the whole edge:
                //
                if (edge.end) {
                    //
                    // Continue from the end of the edge:
                    //
                    pos = edge.end;
                }
                else {
                    //
                    // Mismatch, the tree ends here:
                    //
                    path.clear ();
                    break;
                }
            }
            else {
                //
                // Mismatch on the current edge:
                //
                path.clear ();
                break;
            }
        }
    }

    return path;
}

////////////////////////////////////////////////////////////////////////

/* static */ string
dot_graph_t::make_dot (const suffix_tree_t& t) {
    stringstream ss;

    ss << "#+BEGIN_SRC dot :file suffix-t.png :cmdline -Kdot -Tpng\n";
    ss << "digraph suffix_tree {\n";

    for (size_t i = 0; i < t.nodes.size (); ++i) {
        ss << "    " << i;
        if (0 == i)
            ss << "[shape=square]";
        ss << ";\n";
    }

    size_t leaf = t.nodes.size ();
    vector< size_t > leaves;

    for (const auto& node : t.nodes)
        for (const auto& p : node.edges) {
            const auto& edge = t.edges [p.second];

            ss << "    " << edge.beg << " -> ";

            if (0 == edge.end) {
                ss << leaf;
                leaves.push_back (leaf++);
            }
            else
                ss << edge.end;

            ss << " [label=\""
               << t.text.substr (edge.pos, edge.len)
               << "\"];";

            ss << "// " << edge.pos << ", " << edge.len
               << "\n";
        }

    for (auto leaf : leaves) {
        ss << "    " << leaf << "[shape=point];\n";
    }

    ss << "}\n";
    ss << "#+END_SRC\n\n";

    return ss.str ();
}
