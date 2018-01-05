//
// Created by Terae on 01/08/17.
//

#include "catch.hpp"

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wkeyword-macro"
#endif
#define private public // access to _nodes attribute
	#if defined(TEST_SINGLE_HEADER_FILE)
    	#include "graph.hpp"
	#else
    	#include "Graph.h"
	#endif
#undef private
#if defined(__clang)
    #pragma clang diagnostic pop
#endif

TEST_CASE("iterators") {
    using namespace std;
    
    using Graph      = graph<string, int>;
    using iterator   = Graph::iterator;
    using citerator  = Graph::const_iterator;
    using riterator  = Graph::reverse_iterator;
    using criterator = Graph::const_reverse_iterator;
    
    SECTION("basic behavior") {
        SECTION("null") {
            Graph g;
            Graph g_const(g);
        
            SECTION("graph + begin/end") {
                iterator it = g.begin();
                CHECK(it == g.end());
            }
            
            SECTION("const graph + begin/end") {
                citerator cit_begin = g_const.begin();
                citerator cit_end = g_const.end();
                CHECK(cit_begin == cit_end);
            }
        
            SECTION("graph + cbegin/cend") {
                citerator cit_begin = g.cbegin();
                citerator cit_end = g.cend();
                CHECK(cit_begin == cit_end);
            }
    
            SECTION("const graph + cbegin/cend") {
                citerator cit_begin = g_const.cbegin();
                citerator cit_end = g_const.cend();
                CHECK(cit_begin == cit_end);
            }
        
            SECTION("graph + rbegin/rend") {
                riterator rit = g.rbegin();
                CHECK(rit == g.rend());
            }
    
            SECTION("const graph + rbegin/rend") {
                criterator crit = g_const.rbegin();
                CHECK(crit == g_const.rend());
            }
        
            SECTION("graph + crbegin/crend") {
                criterator crit = g.crbegin();
                CHECK(crit == g.crend());
            }
    
            SECTION("const graph + crbegin/crend") {
                criterator crit = g_const.crbegin();
                CHECK(crit == g_const.crend());
            }
        }
    
        SECTION("initialized") {
            Graph g;
            g["node 1"] = 11;
            g["node 2"] = 22;
            Graph g_const(g);

            SECTION("graph + begin/end") {
                iterator it = g.begin();
                CHECK(it != g.end());
                CHECK(it->first == "node 1");
            
                ++it;
                CHECK(it != g.begin());
                CHECK(it == --g.end());
                CHECK(it->second.get()->get() == 22);
            
                --it;
                CHECK(it == g.begin());
                CHECK(it != g.end());
            
                ++it;
                CHECK(it != g.begin());
                CHECK(it == --g.end());
            
                --it;
                CHECK(it == g.begin());
                CHECK(it != g.end());
            }
            
            SECTION("const graph + begin/end") {        
                citerator cit = g_const.begin();
                CHECK(cit != g_const.end());
                CHECK(cit->first == "node 1");
        
                cit++;
                CHECK(cit != g_const.begin());
                CHECK(cit == --g_const.end());
                CHECK(cit->second.get()->get() == 22);
        
                cit--;
                CHECK(cit == g_const.begin());
                CHECK(cit != g_const.end());
        
                ++cit;
                CHECK(cit != g_const.begin());
                CHECK(cit == --g_const.end());
        
                --cit;
                CHECK(cit == g_const.begin());
                CHECK(cit != g_const.end());
            }
        
            SECTION("graph + cbegin/cend") {
                citerator cit = g.cbegin();
                CHECK(cit != g.cend());
                CHECK(cit->first == "node 1");
            
                cit++;
                CHECK(cit != g.cbegin());
                CHECK(cit == --g.cend());
                CHECK(cit->second.get()->get() == 22);
            
                cit--;
                CHECK(cit == g.cbegin());
                CHECK(cit != g.cend());
            
                ++cit;
                CHECK(cit != g.cbegin());
                CHECK(cit == --g.cend());
            
                --cit;
                CHECK(cit == g.cbegin());
                CHECK(cit != g.cend());
            }
    
            SECTION("const graph + cbegin/cend") {
                citerator cit = g_const.cbegin();
                CHECK(cit != g_const.cend());
                CHECK(cit->first == "node 1");
        
                cit++;
                CHECK(cit != g_const.cbegin());
                CHECK(cit == --g_const.cend());
                CHECK(cit->second.get()->get() == 22);
        
                cit--;
                CHECK(cit == g_const.cbegin());
                CHECK(cit != g_const.cend());
        
                ++cit;
                CHECK(cit != g_const.cbegin());
                CHECK(cit == --g_const.cend());
        
                --cit;
                CHECK(cit == g_const.cbegin());
                CHECK(cit != g_const.cend());
            }
        
            SECTION("graph + rbegin/rend") {
                riterator rit = g.rbegin();
                CHECK(rit != g.rend());
                CHECK(rit->first == "node 2");
            
                rit++;
                CHECK(rit != g.rbegin());
                CHECK(rit == --g.rend());
                CHECK(rit->second.get()->get() == 11);
            
                rit--;
                CHECK(rit == g.rbegin());
                CHECK(rit != g.rend());
            
                ++rit;
                CHECK(rit != g.rbegin());
                CHECK(rit == --g.rend());
            
                --rit;
                CHECK(rit == g.rbegin());
                CHECK(rit != g.rend());
            }
    
            SECTION("const graph + rbegin/rend") {
                riterator rit = g_const.rbegin();
                CHECK(rit != g_const.rend());
                CHECK(rit->first == "node 2");
        
                rit++;
                CHECK(rit != g_const.rbegin());
                CHECK(rit == --g_const.rend());
                CHECK(rit->second.get()->get() == 11);
        
                rit--;
                CHECK(rit == g_const.rbegin());
                CHECK(rit != g_const.rend());
        
                ++rit;
                CHECK(rit != g_const.rbegin());
                CHECK(rit == --g_const.rend());
        
                --rit;
                CHECK(rit == g_const.rbegin());
                CHECK(rit != g_const.rend());
            }
        
            SECTION("graph + crbegin/crend") {
                criterator crit = g.crbegin();
                CHECK(crit != g.crend());
                CHECK(crit->first == "node 2");
            
                crit++;
                CHECK(crit != g.crbegin());
                CHECK(crit == --g.crend());
                CHECK(crit->second.get()->get() == 11);
            
                crit--;
                CHECK(crit == g.crbegin());
                CHECK(crit != g.crend());
            
                ++crit;
                CHECK(crit != g.crbegin());
                CHECK(crit == --g.crend());
            
                --crit;
                CHECK(crit == g.crbegin());
                CHECK(crit != g.crend());
            }
    
            SECTION("const graph + crbegin/crend") {
                criterator crit = g_const.crbegin();
                CHECK(crit != g_const.crend());
                CHECK(crit->first == "node 2");
        
                crit++;
                CHECK(crit != g_const.crbegin());
                CHECK(crit == --g_const.crend());
                CHECK(crit->second.get()->get() == 11);
        
                crit--;
                CHECK(crit == g_const.crbegin());
                CHECK(crit != g_const.crend());
        
                ++crit;
                CHECK(crit != g_const.crbegin());
                CHECK(crit == --g_const.crend());
        
                --crit;
                CHECK(crit == g_const.crbegin());
                CHECK(crit != g_const.crend());
            }
        
            SECTION("key/value") {
                CHECK(g.cbegin()->first == "node 1");
                CHECK((--g.end())->first == "node 2");
    
                CHECK(g.rbegin()->second->get() == 22);
                CHECK((--g.crend())->second->get() == 11);
            }
        }
    }
    
    SECTION("getters") {
        Graph g;
        for(int i = 0; i < 10; ++i)
            g["node " + to_string(i)] = i;
        
        SECTION("find(const key_type& k)") {
            iterator it = g.find("unexisting node");
            CHECK(it == g.end());
            
            it = g.find("node 4");
            CHECK(it == next(begin(g), 4));
            
            citerator cit = g.find("unexisting node");
            CHECK(cit == g.cend());
            
            cit = g.find("node 0");
            CHECK(cit == g.cbegin());
        }
    }
}
