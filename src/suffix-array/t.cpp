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

inline ostream&
operator<< (ostream& s, const string_view_t& v) {
    return s << string (v.first, v.last);
}

int main () {
    string s;
    cin >> s;

    const auto a = make_suffix_array (make_suffix_tree (s));

    for (size_t i = 0; i < a.array.size (); ++i)
        cout << int (a.lcp [i]) << " : " << a.array [i] << endl;

    return 0;
}
