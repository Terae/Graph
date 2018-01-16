///
/// Created by Terae on 10/01/18.
///

#define BENCHPRESS_CONFIG_MAIN

#include <fstream>
#include <sstream>
#include <benchpress.hpp>
#include <graph.hpp>
#include <pthread.h>
#include <cmath>
#include <thread>

struct Coord {
    int x, y;
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
using Graph_grid = graph_undirected<Coord, int, double>;
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

enum class EMode { input, output, make_complete/*, search*/ };

static void bench(benchpress::context &ctx,
                  const std::string &in_path,
                  const EMode mode) {
    // using string streams for benmarking to factor-out cold-cache disk access
#if defined( FROMFILE )
    std::ifstream istr;
    {
        istr.open(in_path, std::ifstream::in);
        if (!istr) {
            std::cerr << "Unexistant file '" << in_path << "'." << std::endl;
            exit(1);
        }
    }
#else
    std::stringstream istr;
    {
        // read file into string stream
        std::ifstream input_file(in_path);
        if (input_file) {
            istr << input_file.rdbuf();
            input_file.close();
        } else {
            std::cerr << "Unexistant file '" << in_path << "'." << std::endl;
            exit(1);
        }
    }
#endif

    switch (mode) {
        // benchmarking input
        case EMode::input: {
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

        // benmarking output
        case EMode::output: {
            // create GRAPH value from input
            Graph_grid g;
            istr >> g;
            std::stringstream ostr;

            ctx.reset_timer();
            for (size_t i{0}; i < ctx.num_iterations(); ++i) {
                ostr << g;

                // reset data
                ostr.str(std::string());
            }

            break;
        }

        // benchmarking big amount of nodes and edges
        case EMode::make_complete: {
            Graph_directed g;
            istr >> g;
            std::stringstream ostr;

            ctx.reset_timer();
            g.link_all_nodes(42.0);

            break;
        }
    }
}

#define BENCHMARKING_I(mode, title, in_path)         \
    BENCHMARK((title), [](benchpress::context* ctx) {\
        bench(*ctx, (in_path), (mode));              \
    });

BENCHMARKING_I(EMode::input,         "parse    grid_graph.txt",       "files/grid_graph.txt")
BENCHMARKING_I(EMode::output,        "dump     grid_graph.txt",       "files/grid_graph.txt")
BENCHMARKING_I(EMode::make_complete, "link_all complete_graph_9.txt", "files/complete_graph_9.txt")

/*
make all
Generation of 0%...
Generation of 10%...
Generation of 20%...
Generation of 30%...
Generation of 40%...
Generation of 50%...
Generation of 60%...
Generation of 70%...
Generation of 80%...
Generation of 90%...
Write on the file 'grid_graph.txt'...
Done.

bash -c 'time ./graph_benchmarks'
parse grid_graph.txt                          1 14427270641 ns/op
dump  grid_graph.txt                          1 11092073379 ns/op
./graph_benchmarks 67.453s

real    1m7,469s
user    1m6,762s
sys     0m0,584s
 */