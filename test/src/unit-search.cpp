//
// Created by Terae on 01*5/01/18.
//

#include "catch.hpp"

#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

// TODO
TEST_CASE("search") {
    SECTION("search_path") {
        using Graph = graph_undirected<std::string, int, double>;

        Graph g;
        g["node 1"] = 1;
        g["node 2"] = 2;
        g["node 3"] = 3;
        g["node 4"] = 4;
        g["node 5"] = 5;
        g("node 1", "node 2") = 12;
        g("node 2", "node 3") = 13;
        g("node 3", "node 4") = 32;

        Graph::search_path path{g.dfs("node 1", "node 4")};
        CHECK_FALSE(path.empty());
        CHECK(path.size() == 4);
        CHECK(path.total_cost() == 12 + 13 + 32);
        CHECK(path.contain(g.find("node 2")));
        CHECK_FALSE(path.contain(g.find("node 6")));
        CHECK(path.contain(g.begin()));
        CHECK(path.front() == std::make_pair<Graph::const_iterator, double>(g.find("node 1"), 0));
        path.pop_front();
        for (Graph::search_path::const_reverse_iterator it{path.crbegin()}; it != path.crend(); ++it) {
            CHECK(it->first != g.cend());
        }
    }

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

        Graph::search_path p{g.astar("node 1", "node 4", [&g](const Graph::const_iterator & it) -> double {
                return std::abs(it->second->get() - g["node 4"]);
            })};

        CHECK(p.total_cost() == 36);
    }

    SECTION("dijkstra") {
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

        Graph::dijkstra_path p{g.dijkstra("node 1")};
        CHECK(p[g.cbegin()].second == 0);
        CHECK(p[g.find("node 2")].first.size() == 1);
        CHECK(p[g.find("node 2")].second == 12);
    }
}
