//
// Created by Terae on 01*5/01/18.
//

#include "catch.hpp"

#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

using namespace std;

// TODO
TEST_CASE("search") {
    SECTION("search_path") {
        using Graph = graph_undirected<string, int, double>;

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
        CHECK      (path.size() == 4);
        CHECK      (path.total_cost() == 12 + 13 + 32);
        CHECK      (path.contain(g.find("node 2")));
        CHECK_FALSE(path.contain(g.find("node 6")));
        CHECK      (path.contain(g.begin()));
        CHECK      (path.front() == make_pair<Graph::const_iterator, double>(g.find("node 1"), 0));
        path.pop_front();
        for (auto it{path.crbegin()}; it != path.crend(); ++it) {
            CHECK(it->first != g.cend());
        }
    }

    SECTION("A*") {
        using Graph = graph_undirected<string, int, double>;

        Graph g;
        g["node 1"] = 1;
        g["node 2"] = 2;
        g["node 3"] = 3;
        g["node 4"] = 4;
        g("unreachable 1", "unreachable 2") = -5;
        g("node 1", "node 2") = 12;
        g("node 1", "node 3") = 13;
        g("node 3", "node 2") = 32;
        g("node 2", "node 4") = 24;

        function<double(Graph::const_iterator)> heuristic = [&g](const Graph::const_iterator & it) -> double {
            return abs(it->second->get() - g["node 4"]);
        };

        // start == cend()
        CHECK_THROWS_WITH(g.astar(g.cend(), g.find("node 1"), heuristic), "[graph.exception.invalid_argument] Start point equals to graph::cend() when calling 'astar'.");

        // none path found
        CHECK_NOTHROW(g.astar("node 1", "unreachable 1", heuristic));
        CHECK(g.astar("node 1", "unreachable 1", heuristic).empty());

        // start == target
        CHECK(g.astar("node 1", "node 1", heuristic).size() == 1);
        CHECK(g.astar("node 1", "node 1", heuristic).total_cost() == 0);

        // A* search
        Graph::search_path p{g.astar("node 1", "node 4", heuristic)};
        CHECK(p.total_cost() == 36);
        CHECK(p.size() == 3);
        stringstream ss;
        ss << p;
        CHECK(ss.str() == "-> node 1 (0) -> node 2 (12) -> node 4 (36) ");

        // negative weight
        g("node 1", "node 4") = -5;
        CHECK_THROWS_WITH(g.astar("node 1", "node 4", heuristic), "[graph.exception.bad_graph.negative_edge] Edge with negative weight when calling 'astar'.");
    }

    SECTION("dijkstra") {
        using Graph = graph_undirected<string, int, double>;

        Graph g;
        g["node 1"] = 1;
        g["node 2"] = 2;
        g["node 3"] = 3;
        g["node 4"] = 4;
        g("unreachable 1", "unreachable 2") = -5;
        g("node 1", "node 2") = 12;
        g("node 1", "node 3") = 13;
        g("node 3", "node 2") = 32;
        g("node 2", "node 4") = 24;

        // start == cend()
        CHECK_THROWS_WITH(g.dijkstra(g.cend()), "[graph.exception.invalid_argument] Start point equals to graph::cend() when calling 'dijkstra'.");

        // none path found
        CHECK_NOTHROW(g.dijkstra("node 1", "unreachable 1"));
        Graph::shortest_paths p1{g.dijkstra("node 1", "unreachable 1")};
        CHECK(p1.get_path("unreachable 1").empty());
        CHECK(p1.get_path("node 4").total_cost() == 36);

        // Dijkstra search
        Graph::shortest_paths p2{g.dijkstra("node 1")};
        CHECK(p2.get_path(g.cbegin()).total_cost() == 0);
        CHECK(p2.get_path("node 2").size() == 2);
        CHECK(p2.get_path("node 2").total_cost() == 12);

        // negative weight
        g("node 1", "new node") = -5;
        CHECK_THROWS_WITH(g.dijkstra("node 1"), "[graph.exception.bad_graph.negative_edge] Edge with negative weight when calling 'dijkstra'.");
    }

    SECTION("Bellman-Ford") {
        using Graph = graph_directed<string, int, double>;

        Graph g;
        for (int i{1}; i <= 6; ++i) {
            g["node " + to_string(i)] = i;
        }
        g["unreachable"] = 7;
        g("node 1", "node 2") = 4;
        g("node 1", "node 3") = 6;
        g("node 3", "node 2") = 9;
        g("node 2", "node 4") = 3;
        g("node 3", "node 5") = 4;
        g("node 4", "node 3") = -4;
        g("node 4", "node 5") = 1;
        g("node 4", "node 6") = 4;
        g("node 5", "node 6") = 2;

        // start == cend()
        CHECK_THROWS_WITH(g.bellman_ford(g.cend()), "[graph.exception.invalid_argument] Start point equals to graph::cend() when calling 'bellman_ford'.");

        // none path found
        Graph::shortest_paths p1{g.bellman_ford("node 1")};
        CHECK(p1.get_path("unreachable").empty());

        // Bellman-Ford search
        Graph::shortest_paths p{g.bellman_ford("node 1")};
        CHECK(p.get_path("node 3").total_cost() == 3);
        CHECK(p.get_path("node 4").total_cost() == 7);
        CHECK(p.get_path("node 5").total_cost() == 7);
        CHECK(p.get_path("node 5").size() == 5);

        // negative-weight cycle
        g("node 5", "node 4") = -2;
        CHECK_THROWS_WITH(g.bellman_ford("node 1"), "[graph.exception.bad_graph.negative_weight_cycle] Negative-weight cycle when calling 'bellman_ford'.");
    }
}
