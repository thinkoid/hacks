#include <cassert>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

#include <hacks/suffix-tree.hpp>

const vector< string > test_data {
    "A",
    "AA",
    "AAA",
    "AAAAAAA",
    "AAAAAAAAAAAA",
    "ABCABC",
    "ABCABCABCABC",
    "ABCABCABCABCABCABCABCABC",
    "ABCATBC",
    "ABCABCABTCABC",
    "ABCABCABCABCABCABTCABCABC"
};

#include <benchmark/benchmark.h>

static void
BM_tree_construction (benchmark::State& state) {
    const string s = test_data [state.range (0)] + "$";

    while (state.KeepRunning ())
        benchmark::DoNotOptimize (make_suffix_tree (s));
}

BENCHMARK (BM_tree_construction)->DenseRange (0, test_data.size () - 1);

static void
BM_leaf_counting (benchmark::State& state) {
    const string s = test_data [state.range (0)] + "$";
    const auto t = make_suffix_tree (s);

    while (state.KeepRunning ())
        benchmark::DoNotOptimize (count_all_leaves (t));
}

BENCHMARK (BM_leaf_counting)->DenseRange (0, test_data.size () - 1);

static void
BM_substring_matching (benchmark::State& state) {
    const string s = test_data [state.range (0)] + "$";

    const auto t = make_suffix_tree (s);
    const auto v = count_all_leaves (t);

    while (state.KeepRunning ()) {
        benchmark::DoNotOptimize (match (t, "AAA"));
        benchmark::DoNotOptimize (match (t, "ABC"));
        benchmark::DoNotOptimize (match (t, "BTC"));
    }
}

BENCHMARK (BM_substring_matching)->DenseRange (0, test_data.size () - 1);

BENCHMARK_MAIN ();
