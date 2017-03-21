// -*- mode: c++ -*-

#ifndef HACKS_AHO_CORASICK_HPP
#define HACKS_AHO_CORASICK_HPP

#include <hacks/defs.hpp>
#include <hacks/alphabet.hpp>

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
    int_type& transition (int_type state, char_type c) {
        static constexpr auto n = alphabet_type::size ();
        return goto_ [state * n + alphabet_type::ordinal (c)];
    }

    const int_type& transition (int_type state, char_type c) const {
        return const_cast< aho_corasick_t* > (this)->transition (state, c);
    }

    void make_transition (int_type from, char_type c, int_type to) {
        transition (from, c) = to;
    }

    void make_output (int_type state, int_type word_index) {
        output_ [state].emplace (word_index);
    }

    template< typename Iterator >
    void make_goto_function (Iterator first, Iterator last) {
        const size_t len = accumulate (
            first, last, 0U, [](size_t n, const auto& word) {
                return n + word.size ();
            });

        failure_.resize (len, -1);
        goto_.resize (alphabet_type::size () * len, -1);

        int_type states = 1, word_index = 0;

        for (auto iter = first; iter != last; ++iter, ++word_index) {
            const auto& word = *iter;

            int_type state = 0;

            for (const auto c : word) {
                if (transition (state, c) == -1)
                    make_transition (state, c, states++);

                state = transition (state, c);
            }

            make_output (state, word_index);
        }

        for (size_t c = 0; c < alphabet_type::size (); ++c)
            if (goto_ [c] == -1)
                goto_ [c] = 0;
    }

    bool fails (int_type state, char_type c) const {
        return transition (state, c) == -1;
    }

    void make_failure (int_type state, char_type c, int_type to) {
        failure_ [transition (state, c)] = to;
    }

    void make_failure_function () {
        queue< int_type > q;

        for (size_t c = 0; c < alphabet_type::size (); ++c) {
            if (const auto state = goto_ [c]) {
                failure_ [state] = 0;
                q.push (state);
            }
        }

        while (q.size ()) {
            const auto state = q.front ();
            q.pop ();

            for (const auto c : make_char_range (a_.begin (), a_.end ())) {
                if (transition (state, c) != -1) {
                    int failure = failure_ [state];

                    while (fails (failure, c))
                        failure = failure_ [failure];

                    failure = transition (failure, c);
                    make_failure (state, c, failure);

                    {
                        auto iter = output_.find (failure);

                        if (iter != output_.end ()) {
                            const auto& f = iter->second;

                            const auto s = transition (state, c);
                            output_ [s].insert (f.begin (), f.end ());
                        }
                    }

                    q.push (transition (state, c));
                }
            }
        }
    }

public:
    template< typename Iterator >
    aho_corasick_t (
        Iterator first, Iterator last, const alphabet_type& a = alphabet_type ())
        : a_ (a) {
        make_goto_function (first, last);
        make_failure_function ();
    }

    template< typename Iterator, typename Function >
    void operator() (Iterator iter, Iterator last, Function f) const {
        constexpr auto n = alphabet_type::size ();

        size_t state = 0;

        for (typename alphabet_type::off_type off = 0; iter != last;
             ++off, ++iter) {

            const auto c = *iter;

            while (transition (state, c) == -1)
                state = failure_ [state];

            state = transition (state, c);

            auto iter2 = output_.find (state);

            if (iter2 != output_.end ())
                for (const auto& i : iter2->second) f (i, off);
        }
    }

    template< typename Iterator >
    auto operator() (Iterator iter, Iterator last) const {
        constexpr auto n = alphabet_type::size ();

        using match_type = pair<
            typename alphabet_type::pos_type,
            typename alphabet_type::off_type >;

        vector< match_type > v;

        size_t state = 0;

        for (typename alphabet_type::off_type off = 0; iter != last;
             ++off, ++iter) {

            const auto c = *iter;

            while (transition (state, c) == -1)
                state = failure_ [state];

            state = transition (state, c);

            auto iter2 = output_.find (state);

            if (iter2 != output_.end ()) {
                for (const auto& i : iter2->second)
                    v.emplace_back (i, off);
            }
        }

        return v;
    }

private:
    vector< int_type > failure_, goto_;
    map< int_type, set< int_type > > output_;
    alphabet_type a_;
};

#endif // HACKS_AHO_CORASICK_HPP
