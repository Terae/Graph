#include "../single_include/graph.hpp"

#include <cmath>

/**
 * @brief Romania Route Finding Problem using different search algorithms
 * @see https://csunplugged.files.wordpress.com/2012/09/romania-graph1.png
 */

using namespace std;

int main() {
    using Map = graph_undirected<string, int, int>;
    Map map;
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

    const string START{"Arad"}, TARGET{"Bucharest"};

    Map::search_path pbfs  {map.bfs  (START, TARGET)};
    Map::search_path pdfs  {map.dfs  (START, TARGET)};
    Map::search_path pdls  {map.dls  (START, TARGET, 5)};
    Map::search_path piddfs{map.iddfs(START, TARGET)};
    Map::search_path pucs  {map.ucs  (START, TARGET)};

    auto heuristic = [&straight_line_Bucharest, TARGET](const graph_undirected<string, int, int>::const_iterator & it) -> double {
        if (it->first == TARGET) {
            return straight_line_Bucharest[it->first];
        }
    };
    Map::search_path pastar{map.astar(START, TARGET, heuristic)};

    Map::dijkstra_path pdijkstra{map.dijkstra(START)};

    const size_t nr_digits_iddfs{static_cast<size_t>(piddfs.empty() ? 0 : log10(piddfs.size()) + 1)};
    cout << "\nCalculation of the shortest path from '" << START << "' to 'Bucharest' with several search algorithms:\n"
         << "              | \x1B[34m\x1B[1mBreadth-First Search\x1B[0m |  \x1B[34m\x1B[1mDepth-First Search\x1B[0m  | \x1B[34m\x1B[1mDepth-Limited Search (5)\x1B[0m |  \x1B[34m\x1B[1mUniform-Cost Search\x1B[0m | \x1B[34m\x1B[1mIterative-Deepening Depth-First Search (" << piddfs.size() << ")\x1B[0m |       \x1B[34m\x1B[1mA* Search\x1B[0m      |\n"
         << "--------------|----------------------|----------------------|--------------------------|----------------------|-------------------------------------------" << string(nr_digits_iddfs, '-') << "|----------------------|\n"
         << "  Total cost  |\x1B[31m" << setw(13) << pbfs.total_cost() << "         \x1B[0m|\x1B[31m" << setw(14) << pdfs.total_cost() << "        \x1B[0m|\x1B[31m" << setw(15) << pdls.total_cost() << "           \x1B[0m|\x1B[31m" << setw(14) << pucs.total_cost() << "        \x1B[0m|\x1B[31m" << setw(25) << piddfs.total_cost() << string(nr_digits_iddfs, ' ') << "                  \x1B[0m|\x1B[31m" << setw(14) << pastar.total_cost() << "        \x1B[0m|\n"
         << "              |                      |                      |                          |                      |                                            |                      |\n";

    int i{1}, col1{0}, col2{0}, col3{0}, col4{0}, col5{0}, col6{0};
    for (; !(pbfs.empty() && pdfs.empty() && pdls.empty() && piddfs.empty() && pucs.empty() && pastar.empty()); ++i) {
        cout << left << setw(13) << ("    Step " + to_string(i)) << " |" << right;

        if (!pbfs.empty()) {
            auto p = pbfs.front();
            pbfs.pop_front();
            col1 += p.second;
            cout << setw(21) << (p.first->first + " (" + to_string(col1) + ")");
        } else {
            cout << string(11, ' ') << '-' << string(9, ' ');
        }

        cout << " |";
        if (!pdfs.empty()) {
            auto p = pdfs.front();
            pdfs.pop_front();
            col2 += p.second;
            cout << setw(21) << (p.first->first + " (" + to_string(col2) + ")");
        } else {
            cout << string(12, ' ') << '-' << string(8, ' ');
        }
        cout << " |";

        if (!pdls.empty()) {
            auto p = pdls.front();
            pdls.pop_front();
            col3 += p.second;
            cout << setw(25) << (p.first->first + " (" + to_string(col3) + ")");
        } else {
            cout << string(13, ' ') << '-' << string(11, ' ');
        }
        cout << " |";

        if (!pucs.empty()) {
            auto p = pucs.front();
            pucs.pop_front();
            col4 += p.second;
            cout << setw(21) << (p.first->first + " (" + to_string(col4) + ")");
        } else {
            cout << string(11, ' ') << '-' << string(9, ' ');
        }
        cout << " |";

        if (!piddfs.empty()) {
            auto p = piddfs.front();
            piddfs.pop_front();
            col5 += p.second;
            cout << setw(42 + static_cast<int>(nr_digits_iddfs)) << (p.first->first + " (" + to_string(col5) + ")");
        } else {
            cout << string(23, ' ') << '-' << string(18 + nr_digits_iddfs, ' ');
        }
        cout << " |";

        if (!pastar.empty()) {
            auto p = pastar.front();
            pastar.pop_front();
            col6 += p.second;
            cout << setw(21) << (p.first->first + " (" + to_string(col6) + ")");
        } else {
            cout << string(12, ' ') << '-' << string(8, ' ');
        }

        cout << " |" << endl;
    }
}
