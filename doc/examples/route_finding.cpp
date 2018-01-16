#include "../single_include/graph.hpp"

/**
 * @brief Romania Route Finding Problem using different search algorithms
 */

using namespace std;

int main() {
    graph_undirected<string, int, int> map;
    map("Oradea",         "Zerind")    = 71;
    map("Zerind",         "Arad")      = 75;
    map("Oradea",         "Sibiu")     = 151;
    map("Arad",           "Sibiu")     = 140;
    map("Arad",           "Timisoara") = 118;
    map("Timisoara",      "Lugoj")     = 111;
    map("Lugoj",          "Mehadia")   = 70;
    map("Mehadia",        "Drobeta")   = 75;
    map("Drobeta",        "Craiova")   = 120;
    map("Rimnicu Vilcea", "Craiova")   = 146;
    map("Craiova",        "Pitesti")   = 138;
    map("Rimnicu Vilcea", "Sibiu")     = 80;
    map("Sibiu",          "Fagaras")   = 99;
    map("Rimnicu Vilcea", "Pitesti")   = 97;
    map("Pitesti",        "Bucharest") = 101;
    map("Fagaras",        "Bucharest") = 211;
    map("Bucharest",      "Giurgiu")   = 90;
    map("Bucharest",      "Urziceni")  = 85;
    map("Urziceni",       "Hirsova")   = 98;
    map("Hirsova",        "Eforie")    = 86;
    map("Urziceni",       "Vaslui")    = 142;
    map("Vaslui",         "Iasi")      = 92;
    map("Iasi",           "Neamt")     = 87;

    cout << "There are " << map.size() << " nodes and " << map.get_nbr_edges() << " edges." << endl;
    cout << '\n' << map << endl;
}
