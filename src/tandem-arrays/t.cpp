// -*- mode: c++ -*-

#include <cassert>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

static vector< int >
zbox (const string& s) {
    const std::size_t n = s.size ();

    vector< int > v (n);

    v [0] = 0;
    v [1] = 0;

    int pos = 2, cnd = 0;

    while (size_t (pos) < n) {
        if (s [pos - 1] == s [cnd]) {
            v [pos] = cnd + 1;

            ++pos;
            ++cnd;
        }
        else if (cnd > 0)
            cnd = v [cnd];
        else {
            v [pos] = 0;
            ++pos;
        }
    }

    v [0] = -1;

    return v;
}

static vector< size_t >
do_tandem_arrays (const string& text, const string& pattern) {
    const auto table = zbox (pattern + '$');

    cout << pattern << ", " << text << " : ";
    copy (table.begin (), table.end (), ostream_iterator< int > (cout, " "));
    cout << endl;

    const int lps = table.back ();

    vector< size_t > v;

    for (size_t i = 0, j = 0; i + j < text.size (); ) {
        if (pattern [j] == text [i + j]) {
            if (j == pattern.size () - 1) {
                //
                // Complete match -- record ...
                //
                v.emplace_back (i);

                //
                // ... and restart:
                //
                i = i + j - lps + 1;
                j = table [lps];
            }
            else {
                //
                // Partial match -- advance.
                //
                ++j;
            }
        }
        else {
            if (table [j] < 0) {
                //
                // Cannot backtrack, must advance:
                //
                ++i;
                j = 0;
            }
            else {
                // Advance by the length of the prefix:
                i = i + j - table [j];
                j = table [j];
            }
        }
    }

    return v;
}

namespace std {

template< typename U, typename V >
ostream&
operator<< (ostream& s, const pair< U, V >& p) {
    return s << p.first << " " << p.second;
}

} // namespace std

static void
tandem_arrays (const string& text, const string& pattern) {
    const auto v = do_tandem_arrays (text, pattern);

    vector< pair< size_t, size_t > > u;

    for (size_t i = 0, n = v.size (); i < n; ++i) {
        if (0 == i || pattern.size () != v [i] - v [i - 1])
            u.emplace_back (v [i], 1);
        else
            ++u.back ().second;
    }

    copy (u.begin (), u.end (), ostream_iterator< pair< size_t, size_t > > (cout, "\n"));
    cout << endl;
}

int main (int, char** argv) {
#define T(a, b) tandem_arrays (#a, #b)

    T (xabaabaababaaba, aba);
    T (axxxxb, xx);
    T (xababababababy, ababab);

#undef T

    return 0;
}
