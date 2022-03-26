#include <cassert>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

template< typename T >
inline auto size_cast (T value) {
    return size_t (make_unsigned_t< T > (value));
}

static vector< size_t > ordinals {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static vector< size_t >
frequencies_in (const string& s) {
    vector< size_t > v (4U, 0);

    for (auto c : s) {
        assert ((unsigned char)c < 128);
        ++v [ordinals [size_cast (c)]];
    }

    return v;
}

static inline int
base_from_ordinal (size_t n) {
    assert (n < 4);
    static int arr [] = { 'A', 'C', 'G', 'T' };
    return arr [n];
}

static inline size_t
base_ordinal (char c) {
    assert ((unsigned char)c < 128);
    return ordinals [size_cast (c)];
}

static string
calculate_complement (const string& s) {
    string anagram, complement;

    const auto freq = frequencies_in (s);

    copy (freq.begin (), freq.end (), ostream_iterator< size_t > (cout, " "));
    cout << endl;

    const auto n = s.size ();

    for (size_t i = 0; i < freq.size (); ++i) {
        if (freq [i] < n/4)
            anagram.append (string (n/4 - freq [i], base_from_ordinal (i)));
        else if (freq [i] > n/4)
            complement.append (string (freq [i] - n/4, base_from_ordinal (i)));
    }

    cout << anagram << ", " << complement << "\n";

    return complement;
}

static inline bool
operator< (const vector< size_t >& lhs, const vector< size_t >& rhs) {
    assert (lhs.size () == rhs.size ());
    return equal (lhs.begin (), lhs.end (), rhs.begin (), less< int > ());
}

static inline bool
operator>= (const vector< size_t >& lhs, const vector< size_t >& rhs) {
    assert (lhs.size () == rhs.size ());
    return equal (lhs.begin (), lhs.end (), rhs.begin (), greater_equal< int > ());
}

static inline bool
operator> (const vector< size_t >& lhs, const vector< size_t >& rhs) {
    assert (lhs.size () == rhs.size ());
    return equal (lhs.begin (), lhs.end (), rhs.begin (), greater< int > ());
}

static inline bool
operator<= (const vector< size_t >& lhs, const vector< size_t >& rhs) {
    return equal (lhs.begin (), lhs.end (), rhs.begin (), less_equal< int > ());
}

static size_t
steady_patch_size (const string& s) {
    const string complement = calculate_complement (s);

    if (0 == complement.size ())
        return 0;

    auto f = frequencies_in (complement);

    decltype (f) g (4U);
    ++g [base_ordinal (s [0])];

    size_t pos = 0, end = 1, len = -1;

    //
    // Sliding window and computation of frequencies:
    //
    for (; s.size () - pos >= complement.size ();) {
        //
        // Grow window:
        //
        for (; end < s.size () && !(g >= f); ++end)
            ++g [base_ordinal (s [end])];

        if (g >= f) {
            //
            // Shrink window:
            //
            for (; end - pos >= complement.size () && g >= f; ++pos) {
                const auto n = base_ordinal (s [pos]);

                if (g [n])
                    --g [n];
            }

            if (len > end - pos + 1)
                len = end - pos + 1;

            cout << " --> " << s.substr (pos - 1, end - pos + 1) << " : "
                 << pos - 1 << ", " << end - pos + 1 << "\n";
        }
        else
            break;
    }

    return len;
}

int
main (int argc, char** argv) {
#if 0
    int ignore;
    cin >> ignore;

    string s;
    cin >> s;

    cout << steady_patch_size (s) << endl;
#else
    cout << steady_patch_size (argv [1]) << endl;
#endif // 0

    return 0;
}
