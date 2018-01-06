//
// Created by Terae on 01/08/17.
//

#include "catch.hpp"
#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

TEST_CASE("modifiers") {
    using namespace std;

    using Graph      = graph<string, int, size_t>;
    using iterator   = graph<string, int, size_t>::iterator;
    using citerator  = graph<string, int, size_t>::const_iterator;

    using Graph_directed      = graph_directed<string, int, size_t>;
    using iterator_directed   = graph_directed<string, int, size_t>::iterator;
    using citerator_directed  = graph_directed<string, int, size_t>::const_iterator;

    using Graph_undirected      = graph_undirected<string, int, size_t>;
    using iterator_undirected   = graph_undirected<string, int, size_t>::iterator;
    using citerator_undirected  = graph_undirected<string, int, size_t>::const_iterator;

    SECTION("adders") {
        SECTION("insert") {
            //SECTION("insert(const value_type& val)") {
            //    Graph g;
            //    for (int i{0}; i < 10; ++i)
            //        g["node " + to_string(i)] = i;
            //
            //    CHECK(g.count("node 15") == 0);
            //    shared_ptr<Graph::node> ptr_1{make_shared<Graph::node>(15)};
            //    pair<iterator, bool> result_1{g.insert(make_pair("node 15", ptr_1))};
            //    CHECK(g.get_iterator(ptr_1) == result_1.first);
            //    CHECK(result_1.second == true);
            //    CHECK(g.count("node 15") == 1);
            //
            //    shared_ptr<Graph::node> ptr_2{make_shared<Graph::node>(5)};
            //    pair<iterator, bool> result_2{g.insert(make_pair("node 5", ptr_2))};
            //    CHECK(g.get_iterator(ptr_2) != result_2.first); // already existing "node 5"
            //    CHECK(result_2.second == false);
            //    CHECK(g.count("node 5") == 1);
            //}

            SECTION("insert(const_iterator position, const value_type& val)") {
                Graph g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                citerator cit{next(begin(g), 3)};
                shared_ptr<Graph::node> ptr{make_shared<Graph::node>(42)};
                iterator it{g.insert(cit, make_pair("node 42", ptr))};
                CHECK(it == next(begin(g), 5));
            }

            SECTION("insert(const_iterator position, const key_type& k, graphed_type& x") {
                Graph g;
                iterator it{g.begin()};
                for (int i{0}; i < 10; ++i)
                    it = g.insert(it, "node " + to_string(i), i);
                CHECK(it == --g.end());
            }

            SECTION("insert(const_iterator position, const key_type& k, const Node& n)") {
                Graph g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;
                Graph::node n(12);
                iterator it{g.insert(g.begin(), "node 12", n)};
            }
        }

        SECTION("emplace") {
            SECTION("emplace(const key_type& k)") {
                Graph g;
                g["node 1"] = 1;
                g["node 3"] = 3;
                g["node 5"] = 5;

                pair<iterator, bool> result_1{g.emplace("new node")};
                CHECK(result_1.first == g.begin());
                CHECK(result_1.second);

                pair<iterator, bool> result_2{g.emplace("node 3")};
                CHECK(result_2.first == next(g.begin(), 2));
                CHECK(result_2.first->second->get() == 3);
                CHECK_FALSE(result_2.second);
            }

            SECTION("emplace(const key_type& k, graphed_type& x)") {
                Graph g;
                g["node 1"] = 1;
                g["node 3"] = 3;
                g["node 5"] = 5;

                pair<iterator, bool> result_1{g.emplace("new node", 18)};
                CHECK(result_1.first == g.begin());
                CHECK(result_1.first->second->get() == 18);
                CHECK(result_1.second);

                pair<iterator, bool> result_2{g.emplace("node 3", 42)};
                CHECK(result_2.first == next(g.begin(), 2));
                CHECK(result_2.first->second->get() == 3);
                CHECK_FALSE(result_2.second);
            }

            SECTION("emplace(const key_type& k, const Node& n)") {
                Graph g;
                g["node 1"] = 1;
                g["node 3"] = 3;
                g["node 5"] = 5;

                Graph::node n(12);
                pair<iterator, bool> result{g.emplace("new node", n)};
                CHECK(result.first == g.begin());
                CHECK(result.first->second->get() == 12);
                CHECK(result.second);
            }
        }

        SECTION("add_node") {
            SECTION("add_node(const key_type& k)") {
                Graph g;
                g["node 1"] = 1;
                g["node 3"] = 3;
                g["node 5"] = 5;

                pair<iterator, bool> result_1{g.add_node("new node")};
                CHECK(result_1.first == g.begin());
                CHECK(result_1.second);

                pair<iterator, bool> result_2{g.add_node("node 3")};
                CHECK(result_2.first == next(g.begin(), 2));
                CHECK(result_2.first->second->get() == 3);
                CHECK_FALSE(result_2.second);
            }

            SECTION("add_node(const key_type& k, const graphed_type& x)") {
                Graph g;
                g["node 1"] = 1;
                g["node 3"] = 3;
                g["node 5"] = 5;

                pair<iterator, bool> result_1{g.add_node("new node", 18)};
                CHECK(result_1.first == g.begin());
                CHECK(result_1.first->second->get() == 18);
                CHECK(result_1.second);

                pair<iterator, bool> result_2{g.add_node("node 3", 42)};
                CHECK(result_2.first == next(g.begin(), 2));
                CHECK(result_2.first->second->get() == 3);
                CHECK_FALSE(result_2.second);
            }

            SECTION("add_node(const key_type& k, const Node& n)") {
                Graph g;
                g["node 1"] = 1;
                g["node 3"] = 3;
                g["node 5"] = 5;

                Graph::node n(12);
                pair<iterator, bool> result{g.add_node("new node", n)};
                CHECK(result.first == g.begin());
                CHECK(result.first->second->get() == 12);
                CHECK(result.second);
            }
        }

        SECTION("add_edge(const_iterator it1, const_iterator it2, Cost cost") {
            SECTION("directed") {
                Graph_directed g;
                iterator_directed it[10];
                for (int i{0}; i < 10; ++i)
                    it[i] = g.add_node("node " + to_string(i)).first;

                g.add_edge(it[1], it[2], 12);
                g.add_edge(it[1], it[5], 15);

                CHECK(g.get_nbr_nodes() == 10);
                CHECK(g.get_nbr_edges() == 2);
                CHECK(g.existing_edge(it[1], it[5]));
                CHECK_FALSE(g.existing_edge(it[5], it[1]));
                CHECK_FALSE(g.existing_edge(it[2], it[5]));
            }

            SECTION("undirected") {
                Graph_undirected g;
                iterator_undirected it[10];
                for (int i{0}; i < 10; ++i)
                    it[i] = g.add_node("node " + to_string(i)).first;

                g.add_edge(it[1], it[2], 12);
                g.add_edge(it[1], it[5], 15);

                CHECK(g.get_nbr_nodes() == 10);
                CHECK(g.get_nbr_edges() == 2);
                CHECK(g.existing_edge(it[1], it[5]));
                CHECK(g.existing_edge(it[5], it[1]));
                CHECK(g(it[5], it[1]) == 15);
                CHECK_FALSE(g.existing_edge(it[2], it[5]));

                g(it[1], it[2]) = 42;
                CHECK(g(it[2], it[1]) == 42);
            }
        }

        SECTION("add_edge(const key_type& k1, const key_type& k2, Cost cost") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g.add_edge("node 1", "node 2", 12);
                g.add_edge("node 1", "node 5", 15);
                g.add_edge("node 1", "new node", 123);
                g.add_edge("new node", "node 5", 321);

                CHECK(g.get_nbr_nodes() == 11);
                CHECK(g.get_nbr_edges() == 4);
                CHECK(g.existing_edge("node 1", "node 5"));
                CHECK_FALSE(g.existing_edge("node 5", "node 1"));
                CHECK_FALSE(g.existing_edge("node 2", "node 5"));
                CHECK(g.existing_edge("node 1", "new node"));
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g.add_edge("node 1", "node 2", 12);
                g.add_edge("node 1", "node 5", 15);
                g.add_edge("node 1", "new node", 123);
                g.add_edge("new node", "node 5", 321);

                CHECK(g.get_nbr_nodes() == 11);
                CHECK(g.get_nbr_edges() == 4);
                CHECK(g.existing_edge("node 1", "node 5"));
                CHECK(g.existing_edge("node 5", "node 1"));
                CHECK_FALSE(g.existing_edge("node 2", "node 5"));
                CHECK(g.existing_edge("node 1", "new node"));
            }
        }

        SECTION("link_all_nodes(Cost cost)") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g.link_all_nodes(5);
                CHECK(g.get_nbr_edges() == 90); // COMBINATION(10, 2) * 2
                CHECK(g.existing_edge("node 2", "node 8"));
                CHECK(g("node 3", "node 9") == 5);

                g.link_all_nodes(80);
                CHECK(g.get_nbr_edges() == 90);
                CHECK(g("node 1", "node 2") == 80);

                g("node 1", "new node") = 4;
                g.link_all_nodes(50);
                CHECK(g.get_nbr_edges() == 110); // COMBINATION(11, 2) * 2
                CHECK(g("node 7", "new node") == 50);

                g.link_all_nodes(numeric_limits<size_t>::max());
                CHECK(g.get_nbr_edges() == 0);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g.link_all_nodes(5);
                CHECK(g.get_nbr_edges() == 45);
                CHECK(g.existing_edge("node 2", "node 8"));
                CHECK(g("node 3", "node 9") == 5);

                g.link_all_nodes(80);
                CHECK(g.get_nbr_edges() == 45);
                CHECK(g("node 1", "node 2") == 80);

                g("node 1", "new node") = 4;
                g.link_all_nodes(50);
                CHECK(g.get_nbr_edges() == 55);
                CHECK(g("node 7", "new node") == 50);
            }
        }
    }

    SECTION("deleters") {
        SECTION("erase(const_iterator position)") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;
                iterator_directed it5{++g.find("node 5")};
                iterator_directed it6{g.erase(g.find("node 5"))};
                CHECK(it5 == it6);
                CHECK(g.size() == 9);

                g("node 1", "node 2") = 42;
                g("node 1", "node 3") = 15;
                iterator_directed it1{g.find("node 1")};
                iterator_directed it2{g.find("node 2")};
                iterator_directed it3{g.find("node 3")};
                CHECK(g.get_nbr_edges() == 2);
                g.erase(it2);
                CHECK_FALSE(g.existing_edge(it1, g.find("node 2"))); // it2 does not point to anything
                CHECK(g.existing_edge(it1, it3));

                CHECK(g.get_nbr_edges() == 1);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;
                iterator_undirected it5{++g.find("node 5")};
                iterator_undirected it6{g.erase(g.find("node 5"))};
                CHECK(it5 == it6);
                CHECK(g.size() == 9);

                g("node 1", "node 2") = 42;
                g("node 1", "node 3") = 15;
                iterator_undirected it1{g.find("node 1")};
                iterator_undirected it2{g.find("node 2")};
                CHECK(g.get_nbr_edges() == 2);
                g.erase(it2);
                CHECK_FALSE(g.existing_edge(it1, g.find("node 2")));
                CHECK_FALSE(g.existing_edge(it2, it1));
                CHECK(g.get_nbr_edges() == 1);
            }
        }

        SECTION("erase(const_iterator first, const_iterator last) - simple") {
            Graph g;
            for (int i{0}; i < 10; ++i)
                g["node " + to_string(i)] = i;
            citerator first{g.find("node 5")};
            citerator last{g.find("node 8")};
            citerator result{g.erase(first, last)};
            CHECK(result == last);
            CHECK(g.size() == 7);
        }

        SECTION("erase(const_iterator first, const_iterator last) - edges") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g("node 1", "node 3") = 13;
                g("node 1", "node 8") = 18;
                citerator_directed it2{g.find("node 2")};
                citerator_directed it4{g.find("node 4")};
                g.erase(it2, it4);
                CHECK_FALSE(g.existing_edge("node 1", "node 3"));
                CHECK(g.get_nbr_edges() == 1);
                CHECK(g.size() == 8);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g("node 1", "node 3") = 13;
                g("node 1", "node 8") = 18;
                iterator_undirected it2{g.find("node 2")};
                iterator_undirected it4{g.find("node 4")};
                g.erase(it2, it4);
                CHECK_FALSE(g.existing_edge("node 1", "node 3"));
                CHECK(g.get_nbr_edges() == 1);
            }
        }

        SECTION("erase(const_iterator first, const_iterator last) - clear") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g.erase(g.begin(), g.end());
                CHECK(g.empty());
                CHECK(g.get_nbr_edges() == 0);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g.erase(g.begin(), g.end());
                CHECK(g.empty());
                CHECK(g.get_nbr_edges() == 0);
            }
        }

        SECTION("erase(const key_type& k)") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;
                CHECK(g.erase("node 4") == 1);
                CHECK(g.erase("unknown node") == 0);
                CHECK(g.size() == 9);

                g("node 1", "node 2") = 42;
                g("node 1", "node 3") = 15;
                CHECK(g.get_nbr_edges() == 2);
                g.erase("node 2");
                CHECK_FALSE(g.existing_edge("node 1", "node 2"));
                CHECK(g.existing_edge("node 1", "node 3"));

                CHECK(g.get_nbr_edges() == 1);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;
                CHECK(g.erase("node 4") == 1);
                CHECK(g.erase("unknown node") == 0);
                CHECK(g.size() == 9);

                g("node 1", "node 2") = 42;
                g("node 1", "node 3") = 15;
                CHECK(g.get_nbr_edges() == 2);
                g.erase("node 2");
                CHECK_FALSE(g.existing_edge("node 1", "node 2"));
                CHECK_FALSE(g.existing_edge("node 2", "node 1"));
                CHECK(g.get_nbr_edges() == 1);
            }
        }

        SECTION("del_node(const_iterator position)") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;
                iterator_directed it5{++g.find("node 5")};
                iterator_directed it6{g.del_node(g.find("node 5"))};
                CHECK(it5 == it6);
                CHECK(g.size() == 9);

                g("node 1", "node 2") = 42;
                g("node 1", "node 3") = 15;
                iterator_directed it1{g.find("node 1")};
                iterator_directed it2{g.find("node 2")};
                iterator_directed it3{g.find("node 3")};
                CHECK(g.get_nbr_edges() == 2);
                g.del_node(it2);
                CHECK_FALSE(g.existing_edge(it1, g.find("node 2")));
                CHECK(g.existing_edge(it1, it3));

                CHECK(g.get_nbr_edges() == 1);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;
                iterator_undirected it5{++g.find("node 5")};
                iterator_undirected it6{g.del_node(g.find("node 5"))};
                CHECK(it5 == it6);
                CHECK(g.size() == 9);

                g("node 1", "node 2") = 42;
                g("node 1", "node 3") = 15;
                iterator_undirected it1{g.find("node 1")};
                iterator_undirected it2{g.find("node 2")};
                CHECK(g.get_nbr_edges() == 2);
                g.del_node(it2);
                CHECK_FALSE(g.existing_edge(it1, g.find("node 2")));
                CHECK_FALSE(g.existing_edge(g.find("node 2"), it1));
                CHECK(g.get_nbr_edges() == 1);
            }
        }

        SECTION("del_nodes(const_iterator first, const_iterator last) - simple") {
            Graph g;
            for (int i{0}; i < 10; ++i)
                g["node " + to_string(i)] = i;
            iterator first{g.find("node 5")};
            iterator last{g.find("node 8")};
            iterator result{g.del_nodes(first, last)};
            CHECK(result == last);
            CHECK(g.size() == 7);
        }

        SECTION("del_nodes(const_iterator first, const_iterator last) - edges") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g("node 1", "node 3") = 13;
                g("node 1", "node 8") = 18;
                iterator_directed it2{g.find("node 2")};
                iterator_directed it4{g.find("node 4")};
                g.del_nodes(it2, it4);
                CHECK_FALSE(g.existing_edge("node 1", "node 3"));
                CHECK(g.get_nbr_edges() == 1);
                CHECK(g.size() == 8);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g("node 1", "node 3") = 13;
                g("node 1", "node 8") = 18;
                iterator_undirected it2{g.find("node 2")};
                iterator_undirected it4{g.find("node 4")};
                g.del_nodes(it2, it4);
                CHECK_FALSE(g.existing_edge("node 1", "node 3"));
                CHECK(g.get_nbr_edges() == 1);
            }
        }

        SECTION("del_nodes(const_iterator first, const_iterator last) - clear") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g.del_nodes(g.begin(), g.end());
                CHECK(g.empty());
                CHECK(g.get_nbr_edges() == 0);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;

                g.del_nodes(g.begin(), g.end());
                CHECK(g.empty());
                CHECK(g.get_nbr_edges() == 0);
            }
        }

        SECTION("del_node(const key_type& k)") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;
                CHECK(g.del_node("node 4") == 1);
                CHECK(g.del_node("unknown node") == 0);
                CHECK(g.size() == 9);

                g("node 1", "node 2") = 42;
                g("node 1", "node 3") = 15;
                CHECK(g.get_nbr_edges() == 2);
                g.del_node("node 2");
                CHECK_FALSE(g.existing_edge("node 1", "node 2"));
                CHECK(g.existing_edge("node 1", "node 3"));

                CHECK(g.get_nbr_edges() == 1);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g["node " + to_string(i)] = i;
                CHECK(g.del_node("node 4") == 1);
                CHECK(g.del_node("unknown node") == 0);
                CHECK(g.size() == 9);

                g("node 1", "node 2") = 42;
                g("node 1", "node 3") = 15;
                CHECK(g.get_nbr_edges() == 2);
                g.del_node("node 2");
                CHECK_FALSE(g.existing_edge("node 1", "node 2"));
                CHECK_FALSE(g.existing_edge("node 2", "node 1"));
                CHECK(g.get_nbr_edges() == 1);
            }
        }

        SECTION("clear()") {
            Graph g;
            for (int i{0}; i < 10; ++i)
                g["node " + to_string(i)] = i;
            g("node 1", "node 2") = 42;
            g("node 2", "node 5") = 50;
            g("node 8", "new node") = 5;

            g.clear();
            CHECK(g.empty());
            CHECK(g.get_nbr_edges() == 0);
            CHECK(g.get_nbr_nodes() == 0);
        }

        SECTION("del_edge(iterator it1, iterator it2)") {
            SECTION("directed") {
                Graph_directed g;
                iterator_directed it[10];
                for (int i{0}; i < 10; ++i)
                    it[i] = g.add_node("node " + to_string(i)).first;

                g.add_edge(it[1], it[2], 12);
                g.add_edge(it[1], it[5], 15);

                CHECK_FALSE(g.del_edge(it[8], it[9]));
                CHECK_FALSE(g.del_edge(it[2], it[1]));
                CHECK(g.get_nbr_edges() == 2);
                CHECK(g.del_edge(it[1], it[2]));
                CHECK(g.get_nbr_edges() == 1);
                CHECK_FALSE(g.existing_edge(it[1], it[2]));
            }

            SECTION("undirected") {
                Graph_undirected g;
                iterator_undirected it[10];
                for (int i{0}; i < 10; ++i)
                    it[i] = g.add_node("node " + to_string(i)).first;

                g.add_edge(it[1], it[2], 12);
                g.add_edge(it[1], it[5], 15);

                CHECK_FALSE(g.del_edge(it[8], it[9]));
                CHECK(g.get_nbr_edges() == 2);

                CHECK(g.del_edge(it[2], it[1]));
                CHECK(g.get_nbr_edges() == 1);
                CHECK_FALSE(g.existing_edge(it[1], it[2]));
                CHECK_FALSE(g.existing_edge(it[2], it[1]));

                CHECK(g.del_edge(it[1], it[5]));
                CHECK(g.get_nbr_edges() == 0);
                CHECK_FALSE(g.existing_edge(it[1], it[5]));
                CHECK_FALSE(g.existing_edge(it[5], it[1]));
            }
        }

        SECTION("del_edge(const Key& k1, const Key& k2") {
            SECTION("directed") {
                Graph_directed g;
                iterator_directed it[10];
                for (int i{0}; i < 10; ++i)
                    it[i] = g.add_node("node " + to_string(i)).first;

                g.add_edge("node 1", "node 2", 12);
                g.add_edge("node 1", "node 5", 15);

                CHECK_FALSE(g.del_edge("node 8", "node 9"));
                CHECK_FALSE(g.del_edge("node 2", "unknown node"));
                CHECK(g.get_nbr_nodes() == 10);
                CHECK_FALSE(g.del_edge("node 2", "node 1"));
                CHECK(g.get_nbr_edges() == 2);
                CHECK(g.del_edge("node 1", "node 2"));
                CHECK(g.get_nbr_edges() == 1);
                CHECK_FALSE(g.existing_edge("node 1", "node 2"));
            }

            SECTION("undirected") {
                Graph_undirected g;
                iterator_undirected it[10];
                for (int i{0}; i < 10; ++i)
                    it[i] = g.add_node("node " + to_string(i)).first;

                g.add_edge("node 1", "node 2", 12);
                g.add_edge("node 1", "node 5", 15);

                CHECK_FALSE(g.del_edge("node 8", "node 9"));
                CHECK_FALSE(g.del_edge("node 2", "unknown node"));
                CHECK(g.get_nbr_edges() == 2);

                CHECK(g.del_edge("node 2", "node 1"));
                CHECK(g.get_nbr_edges() == 1);
                CHECK_FALSE(g.existing_edge("node 1", "node 2"));
                CHECK_FALSE(g.existing_edge("node 2", "node 1"));

                CHECK(g.del_edge("node 1", "node 5"));
                CHECK(g.get_nbr_edges() == 0);
                CHECK_FALSE(g.existing_edge("node 1", "node 5"));
                CHECK_FALSE(g.existing_edge("node 5", "node 1"));
            }
        }

        SECTION("clear_edges()") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g.add_node("node " + to_string(i));

                g("node 1", "node 5") = 15;
                g("node 5", "node 6") = 56;
                g("node 4", "node 2") = 42;
                g("node 8", "node 1") = 81;

                CHECK(g.get_nbr_edges() == 4);
                g.clear_edges();

                CHECK(g.get_nbr_edges() == 0);
                for (citerator_directed cit{g.cbegin()}; cit != g.cend(); ++cit)
                    CHECK(cit->second->degree().second == 0);
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g.add_node("node " + to_string(i));

                g("node 1", "node 5") = 15;
                g("node 5", "node 6") = 56;
                g("node 4", "node 2") = 42;
                g("node 8", "node 1") = 81;

                CHECK(g.get_nbr_edges() == 4);
                g.clear_edges();

                CHECK(g.get_nbr_edges() == 0);
                for (citerator_undirected cit{g.cbegin()}; cit != g.cend(); ++cit)
                    CHECK(cit->second->degree().second == 0);
            }
        }

        SECTION("clear_edges(iterator it)") {
            SECTION("directed") {
                Graph_directed g;
                iterator_directed it[10];
                for (int i{0}; i < 10; ++i)
                    it[i] = g.add_node("node " + to_string(i)).first;

                g(it[1], it[5]) = 15;
                g(it[5], it[6]) = 56;
                g(it[4], it[2]) = 42;
                g(it[8], it[1]) = 81;

                CHECK(g.get_nbr_edges() == 4);
                g.clear_edges(it[4]);
                CHECK(g.get_nbr_edges() == 3);
                CHECK_FALSE(g.existing_edge(it[4], it[2]));

                g.clear_edges(it[1]);
                CHECK(g.get_nbr_edges() == 1);
                CHECK_FALSE(g.existing_edge(it[1], it[5]));
                CHECK_FALSE(g.existing_edge(it[8], it[1]));
                CHECK(g.existing_edge(it[5], it[6]));
            }

            SECTION("undirected") {
                Graph_undirected g;
                iterator_undirected it[10];
                for (int i{0}; i < 10; ++i)
                    it[i] = g.add_node("node " + to_string(i)).first;

                g(it[1], it[5]) = 15;
                g(it[5], it[6]) = 56;
                g(it[4], it[2]) = 42;
                g(it[8], it[1]) = 81;

                CHECK(g.get_nbr_edges() == 4);
                g.clear_edges(it[4]);
                CHECK(g.get_nbr_edges() == 3);
                CHECK_FALSE(g.existing_edge(it[4], it[2]));

                CHECK(g.existing_edge(it[8], it[1]));
                g.clear_edges(it[1]);
                CHECK(g.get_nbr_edges() == 1);
                CHECK_FALSE(g.existing_edge(it[1], it[5]));
                CHECK_FALSE(g.existing_edge(it[8], it[1]));
                CHECK(g.existing_edge(it[5], it[6]));
                CHECK(g.existing_edge(it[6], it[5]));
            }
        }

        SECTION("clear_edges(const Key& k)") {
            SECTION("directed") {
                Graph_directed g;
                for (int i{0}; i < 10; ++i)
                    g.add_node("node " + to_string(i));

                g("node 1", "node 5") = 15;
                g("node 5", "node 6") = 56;
                g("node 4", "node 2") = 42;
                g("node 8", "node 1") = 81;

                CHECK(g.get_nbr_edges() == 4);
                g.clear_edges("unknown node");
                CHECK(g.get_nbr_nodes() == 10);
                CHECK(g.get_nbr_edges() == 4);

                g.clear_edges("node 4");
                CHECK(g.get_nbr_edges() == 3);
                CHECK_FALSE(g.existing_edge("node 4", "node 2"));

                g.clear_edges("node 1");
                CHECK(g.get_nbr_edges() == 1);
                CHECK_FALSE(g.existing_edge("node 1", "node 5"));
                CHECK_FALSE(g.existing_edge("node 8", "node 1"));
                CHECK(g.existing_edge("node 5", "node 6"));
            }

            SECTION("undirected") {
                Graph_undirected g;
                for (int i{0}; i < 10; ++i)
                    g.add_node("node " + to_string(i));

                g("node 1", "node 5") = 15;
                g("node 5", "node 6") = 56;
                g("node 4", "node 2") = 42;
                g("node 8", "node 1") = 81;

                CHECK(g.get_nbr_edges() == 4);
                g.clear_edges("unknown node");
                CHECK(g.get_nbr_nodes() == 10);
                CHECK(g.get_nbr_edges() == 4);

                g.clear_edges("node 4");
                CHECK(g.get_nbr_edges() == 3);
                CHECK_FALSE(g.existing_edge("node 4", "node 2"));

                g.clear_edges("node 1");
                CHECK(g.get_nbr_edges() == 1);
                CHECK_FALSE(g.existing_edge("node 1", "node 5"));
                CHECK_FALSE(g.existing_edge("node 8", "node 1"));
                CHECK(g.existing_edge("node 5", "node 6"));
                CHECK(g.existing_edge("node 6", "node 5"));
            }
        }
    }

    SECTION("others") {
        SECTION("swap(graph& other)") {
            Graph g1;
            g1["node 1"] = 1;
            g1["node 5"] = 5;
            Graph g1_copy = g1;

            Graph g2;
            g2["node 512"] = 512;
            g2["node 513"] = 513;
            g2["node 514"] = 514;
            Graph g2_copy = g2;

            g1.swap(g2);
            CHECK(g1 == g2_copy);
            CHECK(g2 == g1_copy);
        }

        SECTION("std::swap") {
            Graph g1;
            g1["node 1"] = 1;
            g1["node 5"] = 5;
            Graph g1_copy = g1;

            Graph g2;
            g2["node 512"] = 512;
            g2["node 513"] = 513;
            g2["node 514"] = 514;
            Graph g2_copy = g2;

            std::swap(g1, g2);
            CHECK(g1 == g2_copy);
            CHECK(g2 == g1_copy);
        }
    }
}
