/**
 * @file main.cpp
 * @brief Example usage of the Graph library
 *
 * This example demonstrates basic usage of the Graph library,
 * showing how to create a graph, add nodes and edges, and
 * output the graph to stdout.
 */

#include <iostream>
#include <cstdlib>
#include <string>

#include "src/Graph.h"

using namespace std;

/**
 * @brief Main entry point for the example application
 *
 * Creates a simple graph with one node and demonstrates
 * basic graph operations.
 *
 * @return EXIT_SUCCESS on successful execution
 */
int main() {
    try {
        // Create a graph with string keys and integer values
        graph<string, int> g;

        // Add a node with key "Hello World!" and value 42
        g["Hello World!"] = 42;

        // Output the graph to stdout
        cout << g << endl;

        return EXIT_SUCCESS;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return EXIT_FAILURE;
    } catch (...) {
        cerr << "Unknown error occurred" << endl;
        return EXIT_FAILURE;
    }
}