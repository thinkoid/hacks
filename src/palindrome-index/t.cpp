#include <cassert>

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

template< typename Iterator1, typename Iterator2 >
inline bool is_palindrome (Iterator1 iter1, Iterator1 last1, Iterator2 iter2) {
    for (; iter1 != last1 && *iter1 == *iter2; ++iter1, ++iter2) ;
    return iter1 == last1;
}

static size_t
palindrome_index (const string& s) {
    auto iter = s.begin (), last = iter;
    advance (last, s.size () / 2);

    auto riter = s.rbegin ();

    for (; iter < riter.base (); ++iter, ++riter) {
        if (*iter != *riter) {
            if (is_palindrome (iter + 1, last + 1, riter))
                return distance (s.begin (), iter);
            else
                return distance (s.begin (), riter.base ()) - 1;
        }
    }

    return size_t (-1);
}

int main () {
    int ignore;
    cin >> ignore;

    for (string s; cin >> s; )
        cout << int (palindrome_index (s)) << "\n";

    return 0;
}
