//
// Created by Terae on 01/08/17.
//

#include "catch.hpp"
#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

TEST_CASE("element access") {
    using namespace std;

    using Graph_directed   = graph_directed  <string, int, size_t>;
    using Graph_undirected = graph_undirected<string, int, size_t>;

    SECTION("operator[](const key_type& k)") {
        const int a{5}, b{15};
        Graph_directed g;
        g["node"] = a;
        CHECK(g.get_nbr_nodes() == 1);
        CHECK(g["node"] == a);

        g["node"] = b;
        CHECK(g.get_nbr_nodes() == 1);
        CHECK(g["node"] == b);
    }

    SECTION("operator[](const key_type&& k)") {
        Graph_directed g;
        g["node"] = 5;
        CHECK(g.get_nbr_nodes() == 1);
        CHECK(g["node"] == 5);

        g["node"] = 15;
        CHECK(g.get_nbr_nodes() == 1);
        CHECK(g["node"] == 15);
    }

#if defined(GRAPH_HAS_CPP_17)
    SECTION("operator[](key_type&& k) const") {
        Graph_directed g1;
        g1["node"] = 5;
        const Graph_directed g2(g1);
        CHECK(g2["node"] == 5);
        CHECK(g2["new node"] == nullopt);
    }
#else
    SECTION("operator[](key_type&& k) const") {
        Graph_directed g1;
        g1["node"] = 5;
        const Graph_directed g2(g1);
        CHECK(g2["node"] == 5);
        CHECK_THROWS_WITH(g2["new node"],
                          "[graph.exception.invalid_argument] Unexistant node when calling 'operator[]'.");
    }
#endif

    SECTION("operator()(iterator it1, iterator it2)") {
        SECTION("directed") {
            Graph_directed g;
            for(int i{0}; i < 10; ++i)
                g["node " + to_string(i)] = i;

            Graph_directed::iterator it1{g.find("node 1")};
            Graph_directed::iterator it2{g.find("node 2")};
            Graph_directed::iterator it3{g.find("node 3")};
            Graph_directed::iterator it5{g.find("node 5")};

            g(it1, it2) = 12;
            g(it1, it5) = 15;

            CHECK(g.get_nbr_nodes() == 10);
            CHECK(g.get_nbr_edges() == 2);
            CHECK(g.existing_edge(it1, it5));
            CHECK_FALSE(g.existing_edge(it5, it1));
            CHECK(g(it1, it2) == 12);
            CHECK(g(it2, it1) == numeric_limits<size_t>::max());
            CHECK(g(it1, it3) == numeric_limits<size_t>::max());
        }

        SECTION("undirected") {
            Graph_undirected g;
            for(int i{0}; i < 10; ++i)
                g["node " + to_string(i)] = i;

            Graph_undirected::iterator it1{g.find("node 1")};
            Graph_undirected::iterator it2{g.find("node 2")};
            Graph_undirected::iterator it3{g.find("node 3")};
            Graph_undirected::iterator it5{g.find("node 5")};

            g(it1, it2) = 12;
            g(it1, it5) = 15;

            CHECK(g.get_nbr_nodes() == 10);
            CHECK(g.get_nbr_edges() == 2);
            CHECK(g.existing_edge(it1, it5));
            CHECK(g.existing_edge(it5, it1));
            CHECK(g(it1, it2) == 12);
            CHECK(g(it2, it1) == 12);
            CHECK(g(it1, it3) == numeric_limits<size_t>::max());
        }
    }

    SECTION("operator()(const key_type& k1, const key_type& k2)") {
        SECTION("directed") {
            Graph_directed g;
            for (int i{0}; i < 10; ++i)
                g["node " + to_string(i)] = i;

            g("node 1", "node 2") = 12;
            g("node 1", "node 5") = 15;
            g("node 1", "new node") = 123;
            g("new node", "node 5") = 321;

            CHECK(g.get_nbr_nodes() == 11);
            CHECK(g.get_nbr_edges() == 4);
            CHECK(g.existing_edge("node 1", "node 5"));
            CHECK_FALSE(g.existing_edge("node 5", "node 1"));
            CHECK_FALSE(g.existing_edge("node 2", "node 5"));
            CHECK(g.existing_edge("node 1", "new node"));
            CHECK(g("node 1", "node 2") == 12);
            CHECK(g("node 2", "node 1") == numeric_limits<size_t>::max());
            CHECK(g("node 1", "node 3") == numeric_limits<size_t>::max());
        }

        SECTION("undirected") {
            Graph_undirected g;
            for (int i = 0; i < 10; ++i)
                g["node " + to_string(i)] = i;

            g("node 1", "node 2") = 12;
            g("node 1", "node 5") = 15;
            g("node 1", "new node") = 123;
            g("new node", "node 5") = 321;

            CHECK(g.get_nbr_nodes() == 11);
            CHECK(g.get_nbr_edges() == 4);
            CHECK(g.existing_edge("node 1", "node 5"));
            CHECK(g.existing_edge("node 5", "node 1"));
            CHECK_FALSE(g.existing_edge("node 2", "node 5"));
            CHECK(g.existing_edge("node 1", "new node"));
            CHECK(g("node 1", "node 2") == 12);
            CHECK(g("node 2", "node 1") == 12);
            CHECK(g("node 1", "node 3") == numeric_limits<size_t>::max());
        }
    }

#if defined(GRAPH_HAS_CPP_17)
    SECTION("operator()(const key_type& k1, const key_type& k2) const") {
        SECTION("directed") {
            Graph_directed g1;
            for(int i{0}; i < 10; ++i)
                g1["node " + to_string(i)] = i;

            g1("node 1", "node 2") = 12;
            g1("node 1", "node 5") = 15;

            const Graph_directed g2(g1);
            CHECK(g2("node 1", "node 2") == 12);
            CHECK(g2("node 5", "node 1") == nullopt);
            CHECK(g2("node 1", "new node") == nullopt);
        }

        SECTION("undirected") {
            Graph_undirected g1;
            for(int i{0}; i < 10; ++i)
                g1["node " + to_string(i)] = i;

            g1("node 1", "node 2") = 12;
            g1("node 1", "node 5") = 15;

            const Graph_undirected g2(g1);
            CHECK(g2("node 1", "node 2") == 12);
            CHECK(g2("node 5", "node 1") == 15);
            CHECK(g2("node 1", "new node") == nullopt);
        }
    }

    SECTION("operator()(iterator it1, iterator it2) const") {
        SECTION("directed") {
            Graph_directed g1;
            for(int i{0}; i < 10; ++i)
                g1["node " + to_string(i)] = i;

            g1("node 1", "node 2") = 12;
            g1("node 1", "node 5") = 15;

            const Graph_directed g2(g1);
            Graph_directed::const_iterator it1{g2.find("node 1")};
            Graph_directed::const_iterator it2{g2.find("node 2")};
            Graph_directed::const_iterator it3{g2.find("node 3")};
            Graph_directed::const_iterator it5{g2.find("node 5")};

            CHECK(g2.get_nbr_nodes() == 10);
            CHECK(g2.get_nbr_edges() == 2);
            CHECK(g2.existing_edge(it1, it5));
            CHECK_FALSE(g2.existing_edge(it5, it1));
            CHECK(g2(it1, it2) == 12);
            CHECK(g2(it2, it1) == nullopt);
            CHECK(g2(it1, it3) == nullopt);
        }

        SECTION("undirected") {
            Graph_undirected g1;
            for(int i{0}; i < 10; ++i)
                g1["node " + to_string(i)] = i;

            g1("node 1", "node 2") = 12;
            g1("node 1", "node 5") = 15;

            const Graph_undirected g2(g1);
            Graph_undirected::const_iterator it1{g2.find("node 1")};
            Graph_undirected::const_iterator it2{g2.find("node 2")};
            Graph_undirected::const_iterator it3{g2.find("node 3")};
            Graph_undirected::const_iterator it5{g2.find("node 5")};

            CHECK(g2.get_nbr_nodes() == 10);
            CHECK(g2.get_nbr_edges() == 2);
            CHECK(g2.existing_edge(it1, it5));
            CHECK(g2.existing_edge(it5, it1));
            CHECK(g2(it1, it2) == 12);
            CHECK(g2(it2, it1) == 12);
            CHECK(g2(it1, it3) == nullopt);
        }
    }
#else
    SECTION("operator()(const key_type& k1, const key_type& k2) const") {
        SECTION("directed") {
            Graph_directed g1;
            for(int i{0}; i < 10; ++i)
                g1["node " + to_string(i)] = i;

            g1("node 1", "node 2") = 12;
            g1("node 1", "node 5") = 15;

            const Graph_directed g2(g1);
            CHECK(g2("node 1", "node 2") == 12);
            CHECK_THROWS_WITH(g2("node 5", "node 1"),
                              "[graph.exception.invalid_argument] Unexistant edge when calling 'operator()'.");
            CHECK_THROWS_WITH(g2("node 1", "new node"),
                              "[graph.exception.invalid_argument] Unexistant edge when calling 'operator()'.");
        }

        SECTION("undirected") {
            Graph_undirected g1;
            for(int i{0}; i < 10; ++i)
                g1["node " + to_string(i)] = i;

            g1("node 1", "node 2") = 12;
            g1("node 1", "node 5") = 15;

            const Graph_undirected g2(g1);
            CHECK(g2("node 1", "node 2") == 12);
            CHECK(g2("node 5", "node 1") == 15);
            CHECK_THROWS_WITH(g2("node 1", "new node"),
                              "[graph.exception.invalid_argument] Unexistant edge when calling 'operator()'.");
        }
    }

    SECTION("operator()(iterator it1, iterator it2) const") {
        SECTION("directed") {
            Graph_directed g1;
            for(int i{0}; i < 10; ++i)
                g1["node " + to_string(i)] = i;

            g1("node 1", "node 2") = 12;
            g1("node 1", "node 5") = 15;

            const Graph_directed g2(g1);
            Graph_directed::const_iterator it1{g2.find("node 1")};
            Graph_directed::const_iterator it2{g2.find("node 2")};
            Graph_directed::const_iterator it3{g2.find("node 3")};
            Graph_directed::const_iterator it5{g2.find("node 5")};

            CHECK(g2.get_nbr_nodes() == 10);
            CHECK(g2.get_nbr_edges() == 2);
            CHECK(g2.existing_edge(it1, it5));
            CHECK_FALSE(g2.existing_edge(it5, it1));
            CHECK(g2(it1, it2) == 12);
            CHECK_THROWS_WITH(g2(it2, it1),
                              "[graph.exception.invalid_argument] Unexistant edge when calling 'operator()'.");
            CHECK_THROWS_WITH(g2(it1, it3),
                              "[graph.exception.invalid_argument] Unexistant edge when calling 'operator()'.");
        }

        SECTION("undirected") {
            Graph_undirected g1;
            for(int i{0}; i < 10; ++i)
                g1["node " + to_string(i)] = i;

            g1("node 1", "node 2") = 12;
            g1("node 1", "node 5") = 15;

            const Graph_undirected g2(g1);
            Graph_undirected::const_iterator it1{g2.find("node 1")};
            Graph_undirected::const_iterator it2{g2.find("node 2")};
            Graph_undirected::const_iterator it3{g2.find("node 3")};
            Graph_undirected::const_iterator it5{g2.find("node 5")};

            CHECK(g2.get_nbr_nodes() == 10);
            CHECK(g2.get_nbr_edges() == 2);
            CHECK(g2.existing_edge(it1, it5));
            CHECK(g2.existing_edge(it5, it1));
            CHECK(g2(it1, it2) == 12);
            CHECK(g2(it2, it1) == 12);
            CHECK_THROWS_WITH(g2(it1, it3),
                              "[graph.exception.invalid_argument] Unexistant edge when calling 'operator()'.");
        }
    }
#endif
}
