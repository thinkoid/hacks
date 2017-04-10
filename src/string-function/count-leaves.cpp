// -*- mode: c++ -*-

#include <cassert>

#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

int main (int, char** argv) {
    cout << count_leaves (make_suffix_tree (argv [1])) << endl;
    return 0;
}
