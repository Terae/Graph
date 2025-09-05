[![Build Status](https://travis-ci.org/Terae/Graph.svg?branch=master)](https://travis-ci.org/Terae/Graph) [![codecov](https://codecov.io/gh/Terae/Graph/branch/master/graph/badge.svg)](https://codecov.io/gh/Terae/Graph)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/b79d80fae71f4eb9b447d1c78ce1fbc1)](https://www.codacy.com/app/Terae/Graph?utm_source=github.com&utm_medium=referral&utm_content=Terae/Graph&utm_campaign=Badge_Grade)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/1526/badge)](https://bestpractices.coreinfrastructure.org/projects/1526)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](http://terae.github.io/graph)
[![Github Releases](https://img.shields.io/github/release/terae/graph.svg)](https://github.com/terae/graph/releases)
[![Github license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/terae/Structure/master/LICENSE)
[![Github Issues](https://img.shields.io/github/issues/terae/graph.svg)](http://github.com/terae/graph/issues)
<!-- [![Coverage Status](https://coveralls.io/repos/github/Terae/Graph/badge.svg?branch=master)](https://coveralls.io/github/Terae/Graph?branch=master) --> <!-- [![Coverity Scan Build Status](https://scan.coverity.com/projects/14560/badge.svg)](https://scan.coverity.com/projects/structures) [![Build status](https://ci.appveyor.com/api/projects/status/5y7aq1kn6lx3jd94?svg=true)](https://ci.appveyor.com/project/Terae17352/structure) -->

# Graph Library

A STL-like graph library written in C++

- [Design goals](#design-goals)
- [Integration](#integration)
- [Documentation](#documentation)
- [Quickstart example](#quickstart-example)
- [Features](#features)
- [Common algorithms](#common-algorithms)
- [Supported compilers](#supported-compilers)
- [Execute unit tests](#execute-unit-tests)
- [Execute benchmarks](#execute-benchmarks)
- [License](#license)
- [Used third-party tools](#used-third-party-tools)

## Design goals

The Graph library is a STL-like library which can be used as an `std::` container. The class had these design goals:

- **Intuitive syntax**. This container uses all the operator magic of modern C++ to achieve a good feeling in your code.
- **Trivial integration**. The whole code consists of a single header file `graph.hpp`. No library, no subproject, no dependencies, no complex build system. The class is written in vanilla C++11 with optional features for C++17, C++20, and C++23.
- **Serious testing**. This class is heavily [unit-tested](https://github.com/terae/Structure/blob/master/tests/src/unit.cpp) and covers [100%](https://www.codacy.com/app/Terae/Graph?utm_source=github.com&utm_medium=referral&utm_content=Terae/Graph&utm_campaign=Badge_Grade) of the code. Furthermore, I checked with [Valgrind](http://www.valgrind.org/) that there are no memory leaks. To maintain high quality, the project is following the [Core Infrastructure Initiative (CII) best practices](https://bestpractices.coreinfrastructure.org/projects/1526).
- **Modern C++ features**. The library automatically leverages modern C++ features when available:
  - **C++17**: `std::optional` support for safer value access
  - **C++20**: Concepts for better template error messages
  - **C++23**: Full compatibility with the latest standard

## Integration

The library is header-only. To install and use simply copy the single required file `[graph.hpp](https://github.com/terae/graph/blob/dev/single_include/graph.hpp)` in your directory. After that, all you need to do is add

```c++
#include "graph.hpp"
```

to the files you want to process Graph.

This library requires Clang or GCC with `-std=c++11` or later enabled (or other compiler with sufficient C++ standard support). The library automatically detects and uses the highest available C++ standard:

- **C++11/C++14**: Basic template support
- **C++17**: Enhanced features including `std::optional` support
- **C++20**: Concepts-based template constraints for better error messages
- **C++23**: Full modern C++ support

That's it.

## Documentation

The [UML diagram](https://en.wikipedia.org/wiki/Unified_Modeling_Language) of the library can be found here: `doc/diagram.pdf`.

Beside the [examples](#quickstart-example) below, you may want to check the [documentation](https://terae.github.io/graph).

The documentation will be finished once the functionality and interfaces are finalized.

## Quickstart example

All [example files](https://github.com/terae/graph/tree/dev/doc/examples) can be compiled and executed on their own (e.g., file [route_finding.cpp](https://github.com/terae/graph/blob/dev/doc/examples/route_finding.cpp))

Once [installed](#integration), let's initialize a sample graph:

```c++
#include "graph.hpp"

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


|                                                      Name                                                      |      Initial     | Complete? | Optimal? | Time complexity  |     Space complexity     |                     Description                     |
|:--------------------------------------------------------------------------------------------------------------:|:----------------:|:---------:|:--------:|:----------------:|:------------------------:|:---------------------------------------------------:|
|                   [Breadth-First Search](https://en.wikipedia.org/wiki/breadth-first_search)                   |      **BFS**     |    YES    |    YES   |      O(b^d)      |          O(b^d)          |                  Uses a FIFO queue                  |
|                      [Deep-First Search](https://en.wikipedia.org/wiki/depth-first_search)                     |      **DFS**     |     NO    |    NO    |      O(b^m)      |           O(bm)          |                  Uses a LIFO queue                  |
|          [Depth-Limited Search](https://en.wikipedia.org/wiki/iterative_deepening_depth-first_search)          |      **DLS**     |     NO    |    NO    |      O(b^l)      |           O(bl)          |   Same as **DFS** with a predetermined depth limit  |
| [Iterative-Deepening Depth-First Search](https://en.wikipedia.org/wiki/iterative_deepening_depth-first_search) |     **IDDFS**    |    YES    |    YES   |      O(b^d)      |           O(bd)          |     Benefits combination of **BFS** and **DFS**     |
|                  [Uniform-Cost Search](https://en.wikipedia.org/wiki/Talk:uniform-cost_search)                 |      **UCS**     |    YES    |    YES   |  O(b^(1+C\*/E))  |      O(b^(1+C\*/E))      | Uses a queue ordered by the lowest path cost `g(n)` |
|                             [A*](https://en.wikipedia.org/wiki/A*_search_algorithm)                            |      **A\***     |    YES    |    YES   |                  | O((\|V\|+\|E\|)log\|V\|) |          **Optimally efficient  algorithm**         |
|                         [Dijkstra](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)                       |   **Dijkstra**   |    YES    |    YES   |     O(\|V\|)     | O(\|V\| * log\|V\| + \|E\|) |       Find the shortest paths between nodes         |
|                  [Bellman-Ford](https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm)                  | **Bellman-Ford** |    YES    |    YES   | O(\|V\| * \|E\|) |   |

<!-- Counting number of hops (simple BFS) * [Minimum spanning tree (MST)](https://en.wikipedia.org/wiki/Minimum_spanning_tree) - Kruskal - Prim * [Traveling salesman problem (TSP)](https://en.wikipedia.org/wiki/Travelling_salesman_problem) - Bruteforce algorithm - Minimum spanning tree heuristic (TSP MST heuristic) - Nearest neighbor heuristic (NN heuristic) * Maximum flow - [Edmonds-Karp](https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm) * Minimum cost flow (MCF) - Cycle canceling - Successive shortest path * Maximum matching - Flow algorithm -->

 ## Supported compilers

The library supports a wide range of compilers across different C++ standards:

### Modern C++ Support (Recommended)
- **GCC 9+** - Full C++20/C++23 support
- **Clang 10+** - Full C++20/C++23 support
- **MSVC 19.20+** - Full C++20 support

### Legacy Support
- **GCC 5.0 - 8.x** - C++11/C++14/C++17 support
- **Clang 3.5 - 9.x** - C++11/C++14/C++17 support
- **MSVC 19.00+** - C++11/C++14/C++17 support

Please note that `GCC 4.9` and earlier does not work.

The following compilers are currently used in continuous integration at [Travis](https://travis-ci.org/Terae/Graph):

Compiler    | Operating System   | Version String
----------- | ------------------ | -------------------------------------------------------------
GCC 5.4.1   | Ubuntu 14.04.5 LTS | g++-5 (Ubuntu 5.4.1-2ubuntu1~14.04) 5.4.1 20160904
GCC 6.3.0   | Ubuntu 14.04.5 LTS | g++-6 (Ubuntu/Linaro 6.3.0-18ubuntu2~14.04) 6.3.0 20170519
GCC 7.2.0   | Ubuntu 14.04.5 LTS | g++-7 (Ubuntu 7.2.0-1ubuntu1~14.04) 7.2.0
Clang 3.8.0 | Ubuntu 14.04.5 LTS | clang version 3.8.0-2ubuntu3~trusty5 (tags/RELEASE_380/final)
Clang 3.9.1 | Ubuntu 14.04.5 LTS | clang version 3.9.1-4ubuntu3~14.04.3 (tags/RELEASE_391/rc2)
Clang 4.0.1 | Ubuntu 14.04.5 LTS | clang version 3.9.1-4ubuntu3~14.04.3 (tags/RELEASE_401/rc2)

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

## Execute benchmarks

To compile and run the benchmarks, you need to execute

```bash
$ mkdir build
$ cd build
$ cmake .. -DENABLE_BENCHMARKING=ON
$ cmake --build .
$ cd benchmarks
$ ./graph_benchmarks
```

## License

This Graph Library is certified [Open Source](https://opensource.org/docs/definition.html) software. It may be used for any purpose, including commercial purposes, at absolutely no cost. It is distributed under the terms of the permissive [MIT license](https://opensource.org/licenses/MIT) reproduced here.

> MIT License

> Copyright (c) 2017-2025 Benjamin BIGEY

> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Used third-party tools

The library itself contains of a single header file licensed under the MIT license. However, it is build, tested and using a lot of third-party tools and services. Thanks a lot!

- **[Artistic Style](http://astyle.sourceforge.net/astyle.html)** for format the code
- **[benchpress](https://github.com/sbs-ableton/benchpress)** to benchmark the code
- **[Catch](https://github.com/philsquared/Catch)** for the unit tests
- **[Clang](http://clang.llvm.org/)** for compilation with code sanitizers
- **[Cmake](https://cmake.org/)** for build automation
- **[cppcheck](http://cppcheck.sourceforge.net/)** for static analysis
- **[JSON for Modern C++](https://github.com/nlohmann/json)** for [de]serialization of JSON files
- **[Travis](https://travis-ci.org/)** for [continuous integration](https://travis-ci.org/Terae/Graph) on Linux
- **[Valgrind](http://www.valgrind.org/)** to check for correct memory management
