// -*- mode: c++ -*-

#include <cassert>

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

int main (int, char** argv) {
    const auto suffix_tree = make_suffix_tree (argv [1]);
    cout << count_distinct_factors (suffix_tree) << endl;

    return 0;
}
