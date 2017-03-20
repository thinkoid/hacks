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

template< typename T, typename U = char_traits< T > >
struct english_icase_alphabet_t {
    using traits_type = U;

    using   char_type = typename traits_type::char_type;
    using    int_type = typename traits_type::int_type;
    using    off_type = typename traits_type::off_type;
    using    pos_type = typename traits_type::pos_type;
    using   size_type = typename make_unsigned< off_type >::type;

    static constexpr char_type (min) () { return char_type ('A'); }
    static constexpr char_type (max) () { return char_type ('z'); }

    static constexpr size_type (size) () { return 52U; }

    static off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert (0 <= i && i < 128);

        static const int_type arr [] = {
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
             0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0,
             0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0
        };

        return arr [i];
    }
};

int main (int, char** argv) {
    const vector< string > arr { "he", "she", "hers", "his" };
    const string text = "uSHers";

    using alphabet_type = english_icase_alphabet_t< char >;
    aho_corasick_t< alphabet_type > a (arr.begin (), arr.end ());

    const auto v = a (text.begin (), text.end ());

    for (const auto& p : v)
        cout << p.first << ":" << p.second << " ";
    cout << endl;

    return 0;
}
