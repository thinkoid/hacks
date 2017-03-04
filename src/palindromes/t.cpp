#include <iostream>
#include <string>

using namespace std;

static size_t
make_palindrome (const string& s) {
    size_t n = 0;

    auto iter = s.begin ();
    auto iter2 = s.rbegin ();

    for (; iter < iter2.base (); ++iter, ++iter2) {
        if (*iter != *iter2)
            n += abs (*iter - *iter2);
    }

    return n;
}

int main () {
    int ignore;
    cin >> ignore;

    for (string s; cin >> s; )
        cout << make_palindrome (s) << "\n";

    return 0;
}
