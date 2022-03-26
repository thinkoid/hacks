#include <cassert>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static size_t
anagram_distance (string& s) {
    const auto n = s.size ();

    string lhs = s.substr (0, n/2);
    string rhs = s.substr (n/2, n/2);

    std::sort (lhs.begin (), lhs.end ());
    std::sort (rhs.begin (), rhs.end ());

    vector< int > v1 (26U), v2 (26U);

    for (auto c : lhs) ++v1 [size_t ((unsigned char)c) - 'a'];
    for (auto c : rhs) ++v2 [size_t ((unsigned char)c) - 'a'];

    int sum = 0;

    for (size_t i = 0; i < v1.size (); ++i) {
        if (v1 [i] > v2 [i])
            sum += v1 [i] - v2 [i];
    }
    return sum;
}

int main () {
    int ignore;
    cin >> ignore;

    for (string s; cin >> s; ) {
        if (s.size () % 2)
            cout << "-1" << endl;
        else {
            cout << anagram_distance (s) << endl;
        }
    }

    return 0;
}
