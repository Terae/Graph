//
// Created by Terae on 08/12/17.
//

#include "catch.hpp"
#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "Graph.h"
#endif

// FIXME: this file test is necessary? (test the Graph integration into STL functions)

TEST_CASE("algorithms") {
    using namespace std;
    using Graph = graph_directed<string, int, double>;

    SECTION("non-modifying sequence operations") {
        /*SECTION("all_of") {}
        SECTION("any_of") {}
        SECTION("none_of") {}*/

        SECTION("for_each") {
            Graph g;

            for(int i{0}; i < 10; ++i)
                g["node " + to_string(i)] = i;

            Graph::iterator it{g.emplace("target", 10).first};

            int i{0};
            for_each(g.begin(), g.end(), [it, &i](decltype(g)::value_type /*pair<const string, shared_ptr<node<int, double>>> */&p){
                p.second->add_edge(it, i * 10);
                ++i;
            });

            for(i = 0; i < 10; ++i)
                CHECK(g("node " + to_string(i), "target") == i * 10);
        }

        /*SECTION("count") {}
        SECTION("mismatch") {}
        SECTION("equal") {}
        SECTION("find") {}
        SECTION("find_if") {}
        SECTION("find_if_not") {}
        SECTION("adjacent_find") {}
        SECTION("search") {}
        SECTION("search_n") {}*/
    }

    /*SECTION("modifying sequence operations") {
        SECTION("copy") {}
        SECTION("copy_if") {}
        SECTION("copy_n") {}
        SECTION("copy_backward") {}
        SECTION("move") {}
        SECTION("move_backward") {}
        SECTION("fill") {}
        SECTION("fill_n") {}
        SECTION("transform") {}
        SECTION("generate") {}
        SECTION("generate_n") {}
        SECTION("remove") {}
        SECTION("remove_if") {}
        SECTION("remove_copy") {}
        SECTION("remove_copy_if") {}
        SECTION("replace") {}
        SECTION("replace_if") {}
        SECTION("replace_copy") {}
        SECTION("replace_copy_if") {}
        SECTION("swap") {}
        SECTION("swap_ranges") {}
        SECTION("iter_swap") {}
        SECTION("reverse") {}
        SECTION("reverse_copy") {}
        SECTION("rotate") {}
        SECTION("rotate_copy") {}
        SECTION("random_shuffle") {}
        SECTION("shuffle") {}
        SECTION("unique") {}
        SECTION("unique_copy") {}
    }

    SECTION("partitioning operations") {
        SECTION("is_partitioned") {}
        SECTION("partition") {}
        SECTION("partition_copy") {}
        SECTION("stable_partition") {}
        SECTION("partition_point") {}
    }

    SECTION("sorting operations") {
        SECTION("is_sorted") {}
        SECTION("is_sorted_until") {}
        SECTION("sort") {}
        SECTION("partial_sort") {}
        SECTION("partial_sort_copy") {}
        SECTION("stable_sort") {}
        SECTION("nth_element") {}
    }

    SECTION("binary search operations") {
        SECTION("lower_bound") {}
        SECTION("upper_bound") {}
        SECTION("binary_search") {}
        SECTION("equal_range") {}
    }

    SECTION("set operations") {
        SECTION("merge") {}
        SECTION("inplace_merge") {}
        SECTION("includes") {}
        SECTION("set_difference") {}
        SECTION("set_intersection") {}
        SECTION("set_symmetric_difference") {}
        SECTION("set_union") {}
    }

    SECTION("heap operations") {
        SECTION("is_heap") {}
        SECTION("is_heap_until") {}
        SECTION("make_heap") {}
        SECTION("push_heap") {}
        SECTION("pop_heap") {}
        SECTION("sort_heap") {}
    }

    SECTION("minimum/maximum operations") {
        SECTION("clamp") {}
        SECTION("max") {}
        SECTION("max_element") {}
        SECTION("min") {}
        SECTION("min_element") {}
        SECTION("minmax") {}
        SECTION("minmax_element") {}
        SECTION("lexicographical_compare") {}
        SECTION("is_permutation") {}
        SECTION("next_permutation") {}
        SECTION("prev_permutation") {}
    }*/
}
