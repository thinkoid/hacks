#include <cassert>
#include <cstring>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <string>
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

const auto default_active_point = active_point_t { };

////////////////////////////////////////////////////////////////////////

const auto edge_cmp = [](const auto& lhs, const auto& rhs) {
    return lhs.first < rhs.first;
};

static inline pair< size_t, bool >
edge_index (const node_type& node, size_t c) {
    const pair< size_t, size_t > value { c, 0 };

    const auto iter = lower_bound (
        node.edges.begin (), node.edges.end (), value, edge_cmp);

    pair< size_t, bool > result { };

    if (iter != node.edges.end () && c == iter->first)
        result = pair< size_t, bool > { iter->second, true };

    return result;
}

static active_point_t
canonize (const suffix_tree_t& tree, active_point_t active_point) {
    const auto& text = tree.text;

    while (active_point.off) {
        //
        // The edge information in the active point data is valid only with a
        // non-zero offset, i.e., a zero offset means the active point is at an
        // internal node:
        //
        const auto& node = tree.nodes [active_point.node];
        const auto& edge = tree.edges [active_point.edge];

        if (active_point.off < edge.len)
            break;

        active_point.off -= edge.len;

        assert (edge.end);
        active_point.node = edge.end;

        if (active_point.off) {
            assert (text.size () > edge.pos + edge.len);
            const auto c = text [edge.pos + edge.len];

            const auto& node = tree.nodes [active_point.node];

            const auto result = edge_index (node, size_cast (c));
            assert (result.second);

            active_point.edge = result.first;
        }
    }

    if (0 == active_point.off)
        active_point.edge = 0;

    return active_point;
}

static active_point_t
traverse_suffix_border (const suffix_tree_t& t, active_point_t a) {
    const auto& text = t.text;

    const auto& nodes = t.nodes;
    const auto& edges = t.edges;

    a.node = t.nodes [a.node].link;

    const auto c = text [edges [a.edge].pos];

    const auto result = edge_index (nodes [a.node], size_cast (c));
    assert (result.second);

    a.edge = result.first;

    return move (a);
}

suffix_tree_t
make_suffix_tree (const string& text) {
    static const auto npos = size_t (-1);

    suffix_tree_t t { text, { node_type { } } };
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
                    const auto& node = t.nodes [a.node];
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
                            edge.len == npos ? npos : edge.len - a.off,
                            1
                        });
                    }

                    {
                        auto& node = t.nodes.back ();
                        auto& edge = t.edges [a.edge];

                        const auto next = text [edge.pos + a.off];

                        node.edges.emplace_back (size_cast (next), edge_pos);
                        sort (node.edges.begin (), node.edges.end (), edge_cmp);

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
                    edge_type { curr_node, 0, pos, npos, 1 });

                auto& node = t.nodes [curr_node];

                node.edges.emplace_back (size_cast (c), edge_pos);
                sort (node.edges.begin (), node.edges.end (), edge_cmp);
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
count_leaves (const suffix_tree_t& tree, size_t node) {
    // TODO: operate on node references
    size_t n = 0;

    for (const auto& p : tree.nodes [node].edges) {
        const auto& edge = tree.edges [p.second];

        if (edge.len) {
            if (0 == edge.end)
                ++n;
            else
                n += count_leaves (tree, edge.end);
        }
    }

    return n;
}

size_t
count_leaves (const suffix_tree_t& tree) {
    return count_leaves (tree, 0);
}

size_t
count_distinct_factors (const suffix_tree_t& tree) {
    size_t n = 0;

    for (const auto& node : tree.nodes) {
        for (const auto& p : node.edges) {
            const auto& edge = tree.edges [p.second];

            if (0 == edge.len)
                continue;

            if (0 == edge.end)
                n += tree.text.size () - edge.pos;
            else
                n += edge.len;
        }
    }

    return n;
}

////////////////////////////////////////////////////////////////////////

/* static */ string
dot_graph_t::make_dot (const suffix_tree_t& t, const std::string& s) {
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
