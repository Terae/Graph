#include "../single_include/graph.hpp"

#include <cmath>

/**
 * @brief Romania Route Finding Problem using different search algorithms
 * @see https://csunplugged.files.wordpress.com/2012/09/romania-graph1.png
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

    std::map<string, int> straight_line_Bucharest({
        {"Arad",           366},
        {"Bucharest",      0},
        {"Craiova",        160},
        {"Drobeta",        242},
        {"Eforie",         161},
        {"Fagaras",        176},
        {"Giurgiu",        77},
        {"Hirsova",        151},
        {"Iasi",           226},
        {"Lugoj",          244},
        {"Mehadia",        241},
        {"Neamt",          234},
        {"Oradea",         380},
        {"Pitesti",        100},
        {"Rimnicu Vilcea", 193},
        {"Sibiu",          253},
        {"Timisoara",      329},
        {"Urziceni",       80},
        {"Vaslui",         199},
        {"Zerind",         374}
    });

    cout << "\nSee https://csunplugged.files.wordpress.com/2012/09/romania-graph1.png for a visual representation of the Romania Graph." << endl;

    const string BEGIN{"Arad"};

    auto DFS    = search::make_DFS(map);
    auto pDFS   = DFS.run  (map.find(BEGIN), map.find("Bucharest"));

    auto BFS    = search::make_BFS(map);
    auto pBFS   = BFS.run  (map.find(BEGIN), map.find("Bucharest"));

    auto DLS    = search::make_DLS(map);
    auto pDLS   = DLS.run  (map.find(BEGIN), map.find("Bucharest"), 5);

    auto IDDFS  = search::make_IDDFS(map);
    auto pIDDFS = IDDFS.run(map.find(BEGIN), map.find("Bucharest"));

    auto UCS    = search::make_UCS(map);
    auto pUCS   = UCS.run  (map.find(BEGIN), map.find("Bucharest"));

    auto aStar  = search::make_AStar(map);
    auto heuristic = [&straight_line_Bucharest](const graph_undirected<string, int, int>::const_iterator & it, const graph_undirected<string, int, int>::const_iterator & target) -> double {
        if (target->first == "Bucharest") {
            return straight_line_Bucharest[it->first];
        }
    };
    auto pAStar = aStar.run(map.find(BEGIN), map.find("Bucharest"), heuristic);

    const size_t nr_digits_IDDFS{static_cast<size_t>(pIDDFS.empty() ? 0 : log10(pIDDFS.get_nr_steps()) + 1)};
    cout << "\nCalculation of the shortest path from '" << BEGIN << "' to 'Bucharest' with several search algorithms:\n"
         << "              | \x1B[34m\x1B[1mBreadth-First Search\x1B[0m |  \x1B[34m\x1B[1mDepth-First Search\x1B[0m  | \x1B[34m\x1B[1mDepth-Limited Search (5)\x1B[0m |  \x1B[34m\x1B[1mUniform-Cost Search\x1B[0m | \x1B[34m\x1B[1mIterative-Deepening Depth-First Search (" << pIDDFS.get_nr_steps() << ")\x1B[0m |       \x1B[34m\x1B[1mA* Search\x1B[0m      |\n"
         << "--------------|----------------------|----------------------|--------------------------|----------------------|-------------------------------------------" << string(nr_digits_IDDFS, '-') << "|----------------------|\n"
         << "  Total cost  |\x1B[31m" << setw(13) << pBFS.total_cost() << "         \x1B[0m|\x1B[31m" << setw(14) << pDFS.total_cost() << "        \x1B[0m|\x1B[31m" << setw(15) << pDLS.total_cost() << "           \x1B[0m|\x1B[31m" << setw(14) << pUCS.total_cost() << "        \x1B[0m|\x1B[31m" << setw(25) << pIDDFS.total_cost() << string(nr_digits_IDDFS, ' ') << "                  \x1B[0m|\x1B[31m" << setw(14) << pAStar.total_cost() << "        \x1B[0m|\n"
         << "              |                      |                      |                          |                      |                                            |                      |\n";

    int i{1}, col1{0}, col2{0}, col3{0}, col4{0}, col5{0}, col6{0};
    for (; !(pBFS.empty() && pDFS.empty() && pDLS.empty() && pIDDFS.empty() && pUCS.empty() && pAStar.empty()); ++i) {
        cout << left << setw(13) << ("    Step " + to_string(i)) << " |" << right;

        if (!pBFS.empty()) {
            auto p = pBFS.pop_front();
            col1 += p.second;
            cout << setw(21) << (p.first->first + " (" + to_string(col1) + ")");
        } else {
            cout << string(11, ' ') << '-' << string(9, ' ');
        }

        cout << " |";
        if (!pDFS.empty()) {
            auto p = pDFS.pop_front();
            col2 += p.second;
            cout << setw(21) << (p.first->first + " (" + to_string(col2) + ")");
        } else {
            cout << string(12, ' ') << '-' << string(8, ' ');
        }
        cout << " |";

        if (!pDLS.empty()) {
            auto p = pDLS.pop_front();
            col3 += p.second;
            cout << setw(25) << (p.first->first + " (" + to_string(col3) + ")");
        } else {
            cout << string(13, ' ') << '-' << string(11, ' ');
        }
        cout << " |";

        if (!pUCS.empty()) {
            auto p = pUCS.pop_front();
            col4 += p.second;
            cout << setw(21) << (p.first->first + " (" + to_string(col4) + ")");
        } else {
            cout << string(11, ' ') << '-' << string(9, ' ');
        }
        cout << " |";

        if (!pIDDFS.empty()) {
            auto p = pIDDFS.pop_front();
            col5 += p.second;
            cout << setw(42 + static_cast<int>(nr_digits_IDDFS)) << (p.first->first + " (" + to_string(col5) + ")");
        } else {
            cout << string(23, ' ') << '-' << string(18 + nr_digits_IDDFS, ' ');
        }
        cout << " |";

        if (!pAStar.empty()) {
            auto p = pAStar.pop_front();
            col6 += p.second;
            cout << setw(21) << (p.first->first + " (" + to_string(col6) + ")");
        } else {
            cout << string(12, ' ') << '-' << string(8, ' ');
        }

        cout << " |" << endl;
    }
}
