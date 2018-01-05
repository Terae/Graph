//
// Created by Terae on 01/08/17.
//

#include "catch.hpp"
#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

TEST_CASE("bool operators") {
    using namespace std;
    using Graph = graph<string, int, size_t>;
    
    SECTION("operator ==") {
        Graph g1(DIRECTED);
        Graph g2(DIRECTED);
        graph<string, float, double> g3(DIRECTED);
        for(int i = 0; i < 100; ++i) {
            g1["node " + to_string(i)] = i;
            g2["node " + to_string(i)] = i;
            g3["node " + to_string(i)] = i;
        }
    
        for(size_t i{0}; i < 98; i += 3) {
            g1("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
            g1("node " + to_string(i+2), "node " + to_string(i+1)) = 40 * i;
    
            g2("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
            g2("node " + to_string(i+2), "node " + to_string(i+1)) = 40 * i;
    
            g3("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
            g3("node " + to_string(i+2), "node " + to_string(i+1)) = 40 * i;
        }
    
        CHECK(g1 == g2);
        CHECK(g1 == g3);
    }
    
    SECTION("operator !=") {
        Graph g1(DIRECTED);
        Graph g2(UNDIRECTED);
        
        CHECK(g1 != g2); // Different nature
        
        Graph g3(DIRECTED);
        Graph g4(DIRECTED);
        Graph g5(DIRECTED);
        Graph g6(DIRECTED);
        
        for(int i = 0; i < 100; ++i) {
            g1["node " + to_string(i)] = i;
            g3["node " + to_string(i)] = i;
            g4["node " + to_string(i)] = i;
            g5["node " + to_string(i)] = i;
            g6["node " + to_string(i)] = i;
        }
        
        g3["node 4"] = 44;
        CHECK(g1 != g3); // "node 4" has a different value
        
        g4["node 101"] = 101;
        CHECK(g1 != g4); // One more node
    
        for(size_t i{0}; i < 98; i += 3) {
            g1("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
            g1("node " + to_string(i+2), "node " + to_string(i+1)) = 40 * i;
            
            g5("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
            g5("node " + to_string(i+2), "node " + to_string(i+1)) = 40 * i;
    
            g6("node " + to_string(i), "node " + to_string(i + 1)) = 20 * i;
            g6("node " + to_string(i+2), "node " + to_string(i+1)) = 40 * i;
        }
        g5("node 52", "node 12") = 42;
        CHECK(g1 != g5); // One more edge
        
        g1("node 52", "node 12") = 51;
        g6("node 52", "node 12") = 52;
        CHECK(g1 != g6); // Different value on an edge
        
        g6("node 52", "node 12") = 51;
        g1("node 52", "node 13") = 23;
        g6("node 52", "node 14") = 23;
        CHECK(g1 != g6); // Different output node
    
        graph<string, int> g7(UNDIRECTED);
        graph<string, double> g8(UNDIRECTED);
        g7["node"] = 1;
        g8["node"] = 1.2;
        CHECK(g7 != g8); // Different value despite having two different types
    }
}
