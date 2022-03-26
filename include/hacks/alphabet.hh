// -*- mode: c++ -*-

#ifndef HACKS_ALPHABET_HH
#define HACKS_ALPHABET_HH

#include <hacks/defs.hh>

using namespace std;

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

namespace alphabet_detail {

//
// Borrowed from Boost irange implementation to facilitate the definition of a
// character type iterator, useful in iterating over characters in an alphabet:
//
template< typename T >
struct char_iterator : public boost::iterator_facade<
    char_iterator< T >, T, boost::bidirectional_traversal_tag, T,
    std::ptrdiff_t > {

public:
    friend class ::boost::iterator_core_access;

public:
    using char_type = T;

    using base_type = boost::iterator_facade<
        char_iterator< T >, T, boost::bidirectional_traversal_tag,
        T, std::ptrdiff_t >;

    using      value_type = typename base_type::value_type;
    using difference_type = typename base_type::difference_type;
    using       reference = typename base_type::reference;

    using iterator_category = std::bidirectional_iterator_tag;

public:
    char_iterator () : value () { }
    explicit char_iterator (value_type value) : value (value) { }

private:
    void increment () { ++value; }
    void decrement () { --value; }

    bool equal (const char_iterator& other) const {
        return value == other.value;
    }

    reference dereference () const {
        return value;
    }

private:
    value_type value;
};

} // namespace detail

template< typename Iterator >
struct char_range : public boost::iterator_range< Iterator > {
    using iterator_type = Iterator;
    using base_type = boost::iterator_range< iterator_type >;

    char_range (iterator_type first, iterator_type last)
        : base_type (first, last)
        { }
};

template< typename Iterator >
auto make_char_range (Iterator first, Iterator last) {
    return char_range< Iterator > (first, last);
}

template< typename T, typename TraitsT, T CharMin, T CharMax, size_t Size >
struct alphabet_base_t {
    using traits_type = TraitsT;

    using   char_type = typename traits_type::char_type;
    using    int_type = typename traits_type::int_type;
    using    off_type = typename traits_type::off_type;

    using size_type = typename make_unsigned< off_type >::type;

    static constexpr inline char_type (min) () { return CharMin; }
    static constexpr inline char_type (max) () { return CharMax; }

    static constexpr inline size_type (size) () { return Size;}

    static constexpr inline bool sparse () {
        return (max) () - (min) () + 1 > size ();
    }
};

#define HACKS_BASE_TYPEDEFS                     \
    using typename base_type::traits_type;      \
    using typename base_type::char_type;        \
    using typename base_type::int_type;         \
    using typename base_type::off_type;         \
    using typename base_type::size_type

template< typename T, typename U = char_traits< T > >
struct english_alphabet_t : alphabet_base_t< T, U, 'A', 'z', 52U > {
    using base_type = alphabet_base_t< T, U, 'A', 'z', 52U >;

    HACKS_BASE_TYPEDEFS;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert ('A' <= i && i <= 'Z' || 'a' <= i && i <= 'z');
        return ('A' <= i && i <= 'Z')
            ? i - 'A' : ('a' <= i && i <= 'z') ? i - 'a' + 'Z' - 'A' + 1 : 0;
    }

    struct iterator : alphabet_detail::char_iterator< char_type > {
    public:
        friend class ::boost::iterator_core_access;

    public:
        using base_type = alphabet_detail::char_iterator< char_type >;

        using      value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;
        using       reference = typename base_type::reference;

        using iterator_category = std::bidirectional_iterator_tag;

    public:
        iterator () : base_type () { }
        explicit iterator (value_type value) : base_type (value) { }

    private:
        void increment () {
            value = value == 'Z' ? 'a' : value + 1;
        }

        void decrement () {
            value = value == 'a' ? 'Z' : value - 1;
        }

        bool equal (const iterator& other) const {
            return value == other.value;
        }

    private:
        value_type value;
    };

    iterator begin () { return iterator ('A'); }
    iterator   end () { return iterator ('z' + 1); }
};

template< typename T, typename U = char_traits< T > >
struct english_icase_alphabet_t : alphabet_base_t< T, U, 'A', 'Z', 26U > {
    using base_type = alphabet_base_t< T, U, 'A', 'Z', 26U >;

    HACKS_BASE_TYPEDEFS;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert ('A' <= i && i <= 'Z' || 'a' <= i && i <= 'z');
        return ('A' <= i && i <= 'Z')
            ? i - 'A' : ('a' <= i && i <= 'z') ? i - 'a' : 0;
    }

    using iterator = alphabet_detail::char_iterator< char_type >;

    iterator begin () const { return iterator ('A');     }
    iterator   end () const { return iterator ('Z' + 1); }
};

template< typename T, typename U = char_traits< T > >
struct english_lowercase_alphabet_t : alphabet_base_t< T, U, 'a', 'z', 26U > {
    using base_type = alphabet_base_t< T, U, 'a', 'z', 26U >;

    HACKS_BASE_TYPEDEFS;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert ('a' <= i && i <= 'z');
        return i - 'a';
    }

    using iterator = alphabet_detail::char_iterator< char_type >;

    iterator begin () const { return iterator ('a');     }
    iterator   end () const { return iterator ('z' + 1); }
};

template< typename T, typename U = char_traits< T > >
struct english_uppercase_alphabet_t : english_icase_alphabet_t< T, U > {
    using base_type = english_icase_alphabet_t< T, U >;

    HACKS_BASE_TYPEDEFS;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert ('A' <= i && i <= 'Z');
        return i - 'A';
    }

    using iterator = alphabet_detail::char_iterator< char_type >;

    iterator begin () const { return iterator ('A');     }
    iterator   end () const { return iterator ('Z' + 1); }
};

template< typename T, typename U = char_traits< T > >
struct printable_ascii_alphabet_t : alphabet_base_t< T, U, ' ', '~', 95U > {
    using base_type = alphabet_base_t< T, U, ' ', '~', 95U >;

    HACKS_BASE_TYPEDEFS;

    static inline off_type ordinal (char_type c) {
        const auto i = traits_type::to_int_type (c);
        assert (' ' <= i && i <= '~');
        return i - ' ';
    }

    using iterator = alphabet_detail::char_iterator< char_type >;

    iterator begin () const { return iterator (' ');     }
    iterator   end () const { return iterator ('~' + 1); }
};

#undef HACKS_BASE_TYPEDEFS

#endif // HACKS_ALPHABET_HH
