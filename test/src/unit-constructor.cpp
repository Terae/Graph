//
// Created by Terae on 01/08/17.
//

#include <utility>

#include "catch.hpp"

#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

/**
 * TODO
 * initializations with personal objects (finish structs ; do class, ...)
 * check operator = for complete graph
 */

struct cost {
    explicit cost(double n) : number(n) {}
    double number;
    bool operator<(const cost &other) const {
        return number < other.number;
    }
};
template<>
struct std::numeric_limits<cost> {
    static constexpr bool is_specialized = true;
    static bool has_infinity() {
        return true;
    }
    static cost infinity() {
        return cost(std::numeric_limits<double>::infinity());
    }
    static cost max() {
        return cost(std::numeric_limits<double>::max());
    }
};

TEST_CASE("constructors") {
    using namespace std;

    SECTION("create a null graph") {
        using Graph            = graph           <string, int>;
        using Graph_directed   = graph_directed  <string, int>;
        using Graph_undirected = graph_undirected<string, int>;
        SECTION("no parameter") {
            Graph g1;
            Graph g2;
            CHECK(g1.empty());
            CHECK(g2.empty());
            CHECK(g1.get_nature() == UNDIRECTED);
        }

        SECTION("directed") {
            Graph_directed g;
            CHECK(g.get_nature() == DIRECTED);
        }

        SECTION("undirected") {
            Graph_undirected g;
            CHECK(g.get_nature() == UNDIRECTED);
        }
    }

    SECTION("initialize an empty graph") {
        SECTION("default graph") {
            graph<string, int, size_t> g;
            g["node 1"] = 1;
            g["node 2"] = 2;
            g.add_edge("node 1", "node 2", 4);
            //g("node 1", "node 2") = 4;
            CHECK(g.existing_edge("node 1", "node 2"));
        }

        SECTION("graph<bool, bool, bool>") {
            graph_directed<bool, bool, bool> g;
            g[true]  = true;
            g[false] = false;
            g(true, false) = true;
            CHECK(g.existing_edge(true, false));

            CHECK(g(false, true) == numeric_limits<bool>::max());
        }

        SECTION("graph<int, int, int>") {
            graph<int, int, int> g;
            g[1] = 11;
            g[2] = 22;
            g(1, 2) = 4;
            CHECK(g.existing_edge(1, 2));

            CHECK(g(1, 3) == numeric_limits<int>::max());
        }

        SECTION("graph<double, double, double>") {
            graph<double, double, double> g;
            g[1.0] = 11.0;
            g[2.0] = 22.0;
            g(1.0, 2.0) = 4.0;
            CHECK(g.existing_edge(1.0, 2.0));

            CHECK(g(1.0, 5.0) == numeric_limits<double>::infinity());
        }

        SECTION("graph<float, float, float>") {
            graph<float, float, float> g;
            g[1.0] = 11.0;
            g[2.0] = 22.0;
            g(1.0, 2.0) = 4.0;
            CHECK(g.existing_edge(1.0, 2.0));

            CHECK(g(1.0, 5.0) == numeric_limits<float>::infinity());
        }

        SECTION("graph<char, char, char>") {
            graph<char, char, char> g;
            g['a'] = 'A';
            g['b'] = 'B';
            g('a', 'b') = 'c';
            CHECK(g.existing_edge('a', 'b'));

            CHECK(g('a', 'e') == numeric_limits<char>::max());
        }

        SECTION("graph<string, string, string>") {
            graph<string, string, string> g;
            g["node 1"] = "1st node";
            g["node 2"] = "2nd node";
            g("node 1", "node 2") = "cost of 4";
            CHECK(g.existing_edge("node 1", "node 2"));

            CHECK(g("node 1", "new node") == numeric_limits<string>::infinity());
        }

        SECTION("graph of enum types") {
            enum key  {node_1, node_2};
            enum t    {data_1, data_2};
            enum cost {cost_1};

            graph<key, t, cost> g;
            g[node_1] = data_1;
            g[node_2] = data_2;
            g(node_1, node_2) = cost_1;
            CHECK(g.existing_edge(node_1, node_2));
        }

        SECTION("graph of struct types") {
            struct key {
                explicit key(string n, int v) : name(std::move(n)), value(v) {}
                string name;
                int value;
                bool operator<(const key &other) const {
                    return tie(name, value) < tie(other.name, other.value);
                }
            };
            struct t {
                explicit t(float d = 0.0) : data(d) {}
                float data;
                bool operator<(const t &other) const {
                    return data < other.data;
                }
            };
            graph<key, t, cost> g;
            key k1("node 1", 1);
            key k2 = key("node 2", 2);
            g.add_node(k1, t(11.11));
            g[k2] = t(22.22);
            g(k1, k2) = cost(444.444);
            CHECK(g.existing_edge(k1, k2));
        }

        SECTION("Cycled graph") {
            SECTION("directed") {
                graph_directed<std::string, int, double> g;
                g("node 1", "node 2") = 1.2;
                g("node 2", "node 3") = 2.3;
                g("node 3", "node 1") = 3.1;

                CHECK(g.get_nbr_nodes() == 3);
                CHECK(g.get_nbr_edges() == 3);
                CHECK(g.erase("node 1") == 1);
                CHECK(g.get_nbr_edges() == 1);

                g("node 3", "node 2") = 3.2;
                g.clear();
                CHECK(g.empty());
                CHECK(g.get_nbr_edges() == 0);
            }

            SECTION("undirected") {
                graph_undirected<std::string, int, double> g;
                g["node 1"] = 1;
                g["node 2"] = 2;
                g["node 3"] = 3;
                g.make_complete(4.2);

                CHECK(g.get_nbr_nodes() == 3);
                CHECK(g.get_nbr_edges() == 3);
                CHECK(g.erase("node 1") == 1);
                CHECK(g.get_nbr_edges() == 1);

                g.clear();
                CHECK(g.empty());
                CHECK(g.get_nbr_edges() == 0);
            }
        }
    }

    SECTION("istream constructor") {
        using Graph = graph<string, int, int>;
        Graph initial;
        initial("node 1", "node 2") = 12;
        initial("node 2", "node 1") = 21;
        initial("node 2", "node 3") = 23;

        stringstream ss;
        ss << initial;
        Graph final(ss);

        CHECK(initial == final);
    }

    SECTION("copy constructor") {
        using Graph            = graph           <string, int, int>;
        using Graph_directed   = graph_directed  <string, int, int>;
        using Graph_undirected = graph_undirected<string, int, int>;

        SECTION("null") {
            Graph g1;
            Graph g2(g1);
            CHECK(g1 == g2);
        }

        SECTION("nodes only") {
            Graph initial;
            for (int i{0}; i < 100; ++i) {
                initial["node " + to_string(i)] = i;
            }

            Graph final = initial;
            CHECK(initial == final);

            final["node 55"] = 555;
            CHECK(initial != final);
            CHECK(initial.size() == final.size());

            final["node 666"] = 6;
            CHECK(initial.size() + 1 == final.size());
        }

        SECTION("complete graph") {
            SECTION("directed") {
                Graph_directed initial;
                for (int i{0}; i < 100; ++i) {
                    initial["node " + to_string(i)] = i;
                }

                for (int i{0}; i < 99; i += 3) {
                    initial("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
                }
                for (int i{0}; i < 98; i += 3) {
                    initial("node " + to_string(i + 2), "node " + to_string(i + 1)) = 40 * i;
                }

                Graph_directed final(initial);
                CHECK(initial == final);

                final("node 1", "node 3") = 42;
                CHECK(initial != final);
                CHECK(initial.get_nbr_edges() + 1 == final.get_nbr_edges());

                CHECK_FALSE(initial.existing_edge("node 1", "node 3"));
                CHECK      (final.existing_edge  ("node 1", "node 3"));
                CHECK_FALSE(final.existing_edge  ("node 3", "node 1"));
            }

            SECTION("undirected") {
                Graph_undirected initial;
                for (int i{0}; i < 3; ++i) {
                    initial["node " + to_string(i)] = i;
                }
                for (int i{0}; i < 3; i += 3) {
                    initial("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
                }
                for (int i{0}; i < 3; i += 3) {
                    initial("node " + to_string(i + 2), "node " + to_string(i + 1)) = 40 * i;
                }

                Graph_undirected final(initial);
                CHECK(initial == final);

                final("node 1", "node 3") = 42;
                CHECK(initial != final);
                CHECK(initial.get_nbr_edges() + 1 == final.get_nbr_edges());

                CHECK_FALSE(initial.existing_edge("node 1", "node 3"));
                CHECK      (final.existing_edge  ("node 1", "node 3"));
                CHECK      (final.existing_edge  ("node 3", "node 1"));
            }
        }

        SECTION("move constructor") {
            Graph_directed initial;
            for (int i{0}; i < 100; ++i) {
                initial["node " + to_string(i)] = i;
            }

            for (int i{0}; i < 99; i += 3) {
                initial("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
            }

            Graph_directed final(move(initial));
            CHECK(initial != final);
            CHECK(initial.empty());
            CHECK(final.size() == 100);

            CHECK_NOTHROW(final = move(final));
        }

        SECTION("assignment operator") {
            SECTION("directed") {
                Graph_directed initial;
                for (int i{0}; i < 100; ++i) {
                    initial["node " + to_string(i)] = i;
                }

                for (int i{0}; i < 99; i += 3) {
                    initial("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
                }
                for (int i{0}; i < 98; i += 3) {
                    initial("node " + to_string(i + 2), "node " + to_string(i + 1)) = 40 * i;
                }

                Graph_directed final = initial;
                CHECK(initial == final);

                final("node 1", "node 3") = 42;
                CHECK(initial != final);
                CHECK(initial.get_nbr_edges() + 1 == final.get_nbr_edges());

                CHECK_FALSE(initial.existing_edge("node 1", "node 3"));
                CHECK      (final.existing_edge  ("node 1", "node 3"));
                CHECK_FALSE(final.existing_edge  ("node 3", "node 1"));
            }

            SECTION("undirected") {
                Graph_undirected initial;
                for (int i{0}; i < 100; ++i) {
                    initial["node " + to_string(i)] = i;
                }

                for (int i{0}; i < 99; i += 3) {
                    initial("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
                }
                for (int i{0}; i < 98; i += 3) {
                    initial("node " + to_string(i + 2), "node " + to_string(i + 1)) = 40 * i;
                }

                Graph_undirected final = initial;
                CHECK(initial == final);

                final("node 1", "node 3") = 42;
                CHECK(initial != final);
                CHECK(initial.get_nbr_edges() + 1 == final.get_nbr_edges());

                CHECK_FALSE(initial.existing_edge("node 1", "node 3"));
                CHECK      (final.existing_edge  ("node 1", "node 3"));
                CHECK      (final.existing_edge  ("node 3", "node 1"));
            }
        }

        SECTION("move assignment operator") {
            Graph initial;
            for (int i{0}; i < 100; ++i) {
                initial["node " + to_string(i)] = i;
            }

            for (int i{0}; i < 99; i += 3) {
                initial("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
            }

            Graph final;
            final["node 0"] = 5;
            final = move(initial);
            CHECK(initial != final);
            CHECK(initial.size() == 1);
            CHECK(final.size() == 100);
            CHECK(initial["node 0"] == 5);
        }
    }
}
