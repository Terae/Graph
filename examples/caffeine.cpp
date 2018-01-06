#include "../single_include/graph.hpp"

using namespace std;

struct Atom {
    char symbol;
    int atomic_number;
    double atomic_weight;
    Atom() = default;
    Atom(char s, int nr, double w) : symbol(s), atomic_number(nr), atomic_weight(w) {}
};
ostream& operator<<(ostream& os, const Atom& a) {
    return os << a.symbol << ':' << a.atomic_number << ':' << a.atomic_weight;
}

int main() {
    Atom carbon  ('C', 6, 12.01);
    Atom hydrogen('H', 1, 1.01);
    Atom nitrogen('N', 7, 14.01);
    Atom oxygen  ('O', 8, 15.999);

    using covalent_bond = int;
    graph_undirected<string, Atom, covalent_bond> caffeine;
    /// Initializing the graph
    for(int i{1}; i <= 8; ++i)
        caffeine["C" + to_string(i)] = carbon;

    for(int i{1}; i <= 10; ++i)
        caffeine["H" + to_string(i)] = hydrogen;

    for(int i{1}; i <= 4; ++i)
        caffeine["N" + to_string(i)] = nitrogen;

    caffeine["O1"] = oxygen;
    caffeine["O2"] = oxygen;

    /// Covalent bonds between atoms
    caffeine("O1", "C1")  = 2;
    caffeine("C1", "N1")  = 1;
    caffeine("N1", "C2")  = 1;
    caffeine("C2", "O2")  = 2;
    caffeine("C2", "N2")  = 1;
    caffeine("N2", "C3")  = 1;
    caffeine("C3", "C4")  = 2;
    caffeine("C1", "C4")  = 1;
    caffeine("C3", "N3")  = 1;
    caffeine("N3", "C5")  = 2;
    caffeine("C5", "N4")  = 1;
    caffeine("N4", "C4")  = 1;
    caffeine("N4", "C6")  = 1;
    caffeine("N1", "C7")  = 1;
    caffeine("N2", "C8")  = 1;
    caffeine("C6", "H1")  = 1;
    caffeine("C6", "H2")  = 1;
    caffeine("C6", "H3")  = 1;
    caffeine("C5", "H4")  = 1;
    caffeine("C7", "H5")  = 1;
    caffeine("C7", "H6")  = 1;
    caffeine("C7", "H7")  = 1;
    caffeine("C8", "H8")  = 1;
    caffeine("C8", "H9")  = 1;
    caffeine("C8", "H10") = 1;
    cout << caffeine << endl;

    double total_weight{0.0};
    for_each(caffeine.begin(), caffeine.end(), [&](auto p) {
        total_weight += p.second->get().atomic_weight;
    });

    cout << "Caffeine molecule has " << caffeine.size() << " atoms and has a molar mass of " << total_weight << " g/mol." << endl;

    return EXIT_SUCCESS;
}
