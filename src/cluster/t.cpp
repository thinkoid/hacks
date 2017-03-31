// -*- mode: c++; -*-

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

using coord_t = pair< size_t, size_t >;
using augmented_coord_t = tuple< size_t, size_t, int >;

namespace std {

inline ostream&
operator<< (ostream& s, const coord_t& t) {
    cout << get<0> (t) << "," << get<1> (t);
    return s;
}

inline ostream&
operator<< (ostream& s, const augmented_coord_t& t) {
    cout << get<0> (t) << "," << get<1> (t) << "," << get<2> (t);
    return s;
}

} // namespace std

const auto cmp = [](const auto& lhs, const auto& rhs) {
    return
        get<0> (lhs)  < get<0> (rhs) ||
        get<0> (lhs) == get<0> (rhs) && get<1> (lhs) < get<1> (rhs);
};

static inline bool
adjacent (const augmented_coord_t& lhs, const augmented_coord_t& rhs) {
    return
        get<0> (lhs) == get<0> (rhs) &&
        1 == get<1> (rhs) - get<1> (lhs);
}

static inline void
recolor (vector< augmented_coord_t >& v, int from, int to) {
    for (auto& t : v) {
        auto& c = get<2> (t);

        if (c == from)
            c = to;
    }
}

static void
do_cluster (vector< augmented_coord_t >& v, int& color) {
    auto iter = v.begin (), last = v.end ();

    auto prev = iter;
    advance (iter, 1);

    for (; iter != last; ++prev, ++iter) {
        auto& lhs = *prev;
        auto& rhs = *iter;

        if (adjacent (lhs, rhs)) {
            int &p = get<2> (lhs), &q = get<2> (rhs);

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
                recolor (v, p, q);
                break;

            default:
                break;
            }
        }
    }
}

static void
print (const vector< augmented_coord_t >& v) {
    copy (v.begin (), v.end (),
          ostream_iterator< augmented_coord_t > (cout, "; "));
    cout << endl;
}

static void
print (const vector< coord_t >& v) {
    copy (v.begin (), v.end (),
          ostream_iterator< coord_t > (cout, "; "));
    cout << endl;
}

static void
print (const vector< vector< coord_t > >& v) {
    for (const auto& u : v)
        print (u);
}

static vector< vector< coord_t > >
cluster (vector< augmented_coord_t > v) {
    sort (v.begin (), v.end (), cmp);
    print (v);

    int color = 0;
    do_cluster (v, color);

    for (auto& t : v)
        swap (get<0> (t), get<1> (t));

    sort (v.begin (), v.end (), cmp);

    do_cluster (v, color);

    for (auto& t : v)
        swap (get<0> (t), get<1> (t));

    sort (v.begin (), v.end (), cmp);
    print (v);

    vector< vector< coord_t > > u (color + 1);

    for (const auto& c : v)
        u [get<2> (c)].emplace_back (get<0> (c), get<1> (c));

    return u;
}

static vector< augmented_coord_t >
read_coord (istream& s) {
    using iterator = istream_iterator< int >;

    vector< augmented_coord_t > v;

    for (auto iter = iterator (s), last = iterator (); iter != last; ++iter) {

        size_t a = *iter, b = 0;

        if (++iter != iterator ())
            b = *iter;

        v.emplace_back (a, b, 0);
    }

    return v;
}

int main () {
    const auto v = read_coord (cin);

    const auto u = cluster (v);
    print (u);

    return 0;
}
