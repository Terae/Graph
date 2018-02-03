cmake_minimum_required(VERSION 2.8.8)
project(Graph_Benchmarks LANGUAGES CXX)

# set compiler flags
if((CMAKE_CXX_COMPILER_ID MATCHES GNU) OR (CMAKE_CXX_COMPILER_ID MATCHES Clang))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -flto -DNDEBUG -O3 -pthread")
endif()

# header directories
include_directories(third-party/benchpress/src/benchpress)
include_directories(${CMAKE_SOURCE_DIR}/../single_include)

# copy test files to build folder
file(COPY ${CMAKE_SOURCE_DIR}/files/astar.cpp.json ${CMAKE_SOURCE_DIR}/files/astar.rust.json DESTINATION files)

# benchmark binary
add_executable(graph_benchmarks src/benchmarks.cpp)
