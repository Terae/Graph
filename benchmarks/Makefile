#
# Build/run graph.hpp benchmarks, eg. CXX=g++-7 make
#

all: graph_benchmarks
	bash -c 'time ./graph_benchmarks'

graph_benchmarks: src/benchmarks.cpp ../single_include/graph.hpp
	$(CXX) -std=c++11 -pthread $(CXXFLAGS) -DNDEBUG -O3 -flto -I third-party/benchpress/src/benchpress -I ../single_include src/benchmarks.cpp $(LDFLAGS) -o $@

clean:
	rm -f graph_benchmarks
