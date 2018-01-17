//
// Created by Terae on 01*5/01/18.
//

#include "catch.hpp"

#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "GraphSearch.h"
#endif

TEST_CASE("search") {
    SECTION("A*") {
        using Graph = graph_undirected<std::string, int, double>;

        Graph g;
        g["node 1"] = 1;
        g["node 2"] = 2;
        g["node 3"] = 3;
        g["node 4"] = 4;
        g("node 1", "node 2") = 12;
        g("node 1", "node 3") = 13;
        g("node 3", "node 2") = 32;
        g("node 2", "node 4") = 24;

        search::AStar<std::string, int, double, UNDIRECTED> a(g);
        search::path<Graph::const_iterator, double> p{a.run(g.find("node 1"), g.find("node 4"), [](const Graph::const_iterator & it, const Graph::const_iterator & target) -> double {
                return std::abs(it->second->get() - target->second->get());
            })
        };

        CHECK(p.total_cost() == 36);
    }
}
