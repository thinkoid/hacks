#include <cassert>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static inline bool
is_palindrome (const string& s) {
    return equal (s.begin (), s.begin () + s.size () / 2, s.rbegin ());
}

static string
maximize (string s, size_t k) {
    const size_t n = s.size ();

    vector< int > v (n/2);

    size_t i = 0;

    for (; i < n/2; ++i) {
        auto &c = s [i], &d = s [n - 1 - i];

        if (c != d) {
            if (k) {
                c = d = (max) (c, d);
                --k;

                v [i] = 1;
            }
            else
                break;
        }
    }

    if (i == n/2) {
        for (i = 0; k && i < n/2; ++i) {
            auto &c = s [i], &d = s [n - 1 - i];

            if (v [i]) {
                if (c < '9') {
                    c = d = '9';
                    --k;
                }
            }
            else {
                if (k > 1 && c < '9') {
                    c = d = '9';
                    k -= 2;
                }
            }
        }

        if (k && (n % 2))
            s [n/2] = '9';
    }
    else
        s = "-1";

    return s;
}

int main () {
    size_t ignore, k;
    cin >> ignore >> k;

    string s;
    cin >> s;

    cout << maximize (s, k) << "\n";

    return 0;
}
