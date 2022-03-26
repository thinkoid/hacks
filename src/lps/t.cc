// -*- mode: c++ -*-

#include <cassert>

#include <functional>
#include <iostream>
#include <vector>

using namespace std;

static pair< size_t, size_t >
lps (const string& s) {
    const auto n = s.size ();
    vector< vector< bool > > table (n, vector< bool > (n, false));

    for (size_t i = 0; i < n; ++i)
        table [i][i] = true;

    size_t start = 0, len = 1;

    for (size_t i = 0; i < n - 1; ++i) {
        if (s [i] == s [i + 1]) {
            table [i][i + 1] = true;
            start = i;
            len = 2;
        }
    }

    for (size_t k = 3; k <= n; ++k) {
        for (size_t i = 0; i < n - k + 1; ++i) {
            size_t j = i + k - 1;

            if (table [i + 1][j - 1] && s [i] == s [j]) {
                table [i][j] = true;

                if (k > len) {
                    start = i;
                    len = k;
                }
            }
        }
    }

    return { start, len };
}

int main (int, char** argv) {
    const string s (argv [1]);

    size_t pos, len;
    tie (pos, len) = lps (s);

    cout << " --> " << pos << ", " << len << " : " << s.substr (pos, len)
         << endl;

    return 0;
}
