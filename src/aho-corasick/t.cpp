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
