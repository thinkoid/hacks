#include <cassert>

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

template< typename T >
inline auto size_cast (T value) {
    return size_t (make_unsigned_t< T > (value));
}

static inline vector< int >
frequency (const string& s) {
    vector< int > v (26U, 0);

    for (auto c : s)
        ++v [size_cast (c - 'a')];

    return v;
}

static size_t
unordered_annagramatic_pairs_of (const string& s) {
    size_t counter = 0;

    for (size_t n = s.size () - 1; n > 0; --n) {
        vector< string > strings;

        auto iter = s.begin (), last = iter;
        advance (last, s.size () - n + 1);

        for (; iter != last; ++iter)
            strings.emplace_back (iter, iter + n);

        cout << " --> size : " << strings.size () << "\n";

        for (auto& s : strings)
            cout << "  " << s << "\n";

        for (auto first = strings.begin (), last = strings.end ();
             first != last; ++first) {

            auto v = frequency (*first);

            auto iter = first;
            advance (iter, 1);

            for (; iter != last; ++iter) {
                cout << "  [" << *first << " x " << *iter << "] = ";

                auto u = frequency (*iter);

                if (equal (v.begin (), v.end (), u.begin ())) {
                    cout << " v\n";
                    ++counter;
                }
                else {
                    cout << " x\n";
                }
            }
        }
    }

    return counter;
}

int main () {
    size_t ignore;
    cin >> ignore;

    for (string s; cin >> s; )
        cout << unordered_annagramatic_pairs_of (s) << "\n";

    return 0;
}
