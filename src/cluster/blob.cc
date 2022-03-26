// -*- mode: c++; -*-

#include <cassert>

#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

template< size_t N >
struct indexed_less {
    template< typename T >
    bool operator() (const T& lhs, const T& rhs) const {
        return
            get< 0 > (lhs)  < get< 0 > (rhs) ||
            get< 0 > (lhs) == get< 0 > (rhs) && get< 1 > (lhs) < get< 1 > (rhs);
    }
};

using less_0 = indexed_less< 0 >;
using less_1 = indexed_less< 0 >;

static inline bool
adjacent (
    const tuple< int, int, int >& lhs,
    const tuple< int, int, int >& rhs) {
    return get< 0 > (lhs) == get< 0 > (rhs) &&
        1 == abs (get< 1 > (rhs) - get< 1 > (lhs));
}

static inline void
recolor (vector< tuple< int, int, int > >& v, int from, int to) {
    for (auto& t : v) {
        auto& color = get< 2 > (t);

        if (color == from)
            color = to;
    }
}

static void
do_color (vector< tuple< int, int, int > >& v, int& color) {
    assert (v.size () > 1);
    
    auto iter = v.begin (), prev = iter, last = v.end ();
    advance (iter, 1);

    for (; iter != last; ++prev, ++iter) {
        auto& lhs = *prev;
        auto& rhs = *iter;

        if (adjacent (lhs, rhs)) {
            int &p = get< 2 > (lhs), &q = get< 2 > (rhs);

            unsigned mask = (p ? 2 : 0) | (q ? 1 : 0);

            switch (mask) {
            case 0:
                p = q = ++color;
                break;

            case 1:
                p = q;
                break;

            case 2:
                q = p;
                break;

            case 3:
                if (p != q)
                    recolor (v, (max) (p, q), (min) (p, q));
                break;

            default:
                assert (0);
                break;
            }
        }
    }
}

static vector< tuple< int, int, int > >
color (vector< tuple< int, int, int > > v) {
    int color = 0;
    
    sort (v.begin (), v.end (), less_0 { });
    do_color (v, color);

    for (auto& t : v)
        swap (get< 0 > (t), get< 1 > (t));

    sort (v.begin (), v.end (), less_0 { });
    do_color (v, color);

    for (auto& t : v)
        swap (get< 0 > (t), get< 1 > (t));

    sort (v.begin (), v.end (), less_0 { });

    return v;
}

static vector< tuple< int, int, int > >
color (const vector< tuple< int, int > >& v, int c) {
    vector< tuple< int, int, int > > u;

    for (const auto& t : v)
        u.emplace_back (get< 0 > (t), get< 1 > (t), c);

    return u;
}

static tuple< size_t, size_t, vector< tuple< int, int > > >
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
write (const vector< tuple< int, int, int > >& v, int a, int b, ostream& s) {
    vector< int > u (a * b, 0);

    for (const auto& t : v) {
        int x, y, c;
        tie (x, y, c) = t;

        u [y * b + x] = c;
    }

    for (int y = 0; y < a; ++y) {
        for (int x = 0; x < b; ++x)
            s << setw (3) << u [y * b + x] << ' ';

        s << endl;
    }

    s << endl;
}

static void
do_test (const char* s) {
    vector< tuple< int, int > > v;
    int a, b;

    ifstream f (s);
    f.exceptions (ios_base::failbit);

    tie (a, b, v) = read (f);

    const auto src = color (v, 0);

    {
        const auto tmp = color (v, 33);
        write (tmp, a, b, cout);
    }

    const auto dst = color (src);
    write (dst, a, b, cout);
}

static void
test (const char* s) {
    cout << " --> " << s << endl;
    do_test (s);
}

int main () {
    test ("./t1.txt");
    test ("./t2.txt");
    return 0;
}
