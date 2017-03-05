#include <cassert>
#include <cstring>

#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

int main (int, char** argv) {
    assert (argv [1] && argv [1][0]);

    suffix_tree_t tree = make_suffix_tree (argv [1]);
    cout << dot_graph_t (tree, argv [1]).value () << endl;

    return 0;
}
