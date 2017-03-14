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
    const auto t = make_suffix_tree (argv [1]);
    cout << dot_graph_t (t).value () << endl;

    const auto arr = make_suffix_array (t);

    for (size_t i = 0; i < arr.array.size (); ++i) {
        const string s (arr.array [i].first, arr.array [i].last);

        cout << " --> " << int (arr.lcp [i]) << " : " << s << "\n";
    }

    cout << endl;

    return 0;
}
