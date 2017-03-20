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
    template< typename Iterator >
    void make_goto_function (Iterator first, Iterator last) {
        const size_t len = accumulate (
            first, last, 0U, [](size_t n, const auto& word) {
                return n + word.size ();
            });

        failure_.resize (len, -1);
        goto_.resize (alphabet_type::size () * len, -1);

        //
        // Compute the goto function (a word trie) and part of the output
        // function:
        //
        int_type states = 1, word_index = 0;

        for (auto iter = first; iter != last; ++iter) {
            const auto& word = *iter;

            int_type state = 0;

            for (const auto c : word) {
                const auto index =
                    alphabet_type::size () * state +
                    alphabet_type::ordinal (c);

                if (goto_ [index] == -1)
                    goto_ [index] = states++;

                state = goto_ [index];
            }

            output_ [state].emplace (word_index++);
        }

        for (size_t c = 0; c < alphabet_type::size (); ++c)
            if (goto_ [c] == -1)
                goto_ [c] = 0;
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

            for (size_t c = 0; c < alphabet_type::size (); ++c) {
                const auto state_index = state * alphabet_type::size () + c;

                if (goto_ [state_index] != -1) {
                    //
                    // If g(r,a) = fail ... :
                    //
                    int failure = failure_ [state];

                    while (goto_ [failure * alphabet_type::size () + c] == -1)
                        //
                        // Walk the uninterrupted chain of failure states, until
                        // g(state,a) != failure:
                        //
                        failure = failure_ [failure];

                    //
                    // Set f(s) = g(state,a):
                    //
                    failure = goto_ [failure * alphabet_type::size () + c];
                    failure_ [goto_ [state_index]] = failure;

                    //
                    // When f(s) = s', merge outputs of s and s':
                    //
                    {
                        auto iter = output_.find (failure);

                        if (iter != output_.end ()) {
                            const auto& f = iter->second;

                            const auto s = goto_ [state_index];
                            output_ [s].insert (f.begin (), f.end ());
                        }
                    }

                    //
                    // Queue the state for processing in level d+1:
                    //
                    q.push (goto_ [state_index]);
                }
            }
        }
    }

public:
    template< typename Iterator >
    aho_corasick_t (Iterator first, Iterator last) {
        make_goto_function (first, last);
        make_failure_function ();
    }

    template< typename Iterator, typename Function >
    void operator() (Iterator iter, Iterator last, Function f) const {
        constexpr auto n = alphabet_type::size ();

        size_t state = 0;

        for (typename alphabet_type::off_type off = 0; iter != last;
             ++off, ++iter) {

            const auto c = alphabet_type::ordinal (*iter);

            while (goto_ [state * n + c] == -1)
                state = failure_ [state];

            state = goto_ [state * n + c];

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

            const auto c = alphabet_type::ordinal (*iter);

            while (goto_ [state * n + c] == -1)
                state = failure_ [state];

            state = goto_ [state * n + c];

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
};

#endif // HACKS_AHO_CORASICK_HPP
