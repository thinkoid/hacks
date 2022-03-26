// -*- mode: c++ -*-

#include <cassert>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>

#include <hacks/knuth-morris-pratt.hpp>

using namespace std;

static bool
is_cyclic_rotation (string s, const string& w) {
    const auto kmp = knuth_morris_pratt< string::const_iterator > (
        w.begin (), w.end ());

    s += s;
    const auto result = kmp (s.begin (), s.end ());

    return result.first != s.end () &&
        std::distance (result.first, result.second) ==
        std::distance (w.begin (), w.end ());
}

static void
test (const string& s, const string& t, bool expected) {
    cout << s << " " << t << " ";

    const auto b = is_cyclic_rotation (s, t);
    cout << boolalpha << b << " " << (b == expected ? "PASS" : "FAIL");

    cout << endl;
}

int main () {
#define T(s, t, b) test (#s, #t, b)

    T (abc, abc,  true);
    T (bca, abc,  true);
    T (cab, abc,  true);

#undef T

    return 0;
}
