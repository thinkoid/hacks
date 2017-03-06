// -*- mode: c++ -*-

#ifndef HACKS_SUFFIX_TREE_HPP
#define HACKS_SUFFIX_TREE_HPP

#include <hacks/defs.hpp>

struct suffix_tree_t {
    struct edge_t {
        size_t beg, end, pos, len;
    };

    struct node_t {
        size_t parent, link;
        vector< edge_t > edges;
    };

    const string text;
    vector< node_t > nodes;
};

inline ostream&
operator<< (ostream& s, const suffix_tree_t::edge_t& arg) {
    return s << arg.beg << "," << arg.end << "," << arg.pos << "," << arg.len;
}

inline bool
operator== (const suffix_tree_t::edge_t& lhs,
            const suffix_tree_t::edge_t& rhs) {
    return
        lhs.beg == rhs.beg && lhs.end == rhs.end &&
        lhs.pos == rhs.pos && lhs.len == rhs.len;
}

inline bool
operator!= (const suffix_tree_t::edge_t& lhs,
            const suffix_tree_t::edge_t& rhs) {
    return !(lhs == rhs);
}

suffix_tree_t
make_suffix_tree (const string&);

size_t
count_leaves (const suffix_tree_t&, size_t);

size_t
count_leaves (const suffix_tree_t&);

struct dot_graph_t {
    explicit dot_graph_t (const suffix_tree_t& tree, const string& s)
        : value_ (make_dot (tree, s))
        { }

    const string& value () const {
        return value_;
    }

private:
    static string
    make_dot (const suffix_tree_t&, const std::string&);

private:
    string value_;
};

#endif // HACKS_SUFFIX_TREE_HPP
