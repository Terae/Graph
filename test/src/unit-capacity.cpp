//
// Created by Terae on 01/08/17.
//

#include "catch.hpp"

#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

TEST_CASE("capacity") {
    using namespace std;

    SECTION("empty()") {
        graph<string, int> g;

        CHECK(g.empty());

        g["node"] = 42;
        CHECK(!g.empty());

        g.clear();
        CHECK(g.empty());
    }

    SECTION("size()") {
        graph<string, int> g;

        CHECK(g.size() == 0);

        g["node 1"] = 1;
        g["node 2"] = 2;
        CHECK(g.size() == 2);

        for (int i{0}; i < 10; ++i) {
            g["node " + to_string(i)] = i;
        }
        CHECK(g.size() == 10);

        g.del_node("node 9");
        CHECK(g.size() == 9);
    }

    SECTION("max_size()") {
        graph<string, int> g;

        CHECK(g.max_size() >= g.size());
    }
}
