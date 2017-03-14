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
#include <hacks/suffix-array.hpp>

int main (int, char** argv) {
    const auto suffix_tree = make_suffix_tree (argv [1]);
    cout << dot_graph_t (suffix_tree, argv [1]).value () << endl;

    const auto suffix_array = make_suffix_array (suffix_tree);

    for (size_t i = 0; i < suffix_array.array.size (); ++i) {
        const string s (suffix_array.array [i].first,
                        suffix_array.array [i].last);

        cout << " --> " << int (suffix_array.lcp [i]) << " : " << s
             << "\n";
    }

    return 0;
}
