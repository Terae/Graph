# set compiler flags
if((CMAKE_CXX_COMPILER_ID MATCHES GNU) OR (CMAKE_CXX_COMPILER_ID MATCHES Clang))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -flto -DNDEBUG -O3 -pthread")
endif()

execute_process(
        COMMAND git submodule update --init ${Graph_SOURCE_DIR}/benchmarks/third-party/benchpress
        WORKING_DIRECTORY ${Graph_SOURCE_DIR}
)

# header directories
include_directories(third-party/benchpress/src/benchpress)

# copy test files to build folder
set(bench_files files/search.cpp.json
                files/search.rust.json)
file(COPY ${bench_files} DESTINATION files)

# benchmark binary
add_executable(graph_benchmarks src/benchmarks.cpp)
