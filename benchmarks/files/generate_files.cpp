///
/// Created by Terae on 10/01/18.
///

#include "../../single_include/graph.hpp"

#define MAX 500
struct Coord {
    int x, y;
    friend std::ostream &operator<<(std::ostream &os, const Coord &c) {
        return os << '[' << c.x << ", " << c.y << ']';
    }

    bool operator<(const Coord &c) const {
        if (x < c.x) {
            return true;
        }
        return y < c.y;
    }
};

int main() {
    int i{0};
    graph<Coord, int, double, UNDIRECTED> g;
    std::cout << std::setfill('-') << std::left;
    for (int x{- MAX + 1}; x < MAX; ++x) {
        for (int y{- MAX + 1}; y < MAX; ++y, ++i) {
            g({x, y}, {x,    y + 1}) = 1;
            g({x, y}, {x + 1, y})    = 1;
            if (i % (2 * MAX * MAX / 5) == 0) {
                const int percent{i * 25 / MAX / MAX + 10};
                std::cout << "\rGraphs generation ... [" << std::setw(10) << std::string(percent / 10, '#') << "] (" << percent << "%)" << std::flush;
            }
        }
    }

    std::cout << std::setfill('.') << std::right << "\nWrite on the file 'grid_graph.txt'..." << std::endl;
    std::ofstream ofs("grid_graph.txt");
    ofs << g << std::endl;
    ofs.close();
    std::cout << "Done.\n" << std::endl;

    return EXIT_SUCCESS;
}
