// -*- mode: c++ -*-

#ifndef HACKS_ALPHABET_HPP
#define HACKS_ALPHABET_HPP

#include <hacks/defs.hpp>

using namespace std;

template< typename T, typename TraitsT, T CharMin, T CharMax, size_t Size >
struct alphabet_base_t {
    using traits_type = TraitsT;

    using   char_type = typename traits_type::char_type;
    using    int_type = typename traits_type::int_type;
    using    off_type = typename traits_type::off_type;
    using    pos_type = typename traits_type::pos_type;

    using size_type = typename make_unsigned< off_type >::type;

    static constexpr inline char_type (min) () { return CharMin; }
    static constexpr inline char_type (max) () { return CharMax; }

    static constexpr inline size_type (size) () { return Size;}

    static constexpr inline bool sparse () {
        return (max) () - (min) () + 1 > size ();
    }
};


template< typename T, typename U = char_traits< T > >
struct english_alphabet_t : alphabet_base_t< T, U, 'A', 'z', 52U > {
    using base_type = alphabet_base_t< T, U, 'A', 'z', 52U >;

    using typename base_type::traits_type;

    using typename base_type::char_type;
    using typename base_type::int_type;
    using typename base_type::off_type;
    using typename base_type::pos_type;

    using typename base_type::size_type;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert ('A' <= i && i <= 'Z' || 'a' <= i && i <= 'z');
        return ('A' <= i && i <= 'Z')
            ? i - 'A' : ('a' <= i && i <= 'z') ? i - 'a' + 'Z' - 'A' + 1 : 0;
    }
};

template< typename T, typename U = char_traits< T > >
struct english_icase_alphabet_t : alphabet_base_t< T, U, 'A', 'z', 52U > {
    using base_type = alphabet_base_t< T, U, 'A', 'z', 52U >;

    using typename base_type::traits_type;

    using typename base_type::char_type;
    using typename base_type::int_type;
    using typename base_type::off_type;
    using typename base_type::pos_type;

    using typename base_type::size_type;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert ('A' <= i && i <= 'Z' || 'a' <= i && i <= 'z');
        return ('A' <= i && i <= 'Z')
            ? i - 'A' : ('a' <= i && i <= 'z') ? i - 'a' : 0;
    }
};

template< typename T, typename U = char_traits< T > >
struct english_lowercase_alphabet_t : alphabet_base_t< T, U, 'a', 'z', 26U > {
    using base_type = alphabet_base_t< T, U, 'A', 'z', 52U >;

    using typename base_type::traits_type;

    using typename base_type::char_type;
    using typename base_type::int_type;
    using typename base_type::off_type;
    using typename base_type::pos_type;

    using typename base_type::size_type;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert ('a' <= i && i <= 'z');
        return i - 'a';
    }
};

template< typename T, typename U = char_traits< T > >
struct english_uppercase_alphabet_t : alphabet_base_t< T, U, 'A', 'Z', 26U > {
    using base_type = alphabet_base_t< T, U, 'A', 'Z', 26U >;

    using typename base_type::traits_type;

    using typename base_type::char_type;
    using typename base_type::int_type;
    using typename base_type::off_type;
    using typename base_type::pos_type;

    using typename base_type::size_type;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert ('A' <= i && i <= 'Z');
        return i - 'A';
    }
};

template< typename T, typename U = char_traits< T > >
struct printable_ascii_alphabet_t : alphabet_base_t< T, U, ' ', '~', 95U > {
    using base_type = alphabet_base_t< T, U, ' ', '~', 95U >;

    using typename base_type::traits_type;

    using typename base_type::char_type;
    using typename base_type::int_type;
    using typename base_type::off_type;
    using typename base_type::pos_type;

    using typename base_type::size_type;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert (' ' <= i && i <= '~');
        return i - ' ';
    }
};

#endif // HACKS_ALPHABET_HPP
