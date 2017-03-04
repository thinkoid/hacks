#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

static string
lexicographical_merge (string s, string t) {
    string u;

    s += '[';
    t += '[';

    size_t i = 0, j = 0;

    for (; i < s.size () && j < t.size ();) {
        if (0 > s.compare (i, string::npos, t, j, string::npos))
            u += s [i++];
        else
            u += t [j++];
    }

    if (i < s.size ())
        u += s.substr (i);

    if (j < t.size ())
        u += t.substr (j);

    return u.substr (0, u.size () - 2);
}

static void
test (const string& a, const string& b, const string& c) {
    cout << "Running : " << a << " x " << b << " = " << c;

    auto s = lexicographical_merge (a, b);

    if (s == c)
        cout << "\n";
    else
        cerr << " (" << s << ")\n";
}

int main () {
#define T(a, b, c) test (#a, #b, #c)

    T (MA, MM, MAMM);
    T (MM, MA, MAMM);

    T (MQ, MM, MMMQ);
    T (MM, MQ, MMMQ);

    T (MA, MB, MAMB);
    T (MB, MA, MAMB);

    T (MA, MQ, MAMQ);
    T (MQ, MA, MAMQ);

    T (MQ, MR, MMQR);
    T (MR, MQ, MMQR);

    T (MA, MA, MAMA);
    T (MQ, MQ, MMQQ);

    T (  MMA, MM, MMAMM);
    T ( MMMA, MM, MMMAMM);
    T (MMMMA, MM, MMMMAMM);

    T (  MMQ, MM, MMMMQ);
    T ( MMMQ, MM, MMMMMQ);
    T (MMMMQ, MM, MMMMMMQ);

    T (MM, MMA, MMAMM);

    T (MMQ, MM, MMMMQ);
    T (MM, MMQ, MMMMQ);

    T (ABAD, BBDE, ABABBDDE);

    T (ABA, ABAA, AABAABA);

    T (BAB, BAC, BABABC);
    T (BAA, BA, BAABA);

    T (DAD, DAD, DADADD);
    T (ABCBA, BCBA, ABBCBACBA);
    T (BAC, BAB, BABABC);
    T (DAD, DABC, DABCDAD);
    T (YZYYZYZYY, ZYYZYZYY, YZYYZYYZYZYYZYZYY);
    T (ZZYYZZZA, ZZYYZZZB, ZZYYZZYYZZZAZZZB);

    return 0;
}
