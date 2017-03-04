// -*- mode: c++ -*-

#include <cassert>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <string>
#include <vector>

#include <type_traits>
#include <experimental/type_traits>

#include <hacks/knuth-morris-pratt.hpp>

using namespace std;

static inline pair< size_t, size_t >
search (const string& s, const string& t) {
    const auto kmp = knuth_morris_pratt< string::const_iterator > (
        t.begin (), t.end ());

    string::const_iterator iter (s.begin ()), last (s.end ());

    if (s.size () > t.size ())
        std::advance (iter, s.size () - t.size ());

    const auto result = kmp (iter, last);

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

int main (int, char** argv) {
#define T(a, b, pos, len) test (#a, #b, pos, len)

    T (     a,      a,  0,  1);
    T (     b,      a, -1,  0);
    T (     a,      b, -1,  0);

    T (    aa,      a,  1,  1);
    T (   aaa,      a,  2,  1);
    T (  aaaa,      a,  3,  1);

    T (    aa,     aa,  0,  2);
    T (   aaa,     aa,  1,  2);
    T (  aaaa,     aa,  2,  2);

    T (    abc,   abc,  0,  3);
    T (    abc,   bcd,  1,  2);
    T (    abc,   cde,  2,  1);

#undef T

    return 0;
}
