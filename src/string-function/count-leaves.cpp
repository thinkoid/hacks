// -*- mode: c++ -*-

#include <cassert>

#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

using state_type = map< size_t, size_t >;

int main (int, char** argv) {
    const auto suffix_tree = make_suffix_tree (argv [1]);
    cout << count_leaves (suffix_tree) << endl;
    return 0;
}
