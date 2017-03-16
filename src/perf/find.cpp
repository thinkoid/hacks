#include <cassert>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

// -*- mode: c++ -*-

#include <cstddef>
#include <cassert>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <stack>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

using namespace std;

#include <benchmark/benchmark.h>

static void
custom_args (benchmark::internal::Benchmark* p) {
    for (int i = 16; i <= 128; i += 16) {
        for (int j = 7; j < i; j += 8) {
            p->Args ({i, j});
        }
    }
}

static void
BM_find_linear (benchmark::State& state) {
    vector< int > v (state.range (0));

    size_t i = 0;
    generate_n (v.begin (), v.size (), [&]() { return (i++ % 26) + 'A'; });

    const auto c = v [state.range (1)];

    while (state.KeepRunning ())
        benchmark::DoNotOptimize (find (v.begin (), v.end (), c));
}

BENCHMARK (BM_find_linear)->Apply (custom_args);

static void
BM_find_log (benchmark::State& state) {
    map< int, int > m;

    for (size_t i = 0, n = state.range (0); i < n; ++i)
        m.emplace (i, i);

    const auto c = m [state.range (1)];

    while (state.KeepRunning ())
        benchmark::DoNotOptimize (m.find (c));
}

BENCHMARK (BM_find_log)->Apply (custom_args);

BENCHMARK_MAIN ();
