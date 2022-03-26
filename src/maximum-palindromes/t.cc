// -*- mode: c++ -*-

#include <cassert>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#define M_ 1000000007L

static const long N_ = 100001;
static long inverses [N_], factorials [N_], factorial_inverses [N_];

static void precalculate () {
    inverses [0] = 0;
    inverses [1] = 1;

    factorials [0] = factorials [1] = 1;

    factorial_inverses [0] = 1;
    factorial_inverses [1] = 1;

    for (int i = 2; i < N_; ++i) {
        inverses [i] = inverses [M_ % i] * (M_ - M_ / i) % M_;
        factorials [i] = factorials [i - 1] * i % M_;
        factorial_inverses [i] = factorial_inverses [i - 1] * inverses [i] % M_;
    }
}

inline long inv (long a, long m = M_) {
    long s = 0, s_ = 1;
    long r = m, r_ = a;

    while (r) {
        long q = r_ / r;

        {
            long prev = r;
            r = r_ - q * r;
            r_ = prev;
        }

        {
            long prev = s;
            s = s_ - q * s;
            s_ = prev;
        }
    }

    if (s_ < 0)
        s_ += m;

    return s_;
}

static inline long add (long a, long b, long m = M_) {
    return { ((a % m) + (b % m)) % m };
}

static inline long sub (long a, long b, long m = M_) {
    return { ((a % m) - (b % m)) % m };
}

static inline long mul (long a, long b, long m = M_) {
    return { ((a % m) * (b % m)) % m };
}

static inline long div_ (long a, long b, long m = M_) {
    return mul (a, inv (b), m);
}

static inline long factorial (long value, long m = M_) {
    long result{ 1 };

    for (long i{ 1 }; i <= value; ++i)
        result = mul (result, i, m);

    return result;
}

template< typename Iterator >
long maximum_palindrome (Iterator iter, Iterator last) {
    int arr [256] = { 0 };

    for (; iter != last; ++iter) {
        unsigned char c = *iter;
        ++arr [c];
    }

    std::sort (arr, arr + sizeof arr / sizeof *arr, std::greater< int > ());

    int count = 0, odds = 0;
    long denominator = 1;

    for (auto x : arr) {
        if (0 == x)
            break;

        count += x/2;

        if (x % 2)
            ++odds;

        denominator = mul (denominator, factorial (x/2));
    }

    if (odds == 0)
        odds = 1;

    return div_ (mul (factorial (count), odds), denominator);
}

static int
maximum_palindrome (const std::string& s, long l, long r) {
    return maximum_palindrome (s.begin () + l - 1, s.begin () + r);
}

int main () {
    std::string s;
    std::cin >> s;

    long l, r, ignore;
    std::cin >> ignore;

    while (std::cin >> l >> r) {
        std::cout << maximum_palindrome (s, l, r) << "\n";
    }

    return 0;
}
