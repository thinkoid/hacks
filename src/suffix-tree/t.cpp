#include <cassert>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

int main (int, char** argv) {
    assert (argv [1] && argv [1][0]);

    const auto t = make_suffix_tree (argv [1]);
    cout << dot_graph_t (t).value () << endl;

    const auto v = count_all_leaves (t);

    copy (v.begin (), v.end (), ostream_iterator< size_t > (cout, " "));
    cout << endl;

    return 0;
}
