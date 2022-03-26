#include <cassert>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <vector>

using namespace std;

namespace std {

template< typename T, typename U >
inline ostream&
operator<< (ostream& s, const pair< T, U >& arg) {
    return s << arg.first << "," << arg.second;
}

} // namespace std

static vector< int >
manacher (const string& text) {
    const auto n = text.size ();

    //
    // Palindrome length array -- its elements encode the radius of the
    // palindrome in the text, odd-indexed entries encode the radius of odd
    // length palindromes and viceversa.
    //
    vector< int > plen (2*n + 1, 0);

    plen [0] = 0;
    plen [1] = 1;

    //
    // `c' is the "center" of the current palindrome being explored, and `r' is
    // the right boundary.
    //
    int c = 1, r = 2;

    for (int rpos = 2; rpos < int (plen.size ()); ++rpos) {
        //
        // lpos and rpos are mirroring positions around the center.
        //
        int lpos = c - (rpos - c);

        if (plen [lpos] < r - rpos)
            //
            // Palindrome centered at `c' covers the palindrome centered at
            // `lpos' (case 1).
            //
            plen [rpos] = plen [lpos];
        else {
            //
            // Explore the length of the palindrome centered at `rpos': if the
            // palindrome centered at `c' covers `rpos', we can restart at
            // (r - rpos) radius, otherwise we start at radius 0.
            //
            int k = r - rpos;

            if (k < 0)
                k = 0;

            c = rpos;

            {
                int i = rpos - k, j = rpos + k;

                for (; 0 < i && j < int (2*n); --i, ++j) {
                    if (0 == (j & 1) && text [i/2 - 1] != text [j/2])
                        break;
                }

                k = (j - i)/2;
            }

            //
            // Store the radius at right ...
            //
            plen [rpos] = k;

            //
            // ... and adjust the right boundary.
            //
            r = rpos + k;

        }
    }

    return plen;
}

int main (int, char** argv) {
    const string text (argv [1]);

    const auto plen = manacher (text);

    copy (plen.begin (), plen.end (), ostream_iterator< int > (cout, " "));
    cout << "\n  ";

    copy (text.begin (), text.end (), ostream_iterator< char > (cout, "   "));
    cout << endl;

    return 0;
}
