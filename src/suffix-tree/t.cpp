#include <cassert>
#include <cstring>

#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

#include <boost/format.hpp>
using fmt = boost::format;

#include <benchmark/benchmark.h>

template< typename T >
inline auto size_cast (T value) {
    return size_t (make_unsigned_t< T > (value));
}

////////////////////////////////////////////////////////////////////////

static const auto npos = size_t (-1);

struct edge_t {
    size_t beg, end, pos, len;
};

static inline ostream&
operator<< (ostream& s, const edge_t& arg) {
    return s << arg.beg << "," << arg.end << "," << arg.pos << "," << arg.len;
}

static const auto default_edge = edge_t { };

static inline bool
operator== (const edge_t& lhs, const edge_t& rhs) {
    return
        lhs.beg == rhs.beg && lhs.end == rhs.end &&
        lhs.pos == rhs.pos && lhs.len == rhs.len;
}

static inline bool
operator!= (const edge_t& lhs, const edge_t& rhs) {
    return !(lhs == rhs);
}

struct node_t {
    size_t parent, link;
    vector< edge_t > edges;
};

static const auto default_node = node_t {
    { }, { }, vector< edge_t > (256U, default_edge)
};

struct suffix_tree_t {
    const string text;
    vector< node_t > nodes;
};

////////////////////////////////////////////////////////////////////////

struct active_point_t {
    size_t node, head, off;
};

const auto default_active_point = active_point_t { };

////////////////////////////////////////////////////////////////////////

static active_point_t
canonize (const suffix_tree_t& tree, active_point_t active_point) {
    while (active_point.off) {

        auto& node = tree.nodes [active_point.node];
        auto& edge = node.edges [active_point.head];

        assert (edge != default_edge);

        if (active_point.off < edge.len)
            break;

        active_point.off -= edge.len;

        if (active_point.off) {
            assert (tree.text.size () > edge.pos + edge.len);
            active_point.head = tree.text [edge.pos + edge.len];
        }

        assert (edge.end);
        active_point.node = edge.end;
    }

    if (0 == active_point.off)
        active_point.head = 0;

    return active_point;
}

static suffix_tree_t
make_suffix_tree (const string& text) {
    suffix_tree_t tree { text, { default_node } };

    active_point_t active_point = default_active_point;

    for (size_t pos = 0; pos < text.size (); ++pos) {
        size_t prev_node = npos, curr_node = npos;

        for (const auto c = text [pos];;) {
            curr_node = active_point.node;

            if (0 == active_point.off) {
                //
                // Active point is at explicit node:
                //
                auto& node = tree.nodes [active_point.node];
                auto& edge = node.edges [size_cast (c)];

                if (edge != default_edge) {
                    //
                    // Matching along the found edge:
                    //
                    active_point.head = size_cast (c);
                    ++active_point.off;

                    active_point = canonize (tree, active_point);

                    break;
                }
            }
            else {
                //
                // Active point is at implicit node:
                //
                auto& node = tree.nodes [active_point.node];
                auto& edge = node.edges [active_point.head];

                const auto off = active_point.off;
                assert (edge.len > off);

                char next = text [edge.pos + off];

                if (c == next) {
                    //
                    // Matching along the current edge:
                    //
                    ++active_point.off;

                    active_point = canonize (tree, active_point);

                    break;
                }

                //
                // Mismatch, split the current edge:
                //
                const auto fork_pos = tree.nodes.size ();

                {
                    const node_t node {
                        edge.beg, 0U, vector< edge_t > (256U, default_edge)
                    };

                    tree.nodes.emplace_back (node);
                }

                tree.nodes.back ().edges [size_cast (next)] = edge_t {
                    fork_pos, edge.end, edge.pos + off, npos
                };

                edge.end = fork_pos;
                edge.len = off;

                curr_node = fork_pos;

                //
                // Suffix-link this node:
                //
                if (npos != prev_node)
                    tree.nodes [prev_node].link = curr_node;

                prev_node = curr_node;
            }

            //
            // Either an explicit node has no edge out matching the current
            // position, or the edge has been split at the implicit node -- in
            // either case a new edge is required at the current node:
            //
            auto& node = tree.nodes [curr_node];
            auto& edge = node.edges [size_cast (c)];

            edge = { curr_node, 0, pos, npos };

            if (0 == active_point.node) {
                if (0 == active_point.off)
                    break;

                if (--active_point.off)
                    active_point.head = text [pos - active_point.off];
            }
            else {
                active_point.node = tree.nodes [active_point.node].link;
            }

            active_point = canonize (tree, active_point);
        }
    }

    return tree;
}

////////////////////////////////////////////////////////////////////////

struct dot_graph_t {
    explicit dot_graph_t (const suffix_tree_t& tree, const string& s)
        : value_ (make_dot (tree, s))
        { }

    const string& value () const {
        return value_;
    }

private:
    static string
    make_dot (const suffix_tree_t& tree, const std::string& s) {
        stringstream ss;

        ss << "#+BEGIN_SRC dot :file suffix-tree.png :cmdline -Kdot -Tpng\n";
        ss << "digraph suffix_tree {\n";

        for (size_t i = 0; i < tree.nodes.size (); ++i) {
            ss << "    " << i;
            if (0 == i)
                ss << "[shape=square]";
            ss << ";\n";
        }

        size_t leaf = tree.nodes.size ();
        vector< size_t > leaves;

        for (const auto& node : tree.nodes)
            for (const auto& edge : node.edges) {
                if (edge != default_edge) {
                    ss << "    "
                       << edge.beg << " -> ";

                    if (0 == edge.end) {
                        ss << leaf;
                        leaves.push_back (leaf++);
                    }
                    else
                        ss << edge.end;

                    ss << " [label=\""
                       << s.substr (edge.pos, edge.len)
                       << "\"]\n";
                }
            }

        for (auto leaf : leaves) {
            ss << "    " << leaf << "[shape=point];\n";
        }

#if 0
        for (size_t i = 0; i < tree.nodes.size (); ++i) {
            const auto& node = tree.nodes [i];

            if (node.link)
                ss << "    " << i << " -> " << node.link
                   << " [style=dotted]\n";
        }
#endif // 0

        ss << "}\n";
        ss << "#+END_SRC\n\n";

        return ss.str ();
    }

private:
    string value_;
};

#if 1

int main (int, char** argv) {
    assert (argv [1] && argv [1][0]);

    suffix_tree_t tree = make_suffix_tree (argv [1]);
    cout << dot_graph_t (tree, argv [1]).value () << endl;

     return 0;
 }

#else

static std::string
make_test_string (size_t n, size_t m) {
    static std::string template_ = "AAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYYAAZCCDXEBFCCDXEGHAABDEFWCCGDEFWHYY";

    const std::string s = template_.substr (0, n);
    std::stringstream ss;

    for (size_t i = 0; i < m; ++i)
        ss << s;

    return ss.str ();
}

static void
BM_t (benchmark::State& state) {
    const string s = make_test_string (state.range (0), state.range (1));

    while (state.KeepRunning ())
        volatile auto tree = make_suffix_tree (s);
}

BENCHMARK (BM_t)
->Args ({  4, 4096})
->Args ({ 16, 4096})
->Args ({ 32, 4096})
->Args ({ 64, 4096})
->Args ({128, 4096})
->Args ({256, 4096})
->Args ({512, 4096})
;

BENCHMARK_MAIN ();

#endif // 0
