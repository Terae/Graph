///
/// Created by Terae on 10/01/18.
///

#define BENCHPRESS_CONFIG_MAIN

#include <benchpress.hpp>
#include <cmath>
#include <fstream>
#include <graph.hpp>
#include <pthread.h>
#include <random>
#include <sstream>
#include <thread>

#define MAX 500

struct Coord {
    int x, y;
    Coord(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
    friend std::ostream &operator<<(std::ostream &os, const Coord &c) {
        return os << '[' << c.x << ", " << c.y << ']';
    }
    friend std::istream &operator>>(std::istream &is, Coord &c) {
        char ch;
        return is >> ch >> c.x >> ch >> c.y;
    }
    bool operator<(const Coord &c) const {
        if (x < c.x) {
            return true;
        }
        return y < c.y;
    }
};
using Graph_grid = graph_undirected<Coord, int, int>;
using Graph_directed = graph_directed  <std::string, int, double>;

struct StartUp {
    StartUp() {
#ifndef __llvm__
        // pin thread to a single CPU
        cpu_set_t cpuset;
        pthread_t thread;
        thread = pthread_self();
        CPU_ZERO(&cpuset);
        CPU_SET(std::thread::hardware_concurrency() - 1,  &cpuset);
        pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
#endif
    }
};
StartUp startup;

enum class EMode { input, output, make_complete, astar/*, search*/ };

static void bench(benchpress::context &ctx, const EMode mode) {
    graph_undirected<Coord, int, double> grid_graph;
    if (mode == EMode::input || mode == EMode::output) {
        for (int x{-MAX + 1}; x < MAX; ++x) {
            for (int y{-MAX + 1}; y < MAX; ++y) {
                grid_graph({x, y}, {x, y + 1}) = 1;
                grid_graph({x, y}, {x + 1, y}) = 1;
            }
        }
    }

    switch (mode) {
        /// Benchmarking input
        case EMode::input: {
            std::stringstream istr;
            istr << grid_graph;

            ctx.reset_timer();
            for (size_t i{0}; i < ctx.num_iterations(); ++i) {
                // clear flags and rewind
                istr.clear();
                istr.seekg(0);
                Graph_grid g;
                istr >> g;
            }

            break;
        }

        /// Benchmarking output
        case EMode::output: {
            ctx.reset_timer();
            std::ostringstream ostr;

            ctx.reset_timer();
            for (size_t i{0}; i < ctx.num_iterations(); ++i) {
                ostr << grid_graph;

                // reset data
                ostr.str(std::string());
            }

            break;
        }

        /// Benchmarking big amount of nodes and edges
        case EMode::make_complete: {
            graph_directed<std::string, int, double> complete_graph;
            for (int i{1}; i <= MAX; ++i) {
                complete_graph["node " + std::to_string(i)] = i;
            }

            ctx.reset_timer();
            for (size_t i{0}; i < ctx.num_iterations(); ++i) {
                complete_graph.link_all_nodes(42.0);
            }

            break;
        }

        /// Benchmarking A* Search algorithm
        case EMode::astar: {
            graph_directed<std::string, int, int> g;
            //g.load("/home/terae/Programs/Graph/benchmarks/files/astar.json");
            // My own JSON serialization
            g.load("files/astar.cpp.json");
            // rust JSON serialization
            //g.DEBUG_load_from_json("astar.rust.json");
            ctx.reset_timer();
            for (size_t i{0}; i < ctx.num_iterations(); ++i) {
                auto astar = search::make_astar(g, g.find("END"));
                auto search_result = astar.run("START", [](const graph_directed<std::string, int, int>::const_iterator & it) -> int {
                    return 5;
                });
            }

            break;
        }
    }
}

#define BENCHMARKING_I(mode, title)                  \
    BENCHMARK((title), [](benchpress::context* ctx) {\
        bench(*ctx, (mode));                         \
    });

BENCHMARKING_I(EMode::input,         "parse    grid_graph.txt")
BENCHMARKING_I(EMode::output,        "dump     grid_graph.txt")
BENCHMARKING_I(EMode::make_complete, "make_complete a graph of 1000 nodes")
BENCHMARKING_I(EMode::astar,         "astar on a graph of 1000 nodes")