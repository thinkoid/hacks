// -*- mode: c++; -*-

#include <cassert>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

////////////////////////////////////////////////////////////////////////

namespace detail {

static inline void
do_kmr (vector< size_t >& rank, size_t k) {
    const auto n = rank.size ();
    vector< tuple< size_t, size_t, size_t > > NUM (n);

    size_t i = 0;

    for (; i < n - k; ++i)
        NUM [i] = { rank [i], rank [i + k], i };

    for (; i < n; ++i)
        NUM [i] = { rank [i], n, i };

    //
    // Identical rank array values will indicate identical substrings of rank
    // (length) `r'. Two substrings at indices i and j are identical if
    // rank[i] = rank[j] (or, i and j are r-equivalent, when rank[i] = rank[j]).
    //

    //
    // Sorting NUM groups identical pairs {rank[i], rank[i+r]}. Two identical
    // pairs indicate that two pairs of two consecutive substrings are identical
    // (and their constituent substrings in turn, down to size 1).
    //
    sort (NUM.begin (), NUM.end ());

    //
    // The renumber algorithm is an exceedingly clever way to assign a growing
    // sequence of numbers to groups of identical values (pairs of values,
    // indices [0,1] in each NUM element) in NUM.
    //

    //
    // Rebase the counter:
    //
    rank [get<2>(NUM [0])] = 0;

    for (size_t i = 1; i < rank.size (); ++i) {
        const auto &lhs = NUM [i - 1], &rhs = NUM [i];

        //
        // Increment the counter when moving across group boundaries:
        //
        const auto q =
            get<0>(lhs) == get<0>(rhs) &&
            get<1>(lhs) == get<1>(rhs) ? 0 : 1;

        //
        // The counter value propagates unchanged until the next boundary:
        //
        rank [get<2>(rhs)] = rank [get<2>(lhs)] + q;
    }
}

static vector< size_t >
kmr (vector< size_t > rank, size_t r) {
    size_t k = 0;

    //
    // Do KMR up to the floor of log2(r) ...
    //
    for (k = 0; k * 2 <= r; k ? k *= 2 : ++k)
        do_kmr (rank, k);

    //
    // ... and move one step at a time, afterwards:
    //
    do_kmr (rank, r - k);

    return rank;
}

} // namespace detail

static vector< size_t >
kmr (const string& text, size_t r) {
    assert (r < text.size ());

    vector< size_t > rank;
    rank.reserve (text.size ());

    copy (text.begin (), text.end (), back_inserter (rank));

    return detail::kmr (rank, r);
}

int main (int, char** argv) {
    const auto rank = kmr (string (argv [1]), atol (argv [2]));

    copy (rank.begin (), rank.end (), ostream_iterator< size_t > (cout, " "));
    cout << endl;

    return 0;
}
