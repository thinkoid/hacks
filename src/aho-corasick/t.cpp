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

#include <hacks/aho-corasick.hpp>

#if 0

int main (int, char** argv) {
    const vector< string > arr { "he", "she", "his", "hers" };
    const string text = "uSHers";

    using alphabet_type = english_icase_alphabet_t< char >;
    aho_corasick_t< alphabet_type > a (arr.begin (), arr.end ());

    const auto v = a (text.begin (), text.end ());

    for (const auto& p : v)
        cout << p.first << ":" << p.second << " ";

    cout << endl;

    return 0;
}

#else

int main (int, char** argv) {
    size_t n;
    cin >> n;

    vector< string > g;
    g.reserve (n);

    copy_n (istream_iterator< string > (cin), n, back_inserter (g));

    vector< int > h;
    h.reserve (n);

    copy_n (istream_iterator< size_t > (cin), n, back_inserter (h));

    vector< string > texts;
    string s;

    size_t i, j, k;
    cin >> n;

    for (size_t i = 0; cin && i < n; ++i) {
        cin >> j >> k >> s;
    }

    aho_corasick_t< english_lowercase_alphabet_t< char > > aho_corasick (
        g.begin (), g.end ());

    for (string text; cin >> j >> k >> text;) {
        size_t value = 0;

        aho_corasick (text.begin (), text.end (), [&](auto i) {
                if (j <= i && i <= k)
                    value += h [i];
            });

        if (value > max_)
            max_ = value;

        if (value < min_)
            min_ = value;
    }

    size_t min_ = size_t (-1), max_ = 0;
    cout << min_ << " " << max_ << endl;

    return 0;
}

#endif // 0
