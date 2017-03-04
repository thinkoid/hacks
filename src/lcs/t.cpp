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

static string
lcs (const string& a, const string& b) {
    size_t n = a.size (), m = b.size (), i, j;
    string s;

    auto S = vector< vector< int > > (n + 1, vector< int > (m + 1, 0));
    auto R = vector< vector< int > > (n + 1, vector< int > (m + 1, 0));

    for (i = 0; i <= n; ++i) {
        S [i][0] = 0;
        R [i][0] = UP;
    }

    for(j = 0; j <= m; ++j) {
        S [0][j] = 0;
        R [0][j] = LEFT;
    }

    for (i = 1; i <= n; ++i) {
        for (j = 1; j <= m; ++j) {
            if( a[i-1] == b[j-1] ) {
                S[i][j] = S[i-1][j-1] + 1;
                R[i][j] = UP_AND_LEFT;
            }
            else {
                S[i][j] = S[i-1][j-1] + 0;
                R[i][j] = NEITHER;
            }

            if( S[i-1][j] >= S[i][j] ) {
                S[i][j] = S[i-1][j];
                R[i][j] = UP;
            }

            if( S[i][j-1] >= S[i][j] ) {
                S[i][j] = S[i][j-1];
                R[i][j] = LEFT;
            }
        }
    }

    i = n;
    j = m;

    while( i > 0 || j > 0 ) {
        if( R[i][j] == UP_AND_LEFT ) {
            i--;
            j--;

            s += a [i];
        }
        else if( R[i][j] == UP ) {
            i--;
        }
        else if( R[i][j] == LEFT ) {
            j--;
        }
    }

    return reverse (s.begin (), s.end ()), s;
}

int
main (int argc, char** argv) {
    string s, t;
    cin >> s >> t;
    cout << lcs (s, t).size () << endl;
}
