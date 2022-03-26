// -*- mode: c++ -*-

#include <cassert>

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <hacks/knuth-morris-pratt.hh>

using namespace std;

static inline pair< size_t, size_t >
search (const string& s, const string& t) {
    const auto kmp = knuth_morris_pratt< string::const_iterator > (
        t.begin (), t.end ());

    const auto result = kmp (s.begin (), s.end ());

    size_t pos = size_t (-1), len = 0;

    if (result.first != s.end ()) {
        pos = std::distance (s.begin (), result.first);
        len = std::distance (result.first, result.second);
    }

    return { pos, len };
}

static void
test (const string& s, const string& t, size_t pos, size_t len) {
    const auto result = search (s, t);

    cout << s << " x " << t << " : "
         << result.first << ", " << result.second << " : ";

    if (result == decltype (result)(pos, len))
        cout << "PASS";
    else
        cout << "FAIL";

    cout << endl;
}

int main () {
#define T(s, t, pos, len) test (#s, #t, pos, len)

    T (   ab,   a,  0,  1);
    T (  abc,  ab,  0,  2);
    T ( abcd, abc,  0,  3);
    T ( abcd, cde,  2,  2);
    T (    a, abc,  0,  1);

#undef T

    return 0;
}
