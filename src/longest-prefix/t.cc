// -*- mode: c++ -*-

#include <cassert>

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <hacks/knuth-morris-pratt.hpp>

using namespace std;

static inline size_t
longest_prefix (const string& s) {
    const auto t (s + '$');

    const auto kmp = knuth_morris_pratt< string::const_iterator > (
        t.begin (), t.end ());

    return kmp.table ().back ();
}

static void
test (const string& s, size_t n) {
    const auto result = longest_prefix (s);
    cout << s << " " << result << " " << n << " ";

    if (result == n)
        cout << "PASS";
    else
        cout << "FAIL";

    cout << endl;
}

int main () {
#define T(s, n) test (#s, n)

    T (   acax, 0);
    T (    aca, 1);
    T (  abcab, 2);
    T ( abcabc, 3);
    T (abcdabc, 3);

#undef T

    return 0;
}
