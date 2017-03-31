#include <cassert>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

int main () {
    size_t n;
    cin >> n;

    vector< string > g;
    vector< int > h;

    copy_n (istream_iterator< string > (cin), n, back_inserter (g));
    copy_n (istream_iterator< size_t > (cin), n, back_inserter (h));

    size_t m, min_ = size_t (-1), max_ = 0;
    cin >> m;

    for (size_t i = 0; i < m; ++i) {
        size_t j, k;
        cin >> j >> k;

        string s;
        cin >> s;

        s += "$";

        const auto t = make_suffix_tree (s);
        const auto v = count_all_leaves (t);

        size_t sum = 0;

        for (++k; j < k; ++j) {
            const auto path = match (t, g [j]);

            if (!path.empty ()) {
                const size_t mult = (0 == path.back ()) ? 1 : v [path.back ()];
                sum += mult * h [j];
            }
        }

        if (sum < min_)
            min_ = sum;

        if (sum > max_)
            max_ = sum;
    }

    cout << min_ << " " << max_ << endl;

    return 0;
}
