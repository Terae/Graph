//
// Created by Terae on 02/08/17.
//

#include "../../src/Graph.h"

int main() {
    const std::string TEXT{"digraph<string, int, double> {\n"
                                   "    nodes: {\n"
                                   "        \"node 0\", \"0\";\n"
                                   "        \"node 1\", \"1\";\n"
                                   "        \"node 2\", \"2\";\n"
                                   "        \"node 3\", \"3\";\n"
                                   "        \"node 4\", \"4\";\n"
                                   "        \"node 5\", \"5\";\n"
                                   "        \"node 6\", \"6\";\n"
                                   "        \"node 7\", \"7\";\n"
                                   "        \"node 8\", \"8\";\n"
                                   "        \"node 9\", \"9\";\n"
                                   "        \"target\", \"10\"\n"
                                   "    },\n"
                                   "    edges: {\n"
                                   "        \"node 0\", \"target\", \"0\";\n"
                                   "        \"node 1\", \"target\", \"10\";\n"
                                   "        \"node 2\", \"target\", \"20\";\n"
                                   "        \"node 3\", \"target\", \"30\";\n"
                                   "        \"node 4\", \"target\", \"40\";\n"
                                   "        \"node 5\", \"target\", \"50\";\n"
                                   "        \"node 6\", \"target\", \"60\";\n"
                                   "        \"node 7\", \"target\", \"70\";\n"
                                   "        \"node 8\", \"target\", \"80\";\n"
                                   "        \"node 9\", \"target\", \"90\";\n"
                                   "        \"target\", \"target\", \"100\";\n"
                                   "    }\n"
                                   "}"};
    graph<std::string, int, double> g(UNDIRECTED);
    std::stringstream ss;
    ss << TEXT;
    GRAPH_TRY {
        ss >> g;
    } GRAPH_CATCH(std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    std::stringstream graph;
    std::cout << g;
}
