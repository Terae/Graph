//
// Created by Terae on 01/08/17.
//

#include "catch.hpp"
#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

TEST_CASE("operations") {
    using namespace std;
    using Graph = graph<string, int, int>;
    
    SECTION("find(const key_type& k)") {
        Graph g;
        g["node 1"] = 1;
        Graph::iterator it = g.add_node("node 2", 2).first;
        g["node 3"] = 523;
        
        CHECK(g.find("node 2") == it);
        CHECK(g.find("node 3") != g.cend());
        CHECK(g.find("unknown node") == g.cend());
    }
    
    SECTION("count(const key_type& k)") {
        Graph g;
        for(int i = 0; i < 100; ++i)
            g["node " + to_string(i)] = i;
        CHECK(g.count("node 2") == 1);
        CHECK(g.count("new node") == 0);
    }
    
    SECTION("existing_node(const_iterator it) const") {
        Graph g;
        Graph::iterator it = g.add_node("node 1", 11).first;
        g["node 5"] = 55;
        g["node 120"] = 1;
        CHECK(g.existing_node(it));
        CHECK(!g.existing_node(g.cend()));
    }
    
    SECTION("existing_node(const Key& k) const") {
        Graph g;
        for(int i = 0; i < 100; ++i)
            g["node " + to_string(i)] = i;
        CHECK(g.existing_node("node 55"));
        CHECK(!g.existing_node("unknown node"));
    }
    
    SECTION("existing_edge(const_iterator it1, const_iterator it2) const") {
        SECTION("directed") {
            Graph g(DIRECTED);
            Graph::iterator it1 = g.add_node("node 1", 5).first;
            Graph::iterator it2 = g.add_node("node 2", 8).first;
            Graph::iterator it3 = g.add_node("node 3", 2).first;
    
            g.add_edge(it1, it2, 12);
            g.add_edge(it1, it3, 13);
            
            CHECK(g.existing_edge(it1, it2));
            CHECK_FALSE(g.existing_edge(it2, it1));
            CHECK_FALSE(g.existing_edge(it2, it3));
        }
    
        SECTION("undirected") {
            Graph g(UNDIRECTED);
            Graph::iterator it1{g.add_node("node 1", 5).first};
            Graph::iterator it2{g.add_node("node 2", 8).first};
            Graph::iterator it3{g.add_node("node 3", 2).first};
        
            g.add_edge(it1, it2, 12);
            g.add_edge(it1, it3, 13);
        
            CHECK(g.existing_edge(it1, it2));
            CHECK(g.existing_edge(it2, it1));
            CHECK_FALSE(g.existing_edge(it2, it3));
        }
    }
    
    SECTION("existing_edge(const Key& k1, const Key& k2) const") {
        SECTION("directed") {
            Graph g(DIRECTED);
            g["node 1"] = 5;
            g["node 2"] = 8;
            g["node 3"] = 3;
            
            g("node 1", "node 2") = 12;
            g("node 1", "node 3") = 13;
            
            CHECK(g.existing_edge("node 1", "node 2"));
            CHECK_FALSE(g.existing_edge("node 2", "node 1"));
            CHECK_FALSE(g.existing_edge("node 2", "node 3"));
        }
    
        SECTION("undirected") {
            Graph g(UNDIRECTED);
            g["node 1"] = 5;
            g["node 2"] = 8;
            g["node 3"] = 3;
    
            g("node 1", "node 2") = 12;
            g("node 1", "node 3") = 13;
    
            CHECK(g.existing_edge("node 1", "node 2"));
            CHECK(g.existing_edge("node 2", "node 1"));
            CHECK_FALSE(g.existing_edge("node 2", "node 3"));
        }
    }
    
    SECTION("get_nbr_nodes()") {
        Graph g;
        CHECK(g.get_nbr_nodes() == 0);
        
        const int N = 489;
        for(int i = 0; i < N; ++i)
            g["node " + to_string(i)] = i;
        CHECK(g.get_nbr_nodes() == N);
    }
    
    SECTION("get_nbr_edges()") {
        SECTION("directed") {
            Graph g(DIRECTED);
            for(int i = 0; i < 100; ++i)
                g["node " + to_string(i)] = i;
    
            CHECK(g.get_nbr_edges() == 0);
    
            g("node 2", "node 8") = 5;
            g("node 8", "node 5") = 2;
            g("node 5", "node 8") = 3;
            g("node 45", "node 1") = 45;
            g("node 12", "node 5") = 2;
            g("node 12", "node 5") = 2;
            CHECK(g.get_nbr_edges() == 5);
        }
    
        SECTION("undirected") {
            Graph g(UNDIRECTED);
            for(int i = 0; i < 100; ++i)
                g["node " + to_string(i)] = i;
        
            CHECK(g.get_nbr_edges() == 0);
        
            g("node 2", "node 8") = 5;
            g("node 8", "node 5") = 2;
            g("node 5", "node 8") = 3;
            g("node 45", "node 1") = 45;
            g("node 12", "node 5") = 2;
            g("node 12", "node 5") = 2;
            CHECK(g.get_nbr_edges() == 4);
        }
    }
    
    SECTION("get_nature()") {
        graph<string, int> g1(DIRECTED);
        CHECK(g1.get_nature() == DIRECTED);
        
        graph<string, int> g2(UNDIRECTED);
        CHECK(g2.get_nature() == UNDIRECTED);
    }
    
    SECTION("set_nature(Nature new_nature)") {
        SECTION("same nature") {
            graph<string, int, double> g1(DIRECTED);
            g1("node 1", "node 2") = 5.6;
            g1("node 2", "node 3") = 6.5;
            g1("node 1", "node 3") = 1.2;
            g1.set_nature(DIRECTED);
            CHECK(g1.get_nature() == DIRECTED);
            CHECK(g1.get_nbr_edges() == 3);
    
            graph<string, int, double> g2(UNDIRECTED);
            g2("node 1", "node 2") = 5.6;
            g2("node 2", "node 3") = 6.5;
            g2("node 1", "node 3") = 1.2;
            g2.set_nature(UNDIRECTED);
            CHECK(g2.get_nature() == UNDIRECTED);
            CHECK(g2.get_nbr_edges() == 3);
        }
        
        SECTION("directed -> undirected") {
            graph<string, int, double> g(DIRECTED);
            g("node 1", "node 2") = 1.2;
            g("node 2", "node 3") = 2.3;
            g("node 1", "node 3") = 1.3;
            g("node 4", "node 5") = 4.5;
            g.set_nature(UNDIRECTED);
            CHECK(g.get_nature() == UNDIRECTED);
            CHECK(g.get_nbr_edges() == 4);
            CHECK(g("node 5", "node 4") == 4.5);
            g("node 3", "node 1") = 3.1;
            CHECK(g("node 1", "node 3") == 3.1);
        }
        
        SECTION("undirected -> directed") {
            graph<string, int, double> g(UNDIRECTED);
            g("node 1", "node 2") = 1.2;
            g("node 2", "node 3") = 2.3;
            g("node 1", "node 3") = 1.3;
            g("node 4", "node 5") = 4.5;
            g.set_nature(DIRECTED);
            CHECK(g.get_nature() == DIRECTED);
            CHECK(g.get_nbr_edges() == 8);
            CHECK(g("node 3", "node 1") == 1.3);
            g("node 2", "node 1") = 2.1;
            CHECK(g("node 2", "node 1") == 2.1);
            CHECK(g("node 1", "node 2") == 1.2);
        }
    }
    
    SECTION("Degree") {
        Graph g1(DIRECTED);
        g1["n"];
        CHECK_THROWS_WITH(g1.degree("n").get_degree_undirected(),
                          "[graph.exception.invalid_argument] Invalid usage of graph::Degree: wrong nature given when calling 'get_degree_undirected'.");
        CHECK(g1.degree("n").get_degree_directed() == make_pair<size_t, size_t>(0, 0));
        CHECK(g1.degree("n") == make_pair(0, 0));
    
        Graph g2(UNDIRECTED);
        g2["n"];
        CHECK_THROWS_WITH(g2.degree("n").get_degree_directed(),
                          "[graph.exception.invalid_argument] Invalid usage of graph::Degree: wrong nature given when calling 'get_degree_directed'.");
        CHECK(g2.degree("n").get_degree_undirected() == 0);
        CHECK(g2.degree("n") == 0);
        
        CHECK_FALSE(g1.degree("n") == g2.degree("n"));
        CHECK_THROWS_WITH(Graph::Degree(5) < Graph::Degree(1, 2),
                          "[graph.exception.invalid_argument] Invalid usage of graph::Degree: wrong nature given when calling 'operator<'.");
    }
    
    SECTION("degree(const_iterator position") {
        SECTION("directed") {
            graph<string, int, double> g(DIRECTED);
            graph<string, int, double>::iterator it_n1{g.add_node("node 1", 1).first};
            graph<string, int, double>::iterator it_n2{g.add_node("node 2", 2).first};
            graph<string, int, double>::iterator it_n3{g.add_node("node 3", 3).first};
            graph<string, int, double>::iterator it_n4{g.add_node("node 4", 4).first};
            g(it_n1, it_n2);
            g(it_n1, it_n3);
            g(it_n4, it_n1);
            CHECK(g.degree(it_n1) == make_pair(1, 2));
            
            g.del_edge(it_n4, it_n1);
            CHECK(g.degree(it_n1) == make_pair(0, 2));
            g.del_edge(it_n1, it_n2);
            CHECK(g.degree(it_n1) == make_pair(0, 1));
            g.del_edge(it_n1, it_n2);
            CHECK(g.degree(it_n1) == make_pair(0, 1));
            
            graph<string, int, double>::iterator it[6];
            for (int i{0}; i < 6; ++i)
                it[i] = g.add_node(std::to_string(i)).first;
            
            g(it[0], it[1]) = 01;
            g(it[0], it[2]) = 02;
            g(it[0], it[3]) = 03;
            g(it[0], it[4]) = 04;
            g(it[3], it[0]) = 30;
            g(it[4], it[0]) = 40;
            g(it[5], it[0]) = 50;
            it[3]->second->clear_edges();
            CHECK(g.degree(it[0]) == make_pair(2, 4));
            
            g.erase(it[4]);
            CHECK(g.degree(it[0]) == make_pair<int, int>(1, 3));
        }
    
        SECTION("undirected") {
            graph<string, int, double> g(UNDIRECTED);
            graph<string, int, double>::iterator it_n1{g.add_node("node 1", 1).first};
            graph<string, int, double>::iterator it_n2{g.add_node("node 2", 2).first};
            graph<string, int, double>::iterator it_n3{g.add_node("node 3", 3).first};
            graph<string, int, double>::iterator it_n4{g.add_node("node 4", 4).first};
            g(it_n1, it_n2);
            g(it_n1, it_n3);
            g(it_n4, it_n1);
            CHECK(g.degree(it_n1) == 3);
    
            g.del_edge(it_n4, it_n1);
            CHECK(g.degree(it_n1) == 2);
            g.del_edge(it_n1, it_n2);
            CHECK(g.degree(it_n1) == 1);
            g.del_edge(it_n1, it_n2);
            CHECK(g.degree(it_n1) == 1);
            g.del_edge(it_n3, it_n1);
            CHECK(g.degree(it_n1) == 0);
    
            graph<string, int, double>::iterator it[6];
            for (int i{0}; i < 6; ++i)
                it[i] = g.add_node(std::to_string(i)).first;
    
            g(it[0], it[1]) = 01;
            g(it[0], it[2]) = 02;
            g(it[0], it[3]) = 03;
            g(it[0], it[4]) = 04;
            g(it[3], it[0]) = 30;
            g(it[4], it[0]) = 40;
            g(it[5], it[0]) = 50;
            CHECK(g.degree(it[0]) == 5);
            it[3]->second->clear_edges();
            CHECK_FALSE(g.existing_edge(it[3], it[0]));
            CHECK_FALSE(g.existing_edge(it[0], it[3]));
            CHECK(g.degree(it[0]) == 4);
    
            g.erase(it[4]);
            CHECK(g.degree(it[0]) == 3);
        }
    }
    
    SECTION("degree(const Key& key)") {
        SECTION("directed") {
            graph<string, int, double> g(DIRECTED);
            g("node 1", "node 2");
            g("node 1", "node 3");
            g("node 4", "node 1");
            CHECK(g.degree(g.find("node 1")) == make_pair(1, 2));
        
            g.del_edge("node 4", "node 1");
            CHECK(g.degree(g.find("node 1")) == make_pair(0, 2));
            g.del_edge("node 1", "node 2");
            CHECK(g.degree(g.find("node 1")) == make_pair(0, 1));
            g.del_edge("node 1", "node 2");
            CHECK(g.degree(g.find("node 1")) == make_pair(0, 1));
        
            g("1", "2") = 12;
            g("1", "3") = 13;
            g("1", "4") = 14;
            g("1", "5") = 15;
            g("4", "1") = 41;
            g("5", "1") = 51;
            g("6", "1") = 61;
            g.find("4")->second->clear_edges();
            CHECK(g.degree(g.find("1")) == make_pair(2, 4));
        
            g.erase("5");
            CHECK(g.degree(g.find("1")) == make_pair<int, int>(1, 3));
        }
    
        SECTION("undirected") {
            graph<string, int, double> g(UNDIRECTED);
            g("node 1", "node 2");
            g("node 1", "node 3");
            g("node 4", "node 1");
            CHECK(g.degree(g.find("node 1")) == 3);
        
            g.del_edge("node 4", "node 1");
            CHECK(g.degree("node 1") == 2);
            g.del_edge("node 1", "node 2");
            CHECK(g.degree("node 1") == 1);
            g.del_edge("node 1", "node 2");
            CHECK(g.degree("node 1") == 1);
            g.del_edge("node 3", "node 1");
            CHECK(g.degree("node 1") == 0);
        
            g("1", "2") = 12;
            g("1", "3") = 13;
            g("1", "4") = 14;
            g("1", "5") = 15;
            g("4", "1") = 41;
            g("5", "1") = 51;
            g("6", "1") = 61;
            CHECK(g.degree(g.find("1")) == 5);
            g.find("4")->second->clear_edges();
            CHECK_FALSE(g.existing_edge("4", "1"));
            CHECK_FALSE(g.existing_edge("1", "4"));
            CHECK(g.degree("1") == 4);
        
            g.erase("5");
            CHECK(g.degree("1") == 3);
        }
    }
    
    SECTION("degree_max()") {
        SECTION("directed") {
            graph<int, int> g(DIRECTED);
            CHECK(g.degree_max().first == g.cend());
            CHECK(g.degree_max().second == make_pair(0, 0));
            
            g(1, 2);
            g(1, 3);
            g(4, 5);
            g(4, 6);
            g(7, 4);
            pair<typename graph<int, int>::const_iterator, typename graph<int, int>::Degree> deg_max{g.degree_max()};
            CHECK(deg_max.first == g.find(4));
            CHECK(deg_max.second == make_pair(1, 2));
        }
    
        SECTION("undirected") {
            graph<int, int> g(UNDIRECTED);
            CHECK(g.degree_max().first == g.cend());
            CHECK(g.degree_max().second == 0);
        
            g(1, 2);
            g(1, 3);
            g(4, 5);
            g(4, 6);
            g(7, 4);
            pair<typename graph<int, int>::const_iterator, typename graph<int, int>::Degree> deg_max{g.degree_max()};
            CHECK(deg_max.first == g.find(4));
            CHECK(deg_max.second == 3);
        }
    }
    
    SECTION("degree_min()") {
        SECTION("directed") {
            graph<int, int> g(DIRECTED);
            CHECK(g.degree_min().first == g.cend());
            CHECK(g.degree_min().second == make_pair(0, 0));
        
            g(1, 2);
            g(1, 3);
            g(4, 5);
            g(4, 6);
            g(7, 4);
            pair<typename graph<int, int>::const_iterator, typename graph<int, int>::Degree> deg_min{g.degree_min()};
            CHECK(deg_min.first == g.find(7));
            CHECK(deg_min.second == make_pair(0, 1));
        }
    
        SECTION("undirected") {
            graph<int, int> g(UNDIRECTED);
            CHECK(g.degree_min().first == g.cend());
            CHECK(g.degree_min().second == 0);
        
            g(1, 2);
            g(1, 3);
            g(4, 5);
            g(4, 6);
            g(7, 4);
            pair<typename graph<int, int>::const_iterator, typename graph<int, int>::Degree> deg_min{g.degree_min()};
            CHECK(deg_min.first == g.find(2));
            CHECK(deg_min.second == 1);
        }
    }
    
    SECTION("degrees()") {
        SECTION("directed") {
            graph<int, int> g(DIRECTED);
            map<int, typename graph<int, int>::Degree> m1{g.degrees()};
            CHECK(m1.empty());
            
            g(1, 2);
            g(1, 3);
            g(4, 5);
            g(4, 6);
            g(7, 4);
            map<int, typename graph<int, int>::Degree> m2{g.degrees()};
            CHECK(m2.size() == 7);
            CHECK(m2.find(1)->second == graph<int, int>::Degree(0, 2));
            CHECK(m2.find(2)->second == graph<int, int>::Degree(1, 0));
            CHECK(m2.find(3)->second == graph<int, int>::Degree(1, 0));
            CHECK(m2.find(4)->second == graph<int, int>::Degree(1, 2));
            CHECK(m2.find(5)->second == graph<int, int>::Degree(1, 0));
            CHECK(m2.find(6)->second == graph<int, int>::Degree(1, 0));
            CHECK(m2.find(7)->second == graph<int, int>::Degree(0, 1));
        }
    
        SECTION("undirected") {
            graph<int, int> g(UNDIRECTED);
            map<int, typename graph<int, int>::Degree> m1{g.degrees()};
            CHECK(m1.empty());
    
            g(1, 2);
            g(1, 3);
            g(4, 5);
            g(4, 6);
            g(7, 4);
            map<int, typename graph<int, int>::Degree> m2{g.degrees()};
            CHECK(m2.size() == 7);
            CHECK(m2.find(1)->second == 2);
            CHECK(m2.find(2)->second == 1);
            CHECK(m2.find(3)->second == 1);
            CHECK(m2.find(4)->second == 3);
            CHECK(m2.find(5)->second == 1);
            CHECK(m2.find(6)->second == 1);
            CHECK(m2.find(7)->second == 1);
        }
    }
}
