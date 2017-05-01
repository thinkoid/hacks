#include <cassert>

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

//
// E. Ukkonen: Constructing suffix trees on-line in linear time.
// Proc. Information Processing 92, Vol. 1, IFIP Transactions A-12,
// 484-492, Elsevier 1992.
//

template< typename T, size_t N, typename ... REST >
struct tuple_generator_n {
    using type = typename tuple_generator_n< T, N - 1, T, REST ... >::type;
};

template< typename T, typename ... REST >
struct tuple_generator_n< T, 0, REST ... > {
    using type = std::tuple< REST ... >;
};

template< typename T, size_t N >
using tuple_n = typename tuple_generator_n< T, N >::type;

using node_type = suffix_tree_t::node_t;
using edge_type = suffix_tree_t::edge_t;

static constexpr size_t AUX = 0, ROOT = 1, INF = (numeric_limits< int >::max)();

////////////////////////////////////////////////////////////////////////

static inline const node_type&
node_at (const suffix_tree_t& t, size_t s) {
    assert (s < t.nodes.size ());
    return t.nodes [s];
}

static inline node_type&
node_at (suffix_tree_t& t, size_t s) {
    assert (s < t.nodes.size ());
    return t.nodes [s];
}

static inline bool
has_transition (const node_type& node, int c) {
    auto iter = find_if (
        node.edges.begin (), node.edges.end (), [&](const auto& arg) {
            return c == arg.first;
        });

    return iter != node.edges.end ();
}

static inline bool
has_transition (const suffix_tree_t& t, size_t s, int c) {
    //
    // Special case for the transitions out of the auxilliary state, saving
    // the creation of a bunch of edges:
    //
    return AUX == s || has_transition (node_at (t, s), c);
}

static inline tuple< size_t, int, int, size_t >
g_ (const suffix_tree_t& t, size_t s, int c) {
    size_t s_ = 1, k = 0, p = 0;

    if (AUX != s) {
        //
        // Special case for the transitions out of the auxilliary state, saving
        // the creation of a bunch of edges:
        //
        auto& edges = node_at (t, s).edges;

        const auto iter = find_if (
            edges.begin (), edges.end (), [&](const auto& arg) {
                return c == arg.first;
            });

        assert (iter != edges.end ());

        tie (s, k, p, s_) = tuple< size_t, int, int, size_t > (
            t.edges [iter->second]);
    }

    return tie (s, k, p, s_);
}

static size_t&
g_ (suffix_tree_t& t, size_t s, tuple_n< int, 2 > ref) {
    int k, p;
    tie (k, p) = ref;

    auto& edges = node_at (t, s).edges;

    auto iter = find_if (
        edges.begin (), edges.end (), [&](const auto& arg) {
            return t.text [k] == arg.first;
        });

    if (iter == edges.end ()) {
        //
        // Insert a new edge if not already present:
        //
        const auto e = t.edges.size ();

        t.edges.emplace_back (edge_type { s, { }, k, p });
        edges.emplace_back (t.text [k], e);

        iter = --edges.end ();
    }
    else {
        //
        // Modify the existing edge with incoming arguments:
        //
        auto& edge = t.edges [iter->second];

        edge.k = k;
        edge.p = p;
    }

    return t.edges [iter->second].s_;
}

////////////////////////////////////////////////////////////////////////

static tuple< size_t, int >
canonize (const suffix_tree_t& t, size_t s, tuple_n< int, 2 > ref) {
    size_t s_, ignore;
    int k, p, k_, p_;

    tie (k, p) = ref;
    assert (k >= 0);

    if (k <= p) {
        const auto tk = t.text [k];

        tie (ignore, k_, p_, s_) = g_ (t, s, tk);
        assert (k_ >= 0);

        while (p_ - k_ <= p - k) {
            k = k + p_ - k_ + 1;
            s = s_;

            if (k <= p) {
                const auto tk = t.text [k];

                tie (ignore, k_, p_, s_) = g_ (t, s, tk);
                assert (k_ >= 0);
            }
        }
    }

    return { s, k };
}

static tuple< size_t, bool >
test_and_split (suffix_tree_t& t, size_t s, tuple_n< int, 2 > ref, int c) {
    size_t s_, ignore;
    int k, p, k_, p_;

    tie (k, p) = ref;
    assert (k >= 0);

    if (k <= p) {
        const auto tk = t.text [k];

        //
        // Find tk-transition g'(s,(k',p'))=s':
        //
        tie (ignore, k_, p_, s_) = g_ (t, s, tk);
        assert (k_ >= 0);

        if (c == t.text [k_ + p - k + 1])
            return { s, true };
        else {
            //
            // Create new state, r:
            //
            const auto r = t.nodes.size ();
            t.nodes.emplace_back ();

            //
            // Modify existing transition, g'(s,(k',k'+p-k))=r, and create a new
            // transition, g'(r,(k'+p-k+1,p'))=s':
            //
            g_ (t, s, { k_, k_ + p - k }) = r;
            g_ (t, r, { k_ + p - k + 1, p_ }) = s_;

            return { r, false };
        }
    }
    else {
        return { s, has_transition (t, s, c) };
    }
}

static tuple< size_t, int >
update (suffix_tree_t& t, size_t s, tuple_n< int, 2 > ref) {
    int k, i;

    tie (k, i) = ref;
    assert (k >= 0);

    const auto ti = t.text [i];

    bool b;
    size_t oldr = ROOT, r = 0;

    tie (r, b) = test_and_split (t, s, { k, i - 1 }, ti);

    while (!b) {
        //
        // Create new transition g'(r,(i,∞))=r'
        //
        {
            const auto r_ = t.nodes.size ();
            const auto e  = t.edges.size ();

            t.edges.emplace_back (edge_type { r, r_, i, INF });
            t.nodes.emplace_back ();

            t.nodes [r].edges.emplace_back (t.text [i], e);
        }

        if (oldr != ROOT)
            t.nodes [oldr].link = r;

        oldr = r;

        tie (s, k) = canonize (t, t.nodes [s].link, { k, i - 1 });
        tie (r, b) = test_and_split (t, s, { k, i - 1 }, t.text [i]);
    }

    if (oldr != ROOT)
        t.nodes [oldr].link = s;

    return { s, k };
}

////////////////////////////////////////////////////////////////////////

suffix_tree_t
make_suffix_tree (const string& text) {
    suffix_tree_t t { text + "~", vector< node_type > (2U), { } };

    t.nodes [ROOT].link = AUX;
    t.nodes [AUX].link = ROOT;

    size_t s = ROOT;

    for (int k = 0, i = 0; i < int (t.text.size ()); ++i) {
        tie (s, k) = update (t, s, { k, i });
        tie (s, k) = canonize (t, s, { k, i });
    }

    return t;
}
