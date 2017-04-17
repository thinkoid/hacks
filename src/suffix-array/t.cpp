#include <cassert>
#include <cstring>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>
#include <hacks/suffix-array.hpp>

inline ostream&
operator<< (ostream& s, const string_view_t& v) {
    return s << string (v.first, v.last);
}

int main () {
    string s;
    cin >> s;

    const auto t = make_suffix_tree (s);
    const auto a = make_suffix_array (t);

    for (size_t i = 0; i < a.array.size (); ++i) {
        cout << setw (4) << int (a.lcp [i]) << " : " << a.array [i] << endl;
    }

    return 0;
}
