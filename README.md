[![Build Status](https://travis-ci.org/Terae/Graph.svg?branch=master)](https://travis-ci.org/Terae/Graph)
[![codecov](https://codecov.io/gh/Terae/Graph/branch/master/graph/badge.svg)](https://codecov.io/gh/Terae/Graph)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/b79d80fae71f4eb9b447d1c78ce1fbc1)](https://www.codacy.com/app/Terae/Graph?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Terae/Graph&amp;utm_campaign=Badge_Grade)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/1526/badge)](https://bestpractices.coreinfrastructure.org/projects/1526)
[![Github Releases](https://img.shields.io/github/release/terae/graph.svg)](https://github.com/terae/graph/releases)
[![Github license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/terae/Structure/master/LICENSE)
[![Github Issues](https://img.shields.io/github/issues/terae/graph.svg)](http://github.com/terae/graph/issues)
<!--[![Coverage Status](https://coveralls.io/repos/github/Terae/Graph/badge.svg?branch=master)](https://coveralls.io/github/Terae/Graph?branch=master)-->
<!-- [![Coverity Scan Build Status](https://scan.coverity.com/projects/14560/badge.svg)](https://scan.coverity.com/projects/structures) -->
<!--[![Build status](https://ci.appveyor.com/api/projects/status/5y7aq1kn6lx3jd94?svg=true)](https://ci.appveyor.com/project/Terae17352/structure)-->

# Graph Library
A STL-like graph library written in C++

* [Design goals](#design-goals)
* [Documentation](#documentation)
* [Quickstart example](#quickstart-example)
* [Features](#features)
* [Common algorithms](#common-algorithms)
* [Supported compilers](#supported-compilers)
* [Execute unit tests](#execute-unit-tests)
* [License](#license)
* [Used third-party tools](#used-third-party-tools)

## Design goals
The Graph library is a STL-like library which can be used as an `std::` container. The class had these design goals:
* **Intuitive syntax**. This container uses all the operator magic of modern C++ to achieve a good feeling in your code.
* **Trivial integration**. The whole code consists of a single header file `graph.hpp`. No library, no subproject, no dependencies, no complex build system. The class is written in vanilla C++11. All in all, everything should require no adjustment of your compiler flags or project settings.
* **Serious testing**. This class is heavily [unit-tested](https://github.com/terae/Structure/blob/master/tests/src/unit.cpp) and covers [100%](https://www.codacy.com/app/Terae/Graph?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Terae/Graph&amp;utm_campaign=Badge_Grade) of the code. Furthermore, I checked with [Valgrind](http://www.valgrind.org/) that there are no memory leaks. To maintain high quality, the project is following the [Core Infrastructure Initiative (CII) best practices](https://bestpractices.coreinfrastructure.org/projects/1526).

## Documentation
The documentation will be finished once the functionality and interfaces are finalized.

Until then, here are a few short indications on how to use this library:
* Requires Clang or GCC with `-std=c++11` (or other compiler with sufficient C++11 support).
* The library is header-only, to install and use simply copy the single required source, file `graph.hpp` is in the `single_include` directory. All you need to do is add
```c++
#include "graph.hpp"
```
to the files you want to use Graph objects. That's it.

## Quickstart example
Once [installed](#installation), let's initialize a sample graph:

```c++
#include "../src/Graph.h"

using namespace std;

struct Atom {
    char symbol;
    int atomic_number;
    double atomic_weight;
    Atom() = default;
    Atom(char s, int nr, double w) : symbol(s), atomic_number(nr), atomic_weight(w) {}
};

int main() {
    Atom carbon  ('C', 6, 12.01);
    Atom hydrogen('H', 1, 1.01);

    using covalent_bond = int;
    graph<string, Atom, covalent_bond> methane;
    /// Initializing the graph
    methane["C"] = carbon;

    for(int i{1}; i <= 4; ++i)
        methane["H" + to_string(i)] = hydrogen;

    /// Covalent bonds between atoms
    methane("C", "H1") = 1;
    methane("C", "H2") = 1;
    methane("C", "H3") = 1;
    methane("C", "H4") = 1;

    double total_weight{0.0};
    for_each(methane.begin(), methane.end(), [&](decltype(methane)::value_type p) {
        total_weight += p.second->get().atomic_weight;
    });

    cout << "Methane molecule has a molar mass of " << total_weight << " g/mol." << endl;

    return EXIT_SUCCESS;
}
```

## Features
This library is built around the concept of [mathematical graph theory](https://en.wikipedia.org/wiki/Graph_theory) (i.e.) it is **not** a [charting](https://en.wikipedia.org/wiki/Chart) library for drawing a [graph of a function](https://en.wikipedia.org/wiki/Graph_of_a_function)).

In essence, a graph is a set of _nodes_ with any number of _edges_ in between. Edges can be either _undirected_ ("two-way") or _directed_ ("one-way", aka di-edges). Edges are also allowed to form [loops](https://en.wikipedia.org/wiki/Loop_%28graph_theory%29) (i.e. an edge from node A pointing to node A again).

## Common algorithms
One of the most common things to do with graphs is running algorithms to solve common graph problems. Therefore this library is being used as the basis for implementations for a number of commonly used graph algorithms:

|                                                      Name                                                      |  Initial  | Complete? | Optimal? | Time complexity |     Space complexity     |                     Description                     |
|:--------------------------------------------------------------------------------------------------------------:|:---------:|:---------:|:--------:|:---------------:|:------------------------:|:---------------------------------------------------:|
|                   [Breadth-First Search](https://en.wikipedia.org/wiki/breadth-first_search)                   |  **BFS**  |    YES    |    YES   |      O(b^d)     |          O(b^d)          |                  Uses a FIFO queue                  |
|                      [Deep-First Search](https://en.wikipedia.org/wiki/depth-first_search)                     |  **DFS**  |     NO    |    NO    |      O(b^m)     |           O(bm)          |                  Uses a LIFO queue                  |
|          [Depth-Limited Search](https://en.wikipedia.org/wiki/iterative_deepening_depth-first_search)          |  **DLS**  |     NO    |    NO    |      O(b^l)     |           O(bl)          |   Same as **DFS** with a predetermined depth limit  |
| [Iterative-Deepening Depth-First Search](https://en.wikipedia.org/wiki/iterative_deepening_depth-first_search) | **IDDFS** |    YES    |    YES   |      O(b^d)     |           O(bd)          |     Benefits combination of **BFS** and **DFS**     |
|                  [Uniform-Cost Search](https://en.wikipedia.org/wiki/Talk:uniform-cost_search)                 |  **UCS**  |    YES    |    YES   |  O(b^(1+C\*/E)) |      O(b^(1+C\*/E))      | Uses a queue ordered by the lowest path cost `g(n)` |
|                             [A*](https://en.wikipedia.org/wiki/A*_search_algorithm)                            |  **A\***  |    YES    |    YES   |                 | O((\|V\|+\|E\|)log\|V\|) |          **Optimally efficient algorithm**          |

<!--    - [Dijkstra](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)
    - Moore-Bellman-Ford (MBF)
    - Counting number of hops (simple BFS)
* [Minimum spanning tree (MST)](https://en.wikipedia.org/wiki/Minimum_spanning_tree)
    - Kruskal
    - Prim
* [Traveling salesman problem (TSP)](https://en.wikipedia.org/wiki/Travelling_salesman_problem)
    - Bruteforce algorithm
    - Minimum spanning tree heuristic (TSP MST heuristic)
    - Nearest neighbor heuristic (NN heuristic)
* Maximum flow
    - [Edmonds-Karp](https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm)
* Minimum cost flow (MCF)
    - Cycle canceling
    - Successive shortest path
* Maximum matching
    - Flow algorithm-->

## Supported compilers
Though it's 2017 already, the support for C++11 is still a bit sparse. Currently, the following compilers are known to work:
* GCC 5.0 - 7.1 (and possibly later)
* Clang 3.5 - 5.0 (and possibly later)

Please note that `GCC 4.9` and earlier does not work.

The following compilers are currently used in continuous integration at [Travis](https://travis-ci.org/Terae/Graph):

| Compiler        | Operating System             | Version String |
|-----------------|------------------------------|----------------|
| GCC 5.4.1       | Ubuntu 14.04.5 LTS           | g++-5 (Ubuntu 5.4.1-2ubuntu1~14.04) 5.4.1 20160904 |
| GCC 6.3.0       | Ubuntu 14.04.5 LTS           | g++-6 (Ubuntu/Linaro 6.3.0-18ubuntu2~14.04) 6.3.0 20170519 |
| GCC 7.2.0       | Ubuntu 14.04.5 LTS           | g++-7 (Ubuntu 7.2.0-1ubuntu1~14.04) 7.2.0    |
| Clang 3.8.0     | Ubuntu 14.04.5 LTS           | clang version 3.8.0-2ubuntu3~trusty5 (tags/RELEASE_380/final) |
| Clang 3.9.1     | Ubuntu 14.04.5 LTS           | clang version 3.9.1-4ubuntu3~14.04.3 (tags/RELEASE_391/rc2) |
| Clang 4.0.1     | Ubuntu 14.04.5 LTS           | clang version 3.9.1-4ubuntu3~14.04.3 (tags/RELEASE_401/rc2) |
<!--| Clang 3.5.0     | Ubuntu 14.04.5 LTS           | clang version 3.5.0 (tags/RELEASE_350/final) |
| Clang 3.6.2     | Ubuntu 14.04.5 LTS           | clang version 3.6.2 (tags/RELEASE_362/final) |
| Clang 3.7.1     | Ubuntu 14.04.5 LTS           | clang version 3.7.1 (tags/RELEASE_371/final) |
| Clang 5.0.0     | Ubuntu 14.04.5 LTS           | clang version 5.0.0 (tags/RELEASE_500/final) |-->

## Execute unit tests
To compile and run the tests, you need to execute
```bash
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ctest
```
For more information, have a look at the file [.travis.yml](https://github.com/terae/Graph/blob/master/.travis.yml).

## License
This Graph Library is certified [Open Source](https://opensource.org/docs/definition.html) software. It may be used for any purpose, including commercial purposes, at absolutely no cost. It is distributed under the terms of the permissive [MIT license](https://opensource.org/licenses/MIT) reproduced here.

> MIT License
>
> Copyright (c) 2017 Benjamin BIGEY
>
> Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Used third-party tools
The library itself contains of a single header file licensed under the MIT license. However, it is build, tested and using a lot of third-party tools and services. Thanks a lot!
* **[Artistic Style](http://astyle.sourceforge.net/astyle.html)** for format the code
* **[benchpress](https://github.com/sbs-ableton/benchpress)** to benchmark the code
* **[Catch](https://github.com/philsquared/Catch)** for the unit tests
* **[Changelog generator](https://github.com/skywinder/Github-Changelog-Generator)** for automated ChangeLog generation
* **[Clang](http://clang.llvm.org/)** for compilation with code sanitizers
* **[Cmake](https://cmake.org/)** for build automation
* **[cppcheck](http://cppcheck.sourceforge.net/)** for static analysis
* **[Travis](https://travis-ci.org/)** for [continuous integration](https://travis-ci.org/Terae/Graph) on Linux
* **[Valgrind](http://www.valgrind.org/)** to check for correct memory management
