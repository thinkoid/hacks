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

template< typename T >
inline auto size_cast (T value) {
    return size_t (make_unsigned_t< T > (value));
}

static string text;

struct node_t {
    int suffix_link;
};

static vector< node_t > nodes { { -1 } };

struct suffix_t {
    int node, first, last;
};

static inline ostream&
operator<< (ostream& s, const suffix_t& active_point) {
    return s << active_point.node  << ","
             << active_point.first << ","
             << active_point.last;
}

static inline int
is_explicit (const suffix_t& active_point) {
    return active_point.first > active_point.last;
}

struct edge_t {
    int beg, end, first, last;

    int split_edge (suffix_t&);
};

static inline bool
operator== (const edge_t& lhs, const edge_t& rhs) {
    return
        lhs.beg == rhs.beg && lhs.end == rhs.end &&
        lhs.first == rhs.last && lhs.last == rhs.last;
}

static inline bool
operator!= (const edge_t& lhs, const edge_t& rhs) {
    return !(lhs == rhs);
}

const auto default_edge = edge_t { 0, 0, 0, 0 };

static inline ostream&
operator<< (ostream& s, const edge_t& edge) {
    return s << edge.beg << " -> " << edge.end << " : "
             << edge.first << "(" << text [edge.first] << "), "
             << edge.last << "(" << text [edge.last];
}

static vector< vector< edge_t > > edges { vector< edge_t > (256U) };

int
edge_t::split_edge (suffix_t& s) {
    edges [beg][size_cast (text [first])] = edge_t { };

    nodes.emplace_back (node_t { -1 });
    edges.emplace_back (vector< edge_t > (256U));

    assert (nodes.size () == edges.size ());

    edge_t edge = {
        s.node, int (nodes.size ()) - 1, first, first + s.last - s.first
    };

    edges [s.node][size_cast (text [first])] = edge;

    auto& node = nodes [edge.end];
    node.suffix_link = s.node;

    first += s.last - s.first + 1;
    beg = edge.end;

    edges [beg][size_cast (text [first])] = *this;

    return edge.end;
}

////////////////////////////////////////////////////////////////////////

static suffix_t
canonize (suffix_t arg) {
    cout << "//   -- canonize : " << arg << " : ";

    if (!is_explicit (arg)) {
        auto edge = edges [arg.node][size_cast (text [arg.first])];
        int len = edge.last - edge.first;

        while (len <= (arg.last - arg.first)) {
            arg.first = arg.first + len + 1;
            arg.node = edge.end;

            if (arg.first <= arg.last) {
                edge = edges [edge.end][size_cast (text [arg.first])];
                len = edge.last - edge.first;
            };
        }
    }

    cout << arg << endl;

    return arg;
}

////////////////////////////////////////////////////////////////////////

static void
add_prefix (suffix_t& active_point, int last) {
    int parent_node, last_parent_node = -1;

    while (1) {
        edge_t edge;

        parent_node = active_point.node;

        if (is_explicit (active_point)) {
            edge = edges [active_point.node][size_cast (text [last])];

            if (edge != default_edge)
                break;
        }
        else {
            auto suffix_len = active_point.last - active_point.first + 1;

            edge = edges [active_point.node][size_cast (text [active_point.first])];
            assert (edge.beg == active_point.node);

            if (text [edge.first + suffix_len] == text [last])
                break;

            parent_node = edge.split_edge (active_point);
        }

        {
            nodes.emplace_back (node_t { -1 });
            edges.emplace_back (vector< edge_t > (256U));

            edge_t edge = {
                parent_node, int (nodes.size ()) - 1, last, int (text.size ()) - 1
            };

            edges [parent_node][size_cast (text [last])] = edge;
        }

        if (last_parent_node > 0) {
            assert (size_t (last_parent_node) < nodes.size ());
            nodes [last_parent_node].suffix_link = parent_node;
        }

        last_parent_node = parent_node;

        if (0 == active_point.node) {
            active_point.first++;
        }
        else {
            assert (size_t (active_point.node) < nodes.size ());
            active_point.node = nodes [active_point.node].suffix_link;
        }

        active_point = canonize (active_point);
    }

    if (last_parent_node > 0) {
        assert (size_t (last_parent_node) < nodes.size ());
        nodes [last_parent_node].suffix_link = parent_node;
    }

    ++active_point.last;
    active_point = canonize (active_point);
};

////////////////////////////////////////////////////////////////////////

//
// This routine  prints out the contents  of the suffix  tree at the end  of the
// program by  walking through the hash  table and printing out  all used edges.
// It would be really great if we had some code that will print out the tree in a
// graphical fashion, but we don't!
//
static void
dump_edges () {
    cout << "//\n//  Start  End  Suf  First Last  String\n";

    for (const auto& v : edges) {
        for (const auto& edge : v) {
            if (0 > edge.beg)
                continue;

            cout << "// " << right
                 << setw (5) << edge.beg << " "
                 << setw (5) << edge.end << " "
                 << setw (3) << nodes [edge.end].suffix_link << " "
                 << setw (5) << edge.first << " "
                 << setw (6) << edge.last << "  ";

            cout << setw (15) << left
                 << text.substr (edge.first,
                                 edge.last - edge.first + 1)
                 << "\n";
        }
    }

    cout << endl;
}

static string
make_dot (size_t i) {
    stringstream ss;

    ss << "#+BEGIN_SRC dot :file suffix-tree-" << i << ".png :cmdline -Kdot -Tpng\n";
    ss << "digraph suffix_tree_" << i << " {\n";

#if 0
    for (size_t i = 0; i < nodes.size (); ++i) {
        if (0 <= nodes [i].suffix_link)
            ss << "    " << i << " -> " << nodes [i].suffix_link
               << " [style=dotted];\n";
    }
#endif // 0

    for (const auto& v : edges) {
        for (auto& edge : v) {
            if (edge != default_edge) {
                ss << "    " << edge.beg << " -> " << edge.end;

                const size_t
                    pos = edge.first,
                    len = edge.last - edge.first + 1;

                ss << " [label=\"" << text.substr (pos, len) << "\"];\n";
            }
        }
    }

    ss << "}\n";
    ss << "#+END_SRC\n\n";

    return ss.str ();
}

////////////////////////////////////////////////////////////////////////

int main (int, char** argv) {
    assert (argv [1] && argv [1][0]);
    text = string (argv [1]);

    suffix_t active_point { 0, 0, -1 };

    size_t i = 0;
    for (; i < text.size (); ++i) {
        add_prefix (active_point, i);
    }

#if 0
    dump_edges ();
#endif // 0

    cout << make_dot (i) << endl;

    return 0;
}
