// -*- mode: c++ -*-

#include <cassert>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

const long M = 1000000007L;
const long N = 100000L;

static std::vector< long > inverses(N), factorials(N), factorial_inverses(N);
static std::vector< int > sieve;

static void precalculate_factorials()
{
    inverses[0] = 0;
    inverses[1] = 1;

    factorials[0] = factorials[1] = 1;

    factorial_inverses[0] = 1;
    factorial_inverses[1] = 1;

    for(int i = 2; i < N; ++i) {
        inverses[i] = inverses[M % i] *(M - M / i) % M;
        factorials[i] = factorials[i - 1] * i % M;
        factorial_inverses[i] = factorial_inverses[i - 1] * inverses[i] % M;
    }
}

static void precalculate_sieve(const std::string &s)
{
    sieve.resize((s.size() + 1) * 32U);
    
    const int *prev = sieve.data();
    int *ptr = sieve.data() + 32U;

    for (int c : s) {
        std::copy(prev, prev + 32U, ptr);

        ++ptr[c - 'a'];

        prev += 32U;
        ptr  += 32U;
    }
}

int main()
{
    std::string s;
    std::cin >> s;

    long l, r, ignore;
    std::cin >> ignore;

    precalculate_factorials();
    precalculate_sieve(s);

    std::vector< int > xs(32U);

    while(std::cin >> l >> r) {
        {
            const int *beg = sieve.data() + (l -1) * 32U;
            const int *end = sieve.data() + r * 32U;

            for (size_t i = 0; i < 32U; ++i) {
                xs[i] = end[i] - beg[i];
            }
        }

        int count = 0, odds = 0;

        for(auto x : xs) {
            count += x / 2;
            odds  += x & 1;
        }

        if(odds == 0)
            odds = 1;

        long numerator = ((factorials[count] % M) * (odds % M)) % M;
    
        for(auto x : xs) {
            if(x)
                numerator = ((numerator % M) * (factorial_inverses[x/2] % M)) % M;
        }

        std::cout << numerator << "\n";
    }

    return 0;
}
