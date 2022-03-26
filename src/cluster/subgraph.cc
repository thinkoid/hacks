// -*- mode: c++; -*-

#include <cassert>

#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

static vector< int >
make_color_table (const vector< tuple< int, int > >& v) {
    vector< int > u;

    for (const auto& t : v) {
        int a, b;
        tie (a, b) = t;

        const auto c = (max) (a, b);

        if (size_t (c) >= u.size ())
            u.resize (c + 1, -1);

        u [a] = u [b] = 0;
    }

    return u;
}

static inline void
recolor (vector< int >& v, int from, int to) {
    if (from == to)
        return;

    for (auto& c : v)
        if (c == from) c = to;
}

static vector< vector< int > >
cluster (const vector< tuple< int, int > >& v) {
    int counter = 0;

    auto u = make_color_table (v);

    for (auto& p : v) {
        int a, b;
        tie (a, b) = p;

        const unsigned mask = (u [a] ? 2: 0) | (u [b] ? 1 : 0);

        switch (mask) {
        case 0:
            u [a] = u [b] = ++counter;
            break;

        case 1:
            u [a] = u [b];
            break;

        case 2:
            u [b] = u [a];
            break;

        case 3: {
            auto lhs = u [a], rhs = u [b];

            if (lhs != rhs) {
                if (lhs > rhs)
                    swap (lhs, rhs);

                recolor (u, rhs, lhs);
            }
        }
            break;

        default:
            assert (0);
            break;
        }
    }

    for (auto& c : u)
        if (0 > c) c = ++counter;

    map< int, vector< int > > m;

    for (size_t i = 0; i < u.size (); ++i)
        m [u [i]].emplace_back (i);

    vector< vector< int > > w;

    for (auto& p : m)
        w.emplace_back (move (p.second));

    return w;
}

static tuple< int, int, vector< tuple< int, int > > >
read (istream& s) {
    vector< tuple< int, int > > v;

    int a, b;
    s >> a >> b;

    for (int y = 0; y < a; ++y)
        for (int x = 0; x < b; ++x) {
            int c;
            s >> c;

            if (c)
                v.emplace_back (x, y);
        }

    return { a, b, v };
}

static void
write (const vector< vector< int > >& v, ostream& s) {
    for (const auto& u : v) {
        copy (u.begin (), u.end (), ostream_iterator< int > (s, " "));
        s << endl;
    }

    s << endl;
}

static void
write (const vector< tuple< int, int > >& v, int a, int b, ostream& s) {
    vector< int > u (a * b, 0);

    for (const auto& t : v) {
        int x, y;
        tie (x, y) = t;

        u [y * b + x] = 33;
    }

    s << "     ";

    for (int x = 0; x < b; ++x) {
        s << setw (3) << x << ' ';
    }

    s << "\n  " << string ((b + 1) * 4, '-') << "\n";

    for (int y = 0; y < a; ++y) {
        s << setw (3) << y << " |";

        for (int x = 0; x < b; ++x)
            s << setw (3) << u [y * b + x] << ' ';

        s << endl;
    }

    s << endl;
}

int main () {
    vector< tuple< int, int > > src;
    int a, b;

    tie (a, b, src) = read (cin);
    write (src, a, b, cout);

    const auto dst = cluster (src);
    write (dst, cout);

    return 0;
}
