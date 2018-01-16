///
/// Created by Terae on 10/01/18.
///

#include "../../single_include/graph.hpp"

// Used to define range of the grid_graph
#define MAX 500
// Used to calculate the total amount of nodes to create, TODO: update this number when creating a new benchmark graph
#define TOTAL_NODES (4 * MAX * MAX + 2 * MAX)
// Define frequency of percent numbers you want to see (NR_STEPS = 5 includes 5 displayed percents: 20%, 40%, 60%, 800% and 100%
#define NR_STEPS 10

using namespace std;

struct Coord {
    int x, y;
    friend ostream &operator<<(ostream &os, const Coord &c) {
        return os << '[' << c.x << ", " << c.y << ']';
    }

    bool operator<(const Coord &c) const {
        if (x < c.x) {
            return true;
        }
        return y < c.y;
    }
};

// return pair<printable, percent>
void print(int &counter) {
    if (counter % (TOTAL_NODES / NR_STEPS) == 0) {
        int percent{counter * 100 / TOTAL_NODES + 100 / NR_STEPS};
        while (percent % (100 / NR_STEPS) != 0) {
            ++percent;
        }
        percent = min(percent, 100);
        cout << "\rGraphs creation  ... [" << setw(NR_STEPS) << string(percent * NR_STEPS / 100, '#') << "] (" << percent << "%)" << flush;
    }
    ++counter;
}

int main() {
    int count{0};
    cout << setfill('-') << left;

    /// Represents a grid graph; each node is connected to its 4 neighbors
    graph_undirected<Coord, int, double> grid_graph;
    for (int x{- MAX + 1}; x < MAX; ++x) {
        for (int y{- MAX + 1}; y < MAX; ++y) {
            grid_graph({x, y}, {x,     y + 1}) = 1;
            grid_graph({x, y}, {x + 1, y})     = 1;

            print(count);
        }
    }

    graph_directed<string, int, double> complete_graph;
    for (int i{1}; i <= 2 * MAX; ++i) {
        complete_graph["node " + to_string(i)] = i;

        print(count);
    }

    cout << setfill(' ') << right << "\nFiles generation ... " << flush;

    ofstream ofs("grid_graph.txt");
    ofs << grid_graph << endl;
    ofs.close();

    ofs.open("complete_graph.txt");
    ofs << complete_graph << endl;
    ofs.close();

    cout << "Done.\n" << endl;

    return EXIT_SUCCESS;
}
