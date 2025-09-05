//
// Created by Terae on 03/01/18.
//

#include "catch.hpp"

#if defined(TEST_SINGLE_HEADER_FILE)
    #include "graph.hpp"
#else
    #include "detail.hpp"
    #include <map>
#endif

TEST_CASE("detail") {
    using namespace std;
    using namespace detail;

    SECTION("exceptions") {
        CHECK_THROWS_WITH(throw detail::invalid_argument::create("foo"),
                          "[graph.exception.invalid_argument] Invalid argument when calling 'foo'.");
        CHECK_THROWS_WITH(throw detail::unexpected_nullptr::create("foo"),
                          "[graph.exception.unexpected_nullptr] Unexpected nullptr when calling 'foo'.");
        CHECK_THROWS_WITH(throw detail::parse_error::create("foo", 0),
                          "[graph.exception.parse_error] parse error: Bad format when calling 'foo'.");
        CHECK_THROWS_WITH(throw detail::parse_error::create("foo", 42),
                          "[graph.exception.parse_error] parse error at 42: Bad format when calling 'foo'.");
        CHECK_THROWS_WITH(throw detail::negative_edge::create("foo"),
                          "[graph.exception.bad_graph.negative_edge] Edge with negative weight when calling 'foo'.");
        CHECK_THROWS_WITH(throw detail::not_complete::create("foo"),
                          "[graph.exception.bad_graph.not_complete] Not complete graph when calling 'foo'.");
    }

    SECTION("is_map_iterator<T>()") {
        CHECK_FALSE(is_map_iterator<int>::value);
        CHECK_FALSE(is_map_iterator<vector<int>::iterator>::value);
        CHECK      (is_map_iterator<map<string, int>::iterator>::value);
    }

    SECTION("get_value(const V&, const V&)") {
        map<string, int*> m;
        int* ptr = new int(1);
        auto it = m.emplace("a", ptr).first;
        CHECK(is_same<decltype(get_value(it, m.end())), int*>::value);
        CHECK(get_value(it, m.end())      == ptr);
        CHECK(get_value(m.end(), m.end()) == nullptr);
        delete ptr;
    }

    SECTION("type_name<T>()") {
        CHECK(type_name<int>()                == "int");
        CHECK(type_name<float>()              == "float");
        CHECK(type_name<string>()             == "std::string");
        CHECK(type_name<map<string, int >> () == "std::map<std::string, int, std::less<std::string>, std::allocator<std::pair<std::string const, int> > >");
        CHECK(type_name<decltype(2.4f)>()     == "float");
        CHECK(type_name<decltype(2.4)>()      == "double");
        CHECK(type_name<std::size_t>()        == "unsigned long");
    }

    SECTION("basic_degree") {
        basic_degree<DIRECTED> d1(make_pair<std::size_t, std::size_t>(1, 2));
        CHECK(d1 == make_pair<std::size_t, std::size_t>(1, 2));
        CHECK(basic_degree<DIRECTED>::max().get_degree().first == std::numeric_limits<std::size_t>::max());

        basic_degree<UNDIRECTED> d2(0);
        CHECK(d2.get_degree() == 0);
        CHECK(basic_degree<UNDIRECTED>::min() == d2);
    }
}
