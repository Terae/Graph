//
// Created by Terae on 01/08/17.
//

#include "catch.hpp"
#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

TEST_CASE("text") {
    const std::string GRAPH_1{"graph<int, std::string, float> {\n"
        "    nodes: {\n"
        "        \"0\",   \"value of 0.000000\";\n"
        "        \"2\",   \"value of 3.000000\";\n"
        "        \"4\",   \"value of 6.000000\";\n"
        "        \"6\",   \"value of 9.000000\";\n"
        "        \"8\",   \"value of 12.000000\";\n"
        "        \"10\",  \"value of 15.000000\";\n"
        "        \"12\",  \"value of 18.000000\";\n"
        "        \"14\",  \"value of 21.000000\";\n"
        "        \"16\",  \"value of 24.000000\";\n"
        "        \"18\",  \"value of 27.000000\";\n"
        "        \"20\",  \"value of 30.000000\";\n"
        "        \"22\",  \"value of 33.000000\";\n"
        "        \"24\",  \"value of 36.000000\";\n"
        "        \"26\",  \"value of 39.000000\";\n"
        "        \"28\",  \"value of 42.000000\";\n"
        "        \"30\",  \"value of 45.000000\";\n"
        "        \"32\",  \"value of 48.000000\";\n"
        "        \"34\",  \"value of 51.000000\";\n"
        "        \"36\",  \"value of 54.000000\";\n"
        "        \"38\",  \"value of 57.000000\";\n"
        "        \"40\",  \"value of 60.000000\";\n"
        "        \"42\",  \"value of 63.000000\";\n"
        "        \"44\",  \"value of 66.000000\";\n"
        "        \"46\",  \"value of 69.000000\";\n"
        "        \"48\",  \"value of 72.000000\";\n"
        "        \"50\",  \"value of 75.000000\";\n"
        "        \"52\",  \"value of 78.000000\";\n"
        "        \"54\",  \"value of 81.000000\";\n"
        "        \"56\",  \"value of 84.000000\";\n"
        "        \"58\",  \"value of 87.000000\";\n"
        "        \"60\",  \"value of 90.000000\";\n"
        "        \"62\",  \"value of 93.000000\";\n"
        "        \"64\",  \"value of 96.000000\";\n"
        "        \"66\",  \"value of 99.000000\";\n"
        "        \"68\",  \"value of 102.000000\";\n"
        "        \"70\",  \"value of 105.000000\";\n"
        "        \"72\",  \"value of 108.000000\";\n"
        "        \"74\",  \"value of 111.000000\";\n"
        "        \"76\",  \"value of 114.000000\";\n"
        "        \"78\",  \"value of 117.000000\";\n"
        "        \"80\",  \"value of 120.000000\";\n"
        "        \"82\",  \"value of 123.000000\";\n"
        "        \"84\",  \"value of 126.000000\";\n"
        "        \"86\",  \"value of 129.000000\";\n"
        "        \"88\",  \"value of 132.000000\";\n"
        "        \"90\",  \"value of 135.000000\";\n"
        "        \"92\",  \"value of 138.000000\";\n"
        "        \"94\",  \"value of 141.000000\";\n"
        "        \"96\",  \"value of 144.000000\";\n"
        "        \"98\",  \"value of 147.000000\";\n"
        "        \"100\", \"value of 150.000000\";\n"
        "        \"102\", \"value of 153.000000\";\n"
        "        \"104\", \"value of 156.000000\";\n"
        "        \"106\", \"value of 159.000000\";\n"
        "        \"108\", \"value of 162.000000\";\n"
        "        \"110\", \"value of 165.000000\";\n"
        "        \"112\", \"value of 168.000000\";\n"
        "        \"114\", \"value of 171.000000\";\n"
        "        \"116\", \"value of 174.000000\";\n"
        "        \"118\", \"value of 177.000000\";\n"
        "        \"120\", \"value of 180.000000\";\n"
        "        \"122\", \"value of 183.000000\";\n"
        "        \"124\", \"value of 186.000000\";\n"
        "        \"126\", \"value of 189.000000\";\n"
        "        \"128\", \"value of 192.000000\";\n"
        "        \"130\", \"value of 195.000000\";\n"
        "        \"132\", \"value of 198.000000\";\n"
        "        \"134\", \"value of 201.000000\";\n"
        "        \"136\", \"value of 204.000000\";\n"
        "        \"138\", \"value of 207.000000\";\n"
        "        \"140\", \"value of 210.000000\";\n"
        "        \"142\", \"value of 213.000000\";\n"
        "        \"144\", \"value of 216.000000\";\n"
        "        \"146\", \"value of 219.000000\";\n"
        "        \"148\", \"value of 222.000000\";\n"
        "        \"150\", \"value of 225.000000\";\n"
        "        \"152\", \"value of 228.000000\";\n"
        "        \"154\", \"value of 231.000000\";\n"
        "        \"156\", \"value of 234.000000\";\n"
        "        \"158\", \"value of 237.000000\";\n"
        "        \"160\", \"value of 240.000000\";\n"
        "        \"162\", \"value of 243.000000\";\n"
        "        \"164\", \"value of 246.000000\";\n"
        "        \"166\", \"value of 249.000000\";\n"
        "        \"168\", \"value of 252.000000\";\n"
        "        \"170\", \"value of 255.000000\";\n"
        "        \"172\", \"value of 258.000000\";\n"
        "        \"174\", \"value of 261.000000\";\n"
        "        \"176\", \"value of 264.000000\";\n"
        "        \"178\", \"value of 267.000000\";\n"
        "        \"180\", \"value of 270.000000\";\n"
        "        \"182\", \"value of 273.000000\";\n"
        "        \"184\", \"value of 276.000000\";\n"
        "        \"186\", \"value of 279.000000\";\n"
        "        \"188\", \"value of 282.000000\";\n"
        "        \"190\", \"value of 285.000000\";\n"
        "        \"192\", \"value of 288.000000\";\n"
        "        \"194\", \"value of 291.000000\";\n"
        "        \"196\", \"value of 294.000000\";\n"
        "        \"198\", \"value of 297.000000\"\n"
        "    }\n"
        "}\n"};

    const std::string GRAPH_2{"digraph<int, std::string, float> {\n"
        "    nodes: {\n"
        "        \"0\",   \"value of 0.000000\";\n"
        "        \"2\",   \"value of 3.000000\";\n"
        "        \"4\",   \"value of 6.000000\";\n"
        "        \"6\",   \"value of 9.000000\";\n"
        "        \"8\",   \"value of 12.000000\";\n"
        "        \"10\",  \"value of 15.000000\";\n"
        "        \"12\",  \"value of 18.000000\";\n"
        "        \"14\",  \"value of 21.000000\";\n"
        "        \"16\",  \"value of 24.000000\";\n"
        "        \"18\",  \"value of 27.000000\";\n"
        "        \"20\",  \"value of 30.000000\";\n"
        "        \"22\",  \"value of 33.000000\";\n"
        "        \"24\",  \"value of 36.000000\";\n"
        "        \"26\",  \"value of 39.000000\";\n"
        "        \"28\",  \"value of 42.000000\";\n"
        "        \"30\",  \"value of 45.000000\";\n"
        "        \"32\",  \"value of 48.000000\";\n"
        "        \"34\",  \"value of 51.000000\";\n"
        "        \"36\",  \"value of 54.000000\";\n"
        "        \"38\",  \"value of 57.000000\";\n"
        "        \"40\",  \"value of 60.000000\";\n"
        "        \"42\",  \"value of 63.000000\";\n"
        "        \"44\",  \"value of 66.000000\";\n"
        "        \"46\",  \"value of 69.000000\";\n"
        "        \"48\",  \"value of 72.000000\";\n"
        "        \"50\",  \"value of 75.000000\";\n"
        "        \"52\",  \"value of 78.000000\";\n"
        "        \"54\",  \"value of 81.000000\";\n"
        "        \"56\",  \"value of 84.000000\";\n"
        "        \"58\",  \"value of 87.000000\";\n"
        "        \"60\",  \"value of 90.000000\";\n"
        "        \"62\",  \"value of 93.000000\";\n"
        "        \"64\",  \"value of 96.000000\";\n"
        "        \"66\",  \"value of 99.000000\";\n"
        "        \"68\",  \"value of 102.000000\";\n"
        "        \"70\",  \"value of 105.000000\";\n"
        "        \"72\",  \"value of 108.000000\";\n"
        "        \"74\",  \"value of 111.000000\";\n"
        "        \"76\",  \"value of 114.000000\";\n"
        "        \"78\",  \"value of 117.000000\";\n"
        "        \"80\",  \"value of 120.000000\";\n"
        "        \"82\",  \"value of 123.000000\";\n"
        "        \"84\",  \"value of 126.000000\";\n"
        "        \"86\",  \"value of 129.000000\";\n"
        "        \"88\",  \"value of 132.000000\";\n"
        "        \"90\",  \"value of 135.000000\";\n"
        "        \"92\",  \"value of 138.000000\";\n"
        "        \"94\",  \"value of 141.000000\";\n"
        "        \"96\",  \"value of 144.000000\";\n"
        "        \"98\",  \"value of 147.000000\";\n"
        "        \"100\", \"value of 150.000000\";\n"
        "        \"102\", \"value of 153.000000\";\n"
        "        \"104\", \"value of 156.000000\";\n"
        "        \"106\", \"value of 159.000000\";\n"
        "        \"108\", \"value of 162.000000\";\n"
        "        \"110\", \"value of 165.000000\";\n"
        "        \"112\", \"value of 168.000000\";\n"
        "        \"114\", \"value of 171.000000\";\n"
        "        \"116\", \"value of 174.000000\";\n"
        "        \"118\", \"value of 177.000000\";\n"
        "        \"120\", \"value of 180.000000\";\n"
        "        \"122\", \"value of 183.000000\";\n"
        "        \"124\", \"value of 186.000000\";\n"
        "        \"126\", \"value of 189.000000\";\n"
        "        \"128\", \"value of 192.000000\";\n"
        "        \"130\", \"value of 195.000000\";\n"
        "        \"132\", \"value of 198.000000\";\n"
        "        \"134\", \"value of 201.000000\";\n"
        "        \"136\", \"value of 204.000000\";\n"
        "        \"138\", \"value of 207.000000\";\n"
        "        \"140\", \"value of 210.000000\";\n"
        "        \"142\", \"value of 213.000000\";\n"
        "        \"144\", \"value of 216.000000\";\n"
        "        \"146\", \"value of 219.000000\";\n"
        "        \"148\", \"value of 222.000000\";\n"
        "        \"150\", \"value of 225.000000\";\n"
        "        \"152\", \"value of 228.000000\";\n"
        "        \"154\", \"value of 231.000000\";\n"
        "        \"156\", \"value of 234.000000\";\n"
        "        \"158\", \"value of 237.000000\";\n"
        "        \"160\", \"value of 240.000000\";\n"
        "        \"162\", \"value of 243.000000\";\n"
        "        \"164\", \"value of 246.000000\";\n"
        "        \"166\", \"value of 249.000000\";\n"
        "        \"168\", \"value of 252.000000\";\n"
        "        \"170\", \"value of 255.000000\";\n"
        "        \"172\", \"value of 258.000000\";\n"
        "        \"174\", \"value of 261.000000\";\n"
        "        \"176\", \"value of 264.000000\";\n"
        "        \"178\", \"value of 267.000000\";\n"
        "        \"180\", \"value of 270.000000\";\n"
        "        \"182\", \"value of 273.000000\";\n"
        "        \"184\", \"value of 276.000000\";\n"
        "        \"186\", \"value of 279.000000\";\n"
        "        \"188\", \"value of 282.000000\";\n"
        "        \"190\", \"value of 285.000000\";\n"
        "        \"192\", \"value of 288.000000\";\n"
        "        \"194\", \"value of 291.000000\";\n"
        "        \"196\", \"value of 294.000000\";\n"
        "        \"198\", \"value of 297.000000\"\n"
        "    },\n"
        "    edges: {\n"
        "        \"0\",  \"2\",  \"0.2\";\n"
        "        \"0\",  \"6\",  \"0.6\";\n"
        "        \"6\",  \"82\", \"6.82\";\n"
        "        \"8\",  \"46\", \"8.46\";\n"
        "        \"24\", \"42\", \"24.42\";\n"
        "        \"42\", \"2\",  \"42.2\";\n"
        "        \"42\", \"24\", \"42.24\";\n"
        "        \"42\", \"6\",  infinity;\n"
        "        \"70\", \"6\",  \"70.6\"\n"
        "    }\n"
        "}\n"};

    using namespace std;
    using Graph            = graph           <int, string, float>;
    using Graph_directed   = graph_directed  <int, string, float>;
    using Graph_undirected = graph_undirected<int, string, float>;

    SECTION("operator<<(ostream& os, const graph<Key, T, Cost>& g)") {
        SECTION("simple graph") {
            Graph_undirected g;

            for (int i{0}; i < 200; i += 2) {
                g[i] = "value of " + to_string(i * 1.5);
            }

            ostringstream out;
            out << g;

            CHECK(GRAPH_1 == out.str());
        }

        SECTION("complete graph") {
            Graph_directed g;

            for (int i{0}; i < 200; i += 2) {
                g[i] = "value of " + to_string(i * 1.5);
            }

            g(0,  2)  = 0.2;
            g(6,  82) = 6.82;
            g(0,  6)  = 0.6;
            g(8,  46) = 8.46;
            g(42, 2)  = 42.2;
            g(70, 6)  = 70.6;
            g(24, 42) = 24.42;
            g(42, 24) = 42.24;
            g(42, 6);

            ostringstream out;
            out << g;

            CHECK(GRAPH_2 == out.str());
        }
    }

    SECTION("operator>>(istream& is, graph<Key, T, Cost>& g)") {
        SECTION("Exceptions") {
            Graph g1;
            Graph_directed g2;
            Graph_undirected g3;
            std::stringstream ss1, ss2, ss3, ss4, ss5, ss6, ss7, ss8;

            ss1 << "badgraph<string, int, float> {\n    nodes: {\n    }\n}";
            CHECK_THROWS_WITH(ss1 >> g1, "[graph.exception.parse_error] parse error at 31: Bad graph nature (expected '[di]graph') when calling 'operator>>'.");

            ss2 << "graph<string, int, float> {\n    nodes: {\n    }\n}";
            CHECK_THROWS_WITH(ss2 >> g2, "[graph.exception.invalid_argument] Bad graph nature (expected 'graph') when calling 'operator>>'.");

            ss3 << "digraph<string, int, float> {\n    nodes: {\n    }\n}";
            CHECK_THROWS_WITH(ss3 >> g3, "[graph.exception.invalid_argument] Bad graph nature (expected 'digraph') when calling 'operator>>'.");

            ss4 << "graph<string, int, float> {\n badnodes: {\n    }\n}";
            CHECK_THROWS_WITH(ss4 >> g1, "[graph.exception.parse_error] parse error at 41: Bad format for nodes when calling 'operator>>'.");

            ss5 << "graph<string, int, float> {\n    nodes: {\n        bad values";
            CHECK_THROWS_WITH(ss5 >> g1, "[graph.exception.parse_error] parse error at 5: Bad type when calling 'read_T'.");

            ss6 << "graph<string, int, float> {\n    nodes: {\n    },\n    badedges: {\n    }\n}";
            CHECK_THROWS_WITH(ss6 >> g1, "[graph.exception.parse_error] parse error at 64: Bad format for edges when calling 'operator>>'.");

            ss7 << "graph<string, int, float> {\n    nodes: {\n    },\n    edges: {\n        bad values\n    }\n}";
            CHECK_THROWS_WITH(ss7 >> g1, "[graph.exception.parse_error] parse error at 5: Bad type when calling 'read_T'.");

            ss8 << "graph<string, int, float> {\n    nodes: {\n    },\n    edges: {\n    }\n";
            CHECK_THROWS_WITH(ss8 >> g1, "[graph.exception.parse_error] parse error at 18446744073709551615: Bad format at the end of the graph when calling 'operator>>'.");
        };

        SECTION("Reading from istream") {
            Graph_directed g1;

            for (int i{0}; i < 200; i += 2) {
                g1[i] = "value of " + to_string(i * 1.5);
            }

            g1(0,  2)  = 0.2;
            g1(6,  82) = 6.82;
            g1(0,  6)  = 0.6;
            g1(8,  46) = 8.46;
            g1(42, 2)  = 42.2;
            g1(70, 6)  = 70.6;
            g1(24, 42) = 24.42;
            g1(42, 24) = 42.24;
            g1(42, 6);

            Graph_directed g2;
            std::stringstream ss1;
            ss1 << GRAPH_2;
            ss1 >> g2;
            CHECK(g1 == g2);

            std::stringstream ss2;
            ss2 << GRAPH_2;
            Graph_undirected g3;
            CHECK_THROWS_WITH(ss2 >> g3, "[graph.exception.invalid_argument] Bad graph nature (expected 'digraph') when calling 'operator>>'.");
        }
    }

    SECTION("save/load") {
        Graph_directed g1;

        for (int i{0}; i < 200; i += 2) {
            g1[i] = "value of " + to_string(i * 1.5);
        }

        g1(0,  2)  = 0.2;
        g1(6,  82) = 6.82;
        g1(0,  6)  = 0.6;
        g1(8,  46) = 8.46;
        g1(42, 2)  = 42.2;
        g1(70, 6)  = 70.6;
        g1(24, 42) = 24.42;
        g1(42, 24) = 42.24;
        g1(42, 6);

        g1.save("/tmp/graph.txt");

        Graph_directed g2;
        g2.load("/tmp/graph.txt");

        CHECK(g1 == g2);

        Graph_undirected g3;
        CHECK_THROWS_WITH(g3.load("/tmp/graph.txt"), "[graph.exception.invalid_argument] Bad graph nature (expected 'digraph') when calling 'operator>>'.");
    }
}
