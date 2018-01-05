//
// Created by Terae on 01/08/17.
//

#include "catch.hpp"

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wkeyword-macro"
#endif
#define private public
#define protected public
	#if defined(TEST_SINGLE_HEADER_FILE)
		#include "graph.hpp"
	#else
		#include "Graph.h"
    #endif
#undef private
#undef protected
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif

template <class T, class C = std::size_t>
using node = typename graph<std::string, T, C>::node;
template <class T, class C = std::size_t>
using Graph = graph<std::string, T, C>;

TEST_CASE("nodes") {
    using namespace std;
    
    SECTION("constructors") {
        SECTION("default constructor") {
            node<int> n1;
            CHECK(n1.get() == 0);
            CHECK(n1.degree().first == 0);
            CHECK(n1.degree().second == 0);
            CHECK(n1 == node<int>(0));
    
            node<int> n2(80);
            CHECK(n2.get() == 80);
            CHECK(n2.get_edges().empty());
            
            node<int> n3(42.56);
            CHECK(n3.get() == 42);
            
            node<int> n4('a');
            CHECK(n4.get() == 97);
            
            node<char> n5(50);
            CHECK(n5.get() == '2');
        }
    
        SECTION("edges") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(5)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(10)).first};
            std::shared_ptr<node<int, int>> ptr1{it1->second};
            std::shared_ptr<node<int, int>> ptr2{it2->second};
            ptr1->add_edge(it2, 15);
            CHECK(ptr1->get_edges().begin()->target.lock()->get() == 10);
            ptr2->add_edge(it2, 1);
            CHECK(ptr1->get() == 5);
        }
    
        SECTION("nodes") {
            Graph<int, int> g;
            Graph<int, int>::iterator it{g.emplace("node", node<int, int>(12)).first};
            
            node<int, int> n(5);
            n.add_edge(it, 444);
            CHECK(n.get_edges().size() == 1);
        }
        
        SECTION("copy constructor") {
            node<double> n1(3.14159);
            node<double> n2(n1);
            CHECK(n2.get() == 3.14159);
            CHECK(n1 == n2);
            
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
            Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
    
            node<int, int> n3(5);
    
            n3.add_edge(it2, 222);
            n3.add_edge(it1, 111);
            n3.add_edge(it3, 333);
            CHECK(n3.degree().second == 3);
            
            node<int, int> n4(n3);
            CHECK(n4.degree() == n3.degree());
            CHECK(n4.get_edges().size() == 3);
            CHECK(n4.get_edges().begin()->target.lock() == it2->second);
        }

        SECTION("assignment operator") {
            node<double> n1(3.14159);
            node<double> n2 = n1;
            CHECK(n2.get() == 3.14159);
            CHECK(n1 == n2);
    
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
            Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
            
            node<int, int> n3(5);
    
            n3.add_edge(it2, 222);
            n3.add_edge(it1, 111);
            n3.add_edge(it3, 333);
            CHECK(n3.degree().second == 3);
    
            node<int, int> n4 = n3;
            CHECK(n4.degree() == n3.degree());
            CHECK(n4.get_edges().begin()->target.lock() == it2->second);
            CHECK(n4.get_edges().size() == 3);
        }
    }
    
    SECTION("element access") {
        SECTION("get()") {
            node<string> n("node");
            CHECK(n.get() == "node");
            
            n.get() = "new name";
            CHECK(n.get() == "new name");
        }
        
        SECTION("get() const") {
            const node<double> n(3.14);
            CHECK(n.get() == 3.14);
        }
        
        SECTION("get_cost(Container other)") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(40)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(80)).first};
            
            node<int, int> n;
            CHECK_THROWS_WITH(it1->second->get_cost(g.end()),
                              "[graph.exception.unexpected_nullptr] Unexpected nullptr when calling 'get_cost'.");
            
            n.add_edge(it1, 15);
            CHECK(n.get_cost(it1) == 15);
            CHECK(n.get_cost(it2) == numeric_limits<int>::max());
            CHECK(n.existing_adjacent_node(it2));
            
            n.get_cost(it2) = 20;
            CHECK(n.get_cost(it2) == 20);
        }
        
        SECTION("get_cost(constContainer other) const") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(40)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(80)).first};
            
            node<int, int> n;
            n.add_edge(it1, 15);
            const node<int, int> copy(n);
            CHECK_THROWS_WITH(it1->second->get_cost(g.end()),
                              "[graph.exception.unexpected_nullptr] Unexpected nullptr when calling 'get_cost'.");
            
            CHECK(copy.get_cost(it1) == 15);
            CHECK(copy.get_cost(it2) == numeric_limits<int>::max());
        }
    
        SECTION("operator[](Container other)") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(40)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(80)).first};
            
            node<int, int> n;
            CHECK_THROWS_WITH(it1->second->get_cost(g.end()),
                              "[graph.exception.unexpected_nullptr] Unexpected nullptr when calling 'get_cost'.");
        
            n[it1] = 15;
            CHECK(n[it1] == 15);
            CHECK(n[it2] == numeric_limits<int>::max());
            CHECK(n.existing_adjacent_node(it2));
        
            n[it2] = 20;
            CHECK(n[it2] == 20);
        }
    
        SECTION("operator[](constContainer other) const") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(40)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(80)).first};
    
            node<int, int> n;
            n[it1] = 15;
            const node<int, int> copy(n);
            CHECK_THROWS_WITH((*it1->second)[g.end()],
                              "[graph.exception.unexpected_nullptr] Unexpected nullptr when calling 'get_cost'.");
    
            CHECK(copy[it1] == 15);
            CHECK(copy[it2] == numeric_limits<int>::max());
        }
        
        SECTION("get_edges()") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
            Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
            Graph<int, int>::iterator it4{g.emplace("node 4", node<int, int>(44)).first};
            Graph<int, int>::iterator it5{g.emplace("node 5", node<int, int>(55)).first};
    
            CHECK(it1->second->degree().first  == 0);
            CHECK(it1->second->degree().second == 0);
    
            it1->second->add_edge(it2, 2);
            it1->second->add_edge(it3, 3);
            it1->second->add_edge(it4, 4);
            it5->second->add_edge(it1, 1);
            
            auto edges = it1->second->get_edges();
            for(int i = 2; i < 4; ++i) {
                CHECK(*next(edges.begin(), i-2)->cost == i);
                CHECK(next(edges.begin(), i-2)->target.lock()->get() == i * 11);
            }
            CHECK(it2->second->get_edges().empty());
        }
        
        SECTION("tie()") {
            Graph<int, int> g;
            Graph<int, int>::iterator list_it[5];
            
            node<int, int> n(42);
            for(int i{0}; i < 5; ++i) {
                list_it[i] = g.emplace("node " + std::to_string(i), node<int, int>(i)).first;
                n[list_it[i]] = i * 10;
            }
            tuple<int, size_t, list<node<int, int>::edge>> t = n.tie();
            CHECK(get<0>(t) == 42);
            CHECK(get<1>(t) == 0);
            int i{0};
            for(auto it : get<2>(t)) {
                //CHECK(static_cast<Graph<int, int>::node>(it) == *list_it[i]->second);
                CHECK(list_it[i]->second->degree().first == 1);
                ++i;
            }
        }
    }
    
    SECTION("modifiers") {
        SECTION("set(const Data& d)") {
            node<string> n1("a");
            n1.set("b");
            CHECK(n1.get() == "b");
            
            node<int> n2(5);
            n2.set(6.3);
            CHECK(n2.get() == 6);
            
            node<string> n3("abc");
            const char* s("abcd");
            n3.set(s);
            CHECK(n3.get() == "abcd");
            
            //struct T {
            //    T(int x, float y) : a(x), b(y) {}
            //    int a;
            //    float b;
            //};
            //node<T> n4(T(5, 3.14159));
            //T t(1, 11.11);
            //n4.set(t);
            //CHECK(n4.get().a == t.a);
            //CHECK(n4.get().b == t.b);
        }
        
        SECTION("operator=(const Data& d)") {
            node<string> n1("a");
            n1 = "b";
            CHECK(n1.get() == "b");
            
            node<int> n2(5);
            n2 = 6.3;
            CHECK(n2.get() == 6);
    
            node<string> n3("abc");
            const char* s("abcd");
            n3 = s;
            CHECK(n3.get() == "abcd");
        }
    
        SECTION("add_edge(shared_ptr<node> other, Cost)") {
            Graph<int, int> g;
            Graph<int, int>::iterator it{g.emplace("node", node<int, int>(11)).first};
        
            node<int, int> n(42);
            n.add_edge(it, 111);
            CHECK(*n.get_edges().begin()->cost == 111);
            CHECK(get<1>(n.get_edges().begin()->tie()).tie() == it->second->tie());

            CHECK_THROWS_WITH(it->second->add_edge(g.end(), 1),
                              "[graph.exception.unexpected_nullptr] Unexpected nullptr when calling 'add_edge'.");
        }
    
        SECTION("set_cost(const shared_ptr<node<Data, Cost>> &other, const T_cost& c)") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(15)).first};
    
            node<int, int> n(42);
            n.add_edge(it1, 111);
            CHECK(*n.get_edges().begin()->cost == 111);
            n.set_cost(it1, 1.1);
            CHECK(*n.get_edges().begin()->cost == 1);

            CHECK_THROWS_WITH(it1->second->set_cost(g.end(), 1),
                              "[graph.exception.unexpected_nullptr] Unexpected nullptr when calling 'set_cost'.");
    
            n.set_cost(it2, 1.5);
            CHECK(n.get_edges().size() == 2);
            CHECK(*(++n.get_edges().begin())->cost == 1);
        }
        
        SECTION("set_edge(shared_ptr<node<Data, Cost>> other, shared_ptr<Cost> cost)") {
            Graph<int, string> g;
            Graph<int, string>::iterator it{g.emplace("node", node<int, string>(11)).first};
            shared_ptr<string> cost{make_shared<string>("edge")};
            
            node<int, string> n(42);
            CHECK(n.set_edge(it, cost));
            CHECK_FALSE(n.set_edge(it, cost));
            CHECK(n.get_edges().begin()->cost == cost);
            cost->push_back('s');
            CHECK(*n.get_edges().begin()->cost == "edges");
        }
        
        SECTION("deleters") {
            SECTION("del_edge(const shared_ptr<node> other)") {
                Graph<int, int> g;
                Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
                Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
                Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
                Graph<int, int>::iterator it4{g.emplace("node 4", node<int, int>(44)).first};
    
                node<int, int> n(42);
                n.add_edge(it1, 111);
                n.add_edge(it2, 222);
                n.add_edge(it3, 333);
                it2->second->add_edge(it4, 444);
                
                n.del_edge(it2);
                CHECK(it2 == g.find("node 2"));
                CHECK(n.get_edges().size() == 2);
                CHECK_FALSE(n.existing_adjacent_node(it2));
                CHECK(it2->second->get_edges().size() == 1);
            }
    
            SECTION("del_edge_if(const shared_ptr<node> other, function<bool(edge)>)") {
                Graph<int, int> g;
                Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
                Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
                Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
                Graph<int, int>::iterator it4{g.emplace("node 4", node<int, int>(44)).first};
    
                node<int, int> n(42);
                n.add_edge(it1, 111);
                n.add_edge(it2, 222);
                n.add_edge(it3, 333);
                it2->second->add_edge(it4, 444);

                function<bool(node<int, int>::edge)> predicate = [](node<int, int>::edge e) -> bool { return *e.cost % 2 == 1; };
                n.del_edge_if(it2, predicate);
                CHECK(n.existing_adjacent_node(it2));
    
                predicate = [](node<int, int>::edge e) -> bool { return *e.cost % 2 == 0; };
                n.del_edge_if(it2, predicate);
                CHECK_FALSE(n.existing_adjacent_node(it2));
            }
            
            SECTION("clear_edges()") {
                Graph<int, int> g;
                Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
                Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
                Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
                Graph<int, int>::iterator it4{g.emplace("node 4", node<int, int>(44)).first};
    
                node<int, int> n(42);
                n.add_edge(it1, 111);
                n.add_edge(it2, 222);
                n.add_edge(it3, 333);
                it2->second->add_edge(it4, 444);
                
                n.clear_edges();
                CHECK(n.get_edges().empty());
                CHECK(it2->second->existing_adjacent_node(it4));
            }
        }
    }
    
    SECTION("operations") {
        SECTION("degree()") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
            Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
            Graph<int, int>::iterator it4{g.emplace("node 4", node<int, int>(44)).first};
            Graph<int, int>::iterator it5{g.emplace("node 5", node<int, int>(55)).first};
            CHECK(it1->second->degree() == make_pair<std::size_t, std::size_t>(0, 0));
            
            it1->second->add_edge(it2, 2);
            it1->second->add_edge(it3, 3);
            it1->second->add_edge(it4, 4);
            it5->second->add_edge(it1, 1);
            CHECK(it1->second->degree() == make_pair<std::size_t, std::size_t>(1, 3));
            
            it1->second->add_edge(it2, 5);
            it5->second->add_edge(it1, 5);
            CHECK(it1->second->degree() == make_pair<std::size_t, std::size_t>(1, 3));
            
            
            it1->second->del_edge(it2);
            it5->second->del_edge(it1);
            CHECK(it1->second->degree() == make_pair<std::size_t, std::size_t>(0, 2));
    
            it1->second->del_edge(it2);
            it5->second->del_edge(it1);
            CHECK(it1->second->degree() == make_pair<std::size_t, std::size_t>(0, 2));
        }
        
        SECTION("existing_adjacent_node(const shared_ptr<node> other)") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
            Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
            Graph<int, int>::iterator it4{g.emplace("node 4", node<int, int>(44)).first};
            Graph<int, int>::iterator it5{g.emplace("node 5", node<int, int>(55)).first};
    
            it1->second->add_edge(it2, 2);
            it1->second->add_edge(it3, 3);
            it1->second->add_edge(it4, 4);
            it5->second->add_edge(it1, 1);
            
            CHECK_FALSE(it1->second->existing_adjacent_node(g.end()));
            
            CHECK(it1->second->existing_adjacent_node(it2));
            CHECK(it1->second->existing_adjacent_node(it4));
            CHECK(it5->second->existing_adjacent_node(it1));
            
            CHECK_FALSE(it2->second->existing_adjacent_node(it3));
            CHECK_FALSE(it3->second->existing_adjacent_node(it1));
        }
        
        SECTION("degree() const") {
            Graph<int, int> g;
            Graph<int, int>::iterator list_it[5];
    
            node<int, int> n(42);
            for(int i{0}; i < 5; ++i)
                list_it[i] = g.emplace("node " + std::to_string(i), node<int, int>(i)).first;
            
            CHECK((n.degree() == make_pair<size_t, size_t>(0, 0)));
            
            for(int i{0}; i < 5; ++i)
                n[list_it[i]] = i * 10;
            CHECK((n.degree() == make_pair<size_t, size_t>(0, 5)));
            
            list_it[0]->second->add_edge(list_it[1]);
            CHECK((list_it[0]->second->degree() == make_pair<size_t, size_t>(1, 1)));
        }
    }
    
    SECTION("bool operators") {
        SECTION("operator==") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
            Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
    
            node<int, int> n1(42);
            n1.add_edge(it1, 111);
            n1.add_edge(it2, 222);
            n1.add_edge(it3, 333);
    
            node<int, int> n2 = n1;
            CHECK(n1 == n2);
            
            it1->second->get() = 12;
            CHECK(n1 == n2);
            
            node<int, int> n3(42);
            n3.add_edge(it1, 111);
            n3.add_edge(it2, 222);
            n3.add_edge(it3, 333);
            CHECK(n1 == n3);
        }
        
        SECTION("operator!=") {
            Graph<int, int> g;
            Graph<int, int>::iterator it1{g.emplace("node 1", node<int, int>(11)).first};
            Graph<int, int>::iterator it2{g.emplace("node 2", node<int, int>(22)).first};
            Graph<int, int>::iterator it3{g.emplace("node 3", node<int, int>(33)).first};
            Graph<int, int>::iterator it4{g.emplace("node 4", node<int, int>(44)).first};
    
            node<int, int> n1(42);
            n1.add_edge(it1, 111);
            n1.add_edge(it2, 222);
            n1.add_edge(it3, 333);
    
            node<int, int> n2 = n1;
            n1.add_edge(it4, 444);
            CHECK(n1 != n2);
    
            node<int, int> n3(40);
            n3.add_edge(it1, 111);
            n3.add_edge(it2, 222);
            n3.add_edge(it3, 333);
            CHECK(n1 != n3);
        }
        
        SECTION("operator<") {
            Graph<string, int> g;
            node<string, int> n1("abcdefGhijkl");
            node<string, int> n2("abcdefZhijkl");
            
            CHECK(n1 < n2);
            
            n2.get() = n1.get();
            
            CHECK_FALSE(n1 < n2);
            n2 = "abcdefGhijkl";
            
            Graph<string, int>::iterator it1{g.emplace("node 1", node<string, int>("it1")).first};
            n2.add_edge(it1, 10);
            CHECK(n1 < n2);
    
            Graph<string, int>::iterator it2{g.emplace("node 2", node<string, int>("it2")).first};
            n1.add_edge(it2, 10);
            CHECK(n2 < n1);
            
            node<string, int> n3("test node");
            node<string, int> n4("test node");
            n3.add_edge(it1, 3);
            n4.add_edge(it1, 4);
            CHECK(n3 < n4);
            
            n4[it1] = 3;
            Graph<string, int>::iterator it3{g.emplace("node 3", node<string, int>("it3")).first};
            n4.add_edge(it3);
            CHECK(n3 < n4);
        }
    
        SECTION("operator<=") {
            Graph<string, int> g;
            node<string, int> n1("abcdefghijkl");
            node<string, int> n2("abcdefzhijkl");
    
            CHECK(n1 <= n2);
    
            n2.get() = n1.get();
    
            CHECK(n1 <= n2);
    
            Graph<string, int>::iterator it1{g.emplace("node 1", node<string, int>("it1")).first};
            n2.add_edge(it1, 10);
            CHECK(n1 <= n2);
    
            Graph<string, int>::iterator it2{g.emplace("node 2", node<string, int>("it2")).first};
            n1.add_edge(it2, 10);
            CHECK(n2 <= n1);
    
            node<string, int> n3("test node");
            node<string, int> n4("test node");
            n3.add_edge(it1, 3);
            n4.add_edge(it1, 4);
            CHECK(n3 <= n4);
        }
    
        SECTION("operator>") {
            Graph<string, int> g;
            node<string, int> n1("abcdefzhijkl");
            node<string, int> n2("abcdefghijkl");
    
            CHECK(n1 > n2);
    
            n1.set(n2.get());
    
            CHECK_FALSE(n1 > n2);
    
            Graph<string, int>::iterator it1{g.emplace("node 1", node<string, int>("it1")).first};
            n1.add_edge(it1, 10);
            CHECK(n1 > n2);
    
            Graph<string, int>::iterator it2{g.emplace("node 2", node<string, int>("it2")).first};
            n2.add_edge(it2, 10);
            CHECK(n2 > n1);
    
            node<string, int> n3("test node");
            node<string, int> n4("test node");
            n3.add_edge(it1, 3);
            n4.add_edge(it1, 4);
            CHECK(n4 > n3);
        }
    
        SECTION("operator>=") {
            Graph<string, int> g;
            node<string, int> n1("abcdefZhijkl");
            node<string, int> n2("abcdefGhijkl");
    
            CHECK(n1 >= n2);
    
            n1.set(n2.get());
            
            CHECK(n1 >= n2);
            
            Graph<string, int>::iterator it1{g.emplace("node 1", node<string, int>("it1")).first};
            n1.add_edge(it1, 10);
            CHECK(n1 >= n2);
    
            Graph<string, int>::iterator it2{g.emplace("node 2", node<string, int>("it2")).first};
            n2.add_edge(it2, 10);
            CHECK(n2 >= n1);
    
            node<string, int> n3("test node");
            node<string, int> n4("test node");
            n3.add_edge(it1, 3);
            n4.add_edge(it1, 4);
            CHECK(n4 >= n3);
        }
    }
}
