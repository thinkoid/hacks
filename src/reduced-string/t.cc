// -*- mode: c++ -*-

#include <cassert>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

static string
reduced_string (const string& s) {
    assert (!s.empty ());

    using const_iterator = string::const_iterator;

    string t;

    for (const_iterator rhs = s.begin (); rhs != s.end (); ) {
        if (t.empty ())
            t += *rhs++;
        else {
            auto lhs = t.rbegin ();

            while (lhs != t.rend () && rhs != s.end () && *lhs == *rhs)
                ++lhs, ++rhs;

            if (lhs != t.rbegin ())
                t.erase (lhs.base (), t.end ());

            if (rhs != s.end ())
                t += *rhs++;
        }
    }

    return t;
}

int main () {
    string s;

    for (string s; cin >> s; )
        cout << s << " : " << reduced_string (s) << "\n";

    return 0;
}
