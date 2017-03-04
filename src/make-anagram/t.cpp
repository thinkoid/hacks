#include <cassert>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static size_t
replace_distance (const string& s1, const string& s2) {
    assert (s1.size () == s2.size ());

    const auto n = s1.size ();
    vector< int > arr (n + 1);

    for (size_t i = 0; i < n + 1; ++i)
        arr [i] = i;

    for (size_t i = 1; i < n + 1; ++i) {
        int prev = arr [0];

        arr [0] = i;

        for (size_t j = 1; j < n + 1; ++j) {
            int t = 0;

            if (s1 [i - 1] == s2 [j - 1])
                t = prev;
            else
                t = prev + 1;

            prev = arr [j];
            arr [j] = t;
        }
    }

    return arr [n];
}

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
