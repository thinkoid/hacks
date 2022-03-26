#include <cassert>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

//
// Note(!):
// This code is a C++-fication of the original cast iron C code found at:
// http://bix.ucsd.edu/bioalgorithms/downloads/code/LCS.c
//

static constexpr int NEITHER = 0, UP = 1, LEFT = 2, UP_AND_LEFT = 3;

template< typename T >
inline auto size_cast (T value) {
    return size_t (make_unsigned_t< T > (value));
}

static tuple< string, size_t, size_t >
lcs (const string& a, const string& b) {
    size_t n = a.size (), m = b.size (), i, j;
    string s;

    auto S = vector< vector< int > > (n + 1, vector< int > (m + 1, 0));
    auto R = vector< vector< int > > (n + 1, vector< int > (m + 1, 0));

    for (auto& v : S) v [0] = 0;
    for (auto& v : R) v [0] = UP;

    fill (R [0].begin (), R [0].end (), LEFT);

    for (i = 1; i <= n; ++i) {
        for (j = 1; j <= m; ++j) {
            if( a [i-1] == b [j-1] ) {
                S [i][j] = S [i-1][j-1] + 1;
                R [i][j] = UP_AND_LEFT;
            }
            else {
                S [i][j] = S [i-1][j-1] + 0;
                R [i][j] = NEITHER;
            }

            if( S [i-1][j] >= S [i][j] ) {
                S [i][j] = S [i-1][j];
                R [i][j] = UP;
            }

            if( S [i][j-1] >= S [i][j] ) {
                S [i][j] = S [i][j-1];
                R [i][j] = LEFT;
            }
        }
    }

    i = n;
    j = m;

    bool save = false;
    int pos [2] = { 0 };

    while (i > 0 || j > 0) {
        if( R [i][j] == UP_AND_LEFT ) {
            if (!save) {
                save = true;
                pos [0] = i;
                pos [1] = j;
            }

            --i;
            --j;

            s += a [i];
        }
        else if( R[i][j] == UP ) {
            --i;
        }
        else if( R[i][j] == LEFT ) {
            --j;
        }
    }

    reverse (s.begin (), s.end ());

    return { s, pos [0] - s.size (), pos [1] - s.size () };
}

int
main (int, char** argv) {
    string s (argv [1]), t (argv [2]);

    size_t a, b;
    string u;

    tie (u, a, b) = lcs (s, t);
    cout << a << ", " << b << ", " << u << endl;

    return 0;
}
