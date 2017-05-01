#include <cassert>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
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

#if 0

#include <benchmark/benchmark.h>

static string
make_label (const string& s) {
    stringstream ss;

    if (s.size () > 6) {
        ss << s.substr (0, 6) << "...";
        ss << "(" << s.size () << ")";
    }
    else
        ss << s;

    return ss.str ();
}

static void
BM_tree_construction (benchmark::State& state) {
    const string s = test_data [state.range (0)] + "$";

    state.SetLabel (make_label (s));

    while (state.KeepRunning ())
        benchmark::DoNotOptimize (make_suffix_tree (s));
}

BENCHMARK (BM_tree_construction)->DenseRange (0, test_data.size () - 1);

BENCHMARK_MAIN ();

#else

////////////////////////////////////////////////////////////////////////

struct dot_graph_t {
    explicit dot_graph_t (const suffix_tree_t& t)
        : value_ (make_dot (t))
        { }

    const string& value () const {
        return value_;
    }

private:
    static string
    make_dot (const suffix_tree_t& t) {
        stringstream ss;

        ss << "#+BEGIN_SRC dot :file t.png :cmdline -Kdot -Tpng\n";
        ss << "digraph g {\n";

        for (size_t i = 1; i < t.nodes.size (); ++i)
            ss << "    " << i << ";\n";

        for (size_t i = 1; i < t.nodes.size (); ++i) {
            for (size_t j = 0; j < t.nodes [i].edges.size (); ++j) {
                const auto& e = t.edges [t.nodes [i].edges [j].second];

                ss << "    " << e.s << " -> " << e.s_
                   << " [label=\"" << t.text.substr (e.k, e.p - e.k + 1)
                   << "\"];\n";
            }
        }

        ss << "}\n";
        ss << "#+END_SRC\n\n";

        return ss.str ();
    }

private:
    string value_;
};

int main (int, char** argv) {
    const auto t = make_suffix_tree (argv [1]);
    cout << dot_graph_t (t).value () << endl;
    return 0;
}

#endif
