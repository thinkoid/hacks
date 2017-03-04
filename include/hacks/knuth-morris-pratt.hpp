// -*- mode: c++ -*-

#ifndef HACKS_KMP_HPP
#define HACKS_KMP_HPP

#include <hacks/defs.hpp>

#include <vector>

#include <type_traits>
#include <experimental/type_traits>

template< typename Iterator >
struct knuth_morris_pratt {
    knuth_morris_pratt (Iterator first, Iterator last)
        : first_ (first), last_ (last), table_ (make_table (first_, last_))
        { }

    template< typename TextIterator >
    std::pair< TextIterator, TextIterator >
    operator () (TextIterator first, TextIterator last) const {
        size_t m = 0, i = 0;

        Iterator pattern = first_;
        TextIterator text = first;

        while (std::ptrdiff_t (m + i) < std::distance (first, last)) {
            if (pattern [i] == text [m + i]) {
                if (++i == table_.size ())
                    break;
            }
            else {
                if (table_ [i] > -1) {
                    m += m + i - table_ [i];
                    i = table_ [i];
                }
                else {
                    ++m;
                    i = 0;
                }
            }
        }

        if (i) {
            last = first;

            std::advance (first, m);
            std::advance (last,  m + i);
        }
        else
            first = last;

        return { first, last };
    }

    const std::vector< int >&
    table () const {
        return table_;
    }

private:
    static std::vector< int >
    make_table (Iterator iter, Iterator last) {
        const size_t n = std::distance (iter, last);

        std::vector< int > v (n);

        for (size_t pos = 2, cnd = 0; pos < n; ) {
            if (iter [pos - 1] == iter [cnd])
                v [pos++] = cnd++ + 1;
            else if (cnd > 0)
                cnd = v [cnd];
            else
                v [pos++] = 0;
        }

        v [0] = -1;

        return v;
    }

private:
    Iterator first_, last_;
    std::vector< int > table_;
};

#endif // HACKS_KMP_HPP
