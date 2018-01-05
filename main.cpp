//
// Created by Terae on 09/03/17.
//

#include "src/Graph.h"

using namespace std;

int main() {
	graph<string, int> g;
	g["Hello Word!"] = 42;
	cout << g << endl;

	return EXIT_SUCCESS;
}