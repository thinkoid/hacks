// -*- mode: c++ -*-

#ifndef HACKS_AHO_CORASICK_HH
#define HACKS_AHO_CORASICK_HH

#include <hacks/defs.hh>
#include <hacks/alphabet.hh>

#include <iterator>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <vector>

//
// Alfred V. Aho and Margaret J. Corasick. 1975. Efficient string matching: an
// aid to bibliographic search. Commun. ACM 18, 6 (June 1975),
// 333-340. DOI=http://dx.doi.org/10.1145/360825.360855
//
template< typename T >
struct aho_corasick_t {
    using alphabet_type = T;

    using char_type = typename alphabet_type::char_type;
    using  int_type = typename alphabet_type::int_type;
    using  off_type = typename alphabet_type::off_type;
    using size_type = typename alphabet_type::size_type;

private:
    static constexpr size_type npos = size_type (-1);

private:
    template< typename Iterator >
    void make_transition_function (Iterator first, Iterator last) {
        size_type states = 1, word_index = 0;

        g_.resize (states);

        for (auto iter = first; iter != last; ++iter, ++word_index) {
            const auto& word = *iter;

            size_type state = 0;

            for (const auto c : word) {
                const auto i = alphabet_type::ordinal (c);

                assert (state < g_.size ());
                auto& t = g_ [state];

                auto iter = find_if (
                    t.begin (), t.end (), [&](const auto& arg) {
                        return i == arg.first;
                    });

                if (iter == t.end ()) {
                    t.emplace_back (i, states);

                    ++states;
                    g_.resize (states);

                    iter = --g_ [state].end();
                }

                state = iter->second;
                assert (state < g_.size ());
            }

            o_ [state].emplace (word_index);
        }
    }

    size_type traverse (size_type state, int_type i) const {
        while (true) {
            auto& t = g_ [state];

            auto iter = find_if (
                t.begin (), t.end (), [&](const auto& arg) {
                    return i == arg.first;
                });

            if (iter == t.end ()) {
                if (0 == state)
                    break;

                state = f_ [state];
            }
            else {
                state = iter->second;
                break;
            }
        }

        return state;
    }

    void make_failure_function () {
        f_.resize (g_.size ());

        queue< size_type > q;

        for (const auto& p : g_ [0]) {
            const auto state = p.second;
            q.push (state);
        }

        while (q.size ()) {
            const auto state = q.front ();
            q.pop ();

            for (auto& p : g_ [state]) {
                const auto to = p.second;
                q.push (to);

                const auto i = p.first;
                f_ [to] = traverse (f_ [state], i);

                auto iter = o_.find (f_ [to]);

                if (iter != o_.end ()) {
                    const auto& states = iter->second;
                    o_ [to].insert (states.begin (), states.end ());
                }
            }
        }
    }

public:
    template< typename Iterator >
    aho_corasick_t (
        Iterator first, Iterator last, const alphabet_type& a = alphabet_type ())
        : a_ (a) {
        make_transition_function (first, last);
        make_failure_function ();
    }

    template< typename Iterator, typename Function >
    void operator() (Iterator iter, Iterator last, Function f) const {
        size_t state = 0;

        for (typename alphabet_type::size_type pos = 0; iter != last;
             ++pos, ++iter) {
            const auto c = alphabet_type::ordinal (*iter);
            state = traverse (state, c);

            auto iter2 = o_.find (state);

            if (iter2 != o_.end ())
                for (const auto& i : iter2->second) f (i, pos);
        }
    }

    template< typename Iterator >
    auto operator() (Iterator iter, Iterator last) const {
        size_t state = 0;

        using match_type = pair<
            typename alphabet_type::size_type,
            typename alphabet_type::size_type >;

        vector< match_type > v;

        for (typename alphabet_type::size_type pos = 0; iter != last;
             ++pos, ++iter) {
            const auto c = alphabet_type::ordinal (*iter);
            state = traverse (state, c);

            auto iter2 = o_.find (state);

            if (iter2 != o_.end ()) {
                for (const auto& i : iter2->second)
                    v.emplace_back (i, pos);
            }
        }

        return v;
    }

private:
    vector< vector< pair< char_type, size_type > > > g_;
    vector< size_type > f_;
    map< size_type, set< size_type > > o_;
    alphabet_type a_;
};

template< typename T >
/* static */ constexpr typename aho_corasick_t< T >::size_type
aho_corasick_t< T >::npos /* = size_type (-1) */;

#endif // HACKS_AHO_CORASICK_HH
