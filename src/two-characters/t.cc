#include <cassert>
#include <cmath>
#include <cstdio>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

using namespace std;

template< typename Iterator >
bool
interleaved (Iterator iter1, Iterator last1,
             Iterator iter2, Iterator last2) {
    assert (0 < distance (iter1, last1));
    assert (0 < distance (iter2, last2));

    if (1 < abs (distance (iter1, last1) - distance (iter2, last2)))
        return false;

    if (*iter1 > *iter2) {
        swap (iter1, iter2);
        swap (last1, last2);
    }

    if (distance (iter1, last1) < distance (iter2, last2))
        return false;

    bool b = true;

    for (; b && iter1 != last1 && iter2 != last2; ) {
        if (b = *iter1 < *iter2) {
            if (++iter1 != last1)
                b = *iter2 < *iter1;
        }

        ++iter2;
    }

    return b;
}

static size_t
two_characters (const string& s) {
    map< int, vector< int > > m;

    for (auto iter = s.begin (); iter != s.end (); ++iter)
        m [*iter].emplace_back (std::distance (s.begin (), iter));

    using iterator = map< int, vector< int > >::iterator;
    vector< iterator > v;

    for (iterator iter = m.begin (); iter != m.end (); ++iter)
        v.emplace_back (iter);

    sort (v.begin (), v.end (), [](const auto& lhs, const auto& rhs) {
            return lhs->second.size () > rhs->second.size ();
        });

    size_t n = 0, nmax = 0;

    // O(N^2) ...
    for (auto iter1 = v.begin (); iter1 != v.end (); ++iter1) {
        for (auto iter2 = v.begin (); iter2 != v.end (); ++iter2) {
            if (interleaved (
                    (*iter1)->second.begin (), (*iter1)->second.end (),
                    (*iter2)->second.begin (), (*iter2)->second.end ())) {
                n = (*iter1)->second.size () + (*iter2)->second.size ();
                if (n > nmax)
                    nmax = n;
            }
        }
    }

    return nmax;
}

int main () {
    int ignore;
    cin >> ignore;

    for (string s; cin >> s; )
        cout << two_characters (s) << "\n";

    return 0;
}
