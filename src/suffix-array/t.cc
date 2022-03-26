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

int main () {
    string s;
    cin >> s;

    const auto a = make_suffix_array (s);

    for (size_t i = 0; i < a.arr.size (); ++i) {
        cout << setw (4) << int (a.lcp [i]) << " : "
             << s.substr (a.arr [i].pos, a.arr [i].len)
             << endl;
    }

    return 0;
}
