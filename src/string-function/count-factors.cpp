// -*- mode: c++ -*-

#include <cassert>

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

int main (int, char** argv) {
    const auto t = make_suffix_tree (argv [1]);

    const auto f = distinct_factors (t);
    assert (!f.empty ());

    for (const auto& p : f) {
        size_t pos, len;
        tie (pos, len) = p;

        cout << " --> " << t.text.substr (pos, len) << endl;
    }

    return 0;
}
