///
/// Created by Terae on 09/03/17.
///

#ifndef ROOT_GRAPH_H
#define ROOT_GRAPH_H

#include <algorithm> /// find
#include <fstream>   /// setw, operator<<
#include <iomanip>   /// setw
#include <map>       /// map
#include <queue>     /// queue
#include <vector>    /// vector

#ifdef INCLUDE_JSON_FILE
    #include "../third-party/json/single_include/nlohmann/json.hpp"
#else
    #include <nlohmann/json.hpp>
#endif

/// C++ language standard detection
#if   (defined(__cplusplus) && __cplusplus >= 201703L) || (defined(_MSC_VER)   && _MSC_VER > 1900 && defined(_HAS_CXX17) && _HAS_CXX17 == 1)
    #define GRAPH_HAS_CPP_17
    #define GRAPH_HAS_CPP_14
#elif (defined(__cplusplus) && __cplusplus >= 201402L) || (defined(_HAS_CXX14) && _HAS_CXX14 == 1)
    #define GRAPH_HAS_CPP_14
#endif

#if defined(GRAPH_HAS_CPP_17)
    #include <optional>
#endif

/// exclude unsupported compilers
#if defined(__clang__)
    #if (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__) < 30500
        #error "unsupported Clang version - see https://github.com/terae/structure#supported-compilers"
    #endif
#elif defined(__GNUC__) && !(defined(__ICC) || defined(__INTEL_COMPILER))
    #if (__GNUC__        * 10000 + __GNUC_MINOR__  * 100 + __GNUC_PATCHLEVEL__)  < 50000
        #error "unsupported GCC version - see https://github.com/terae/structure#supported-compilers"
    #endif
#endif

#include "Node.h"

///////////////////////////
///// ADJACENCY GRAPH /////
///////////////////////////

///
/// @brief a generalized class of Graph
///
/// @tparam Key type of the keys. Each element in a Graph is uniquely identified by its key value. Aliased as member type Graph::key_type
/// @tparam T type of graphed value stored into a node. Aliased as member type Graph::graphed_type
/// @tparam Cost type of the cost between nodes.
/// @since version 1.0.0
///
template <class Key, class T, class Cost = std::size_t, Nature Nat = UNDIRECTED>
class graph {
  public:
    class node;
    using Degree = detail::basic_degree<Nat>;
    class search_path;
    class shortest_paths;

  private:
    using PtrNode  = std::shared_ptr<node>;
    using MapNodes = std::map<Key, PtrNode>;

    MapNodes _nodes;
    std::size_t _num_edges = 0;

    const Cost infinity = std::numeric_limits<Cost>::has_infinity ? std::numeric_limits<Cost>::infinity() :
                          std::numeric_limits<Cost>::max();
    class path_comparator;
    struct iterator_comparator;

  public:

    ///
    //! @section exceptions
    ///

    using bad_graph          = detail::bad_graph;
    using exception          = detail::exception;
    using invalid_argument   = detail::invalid_argument;
    using negative_edge      = detail::negative_edge;
    using not_complete       = detail::not_complete;
    using parse_error        = detail::parse_error;
    using unexpected_nullptr = detail::unexpected_nullptr;

    ///
    //! @section container types
    ///

    /// @name container types
    /// The canonic container types to use @ref Graph like any other STL container
    /// @{

    /// the type of elements in a graph container
    using value_type   = std::pair<const Key, PtrNode>;
    /// the type of an element reference
    using reference    = value_type &;
    /// the type of the unique and identifying key
    using key_type     = Key;
    /// the type of stored values on a graph
    using graphed_type = T;
    /// a type to represent container sizes
    using size_type    = std::size_t;

    /// an iterator for a graph container
    using iterator               = typename MapNodes::iterator;
    /// a const iterator for a graph container
    using const_iterator         = typename MapNodes::const_iterator;
    /// a reverse iterator for a graph container
    using reverse_iterator       = typename MapNodes::reverse_iterator;
    /// a const reverse iterator for a graph container
    using const_reverse_iterator = typename MapNodes::const_reverse_iterator;

    /// @}

    ///
    //! @section Iterators
    ///

    iterator begin() noexcept;
    iterator end()   noexcept;

    const_iterator begin()  const noexcept;
    const_iterator cbegin() const noexcept;

    const_iterator end()  const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    reverse_iterator rend()   noexcept;

    const_reverse_iterator rbegin()  const noexcept;
    const_reverse_iterator crbegin() const noexcept;

    const_reverse_iterator rend()  const noexcept;
    const_reverse_iterator crend() const noexcept;

    ///
    //! @section Constructors
    ///

    /// default constructor
    explicit graph();

    /// istream constructor
    explicit graph(std::istream &);

    /// copy constructor
    graph(const graph &);

    /// move constructor
    graph(graph &&) noexcept;

    /// copy assignment operator
    graph &operator=(const graph &);

    /// move assignment operator
    graph &operator=(graph &&);

    virtual ~graph();

    ///
    //! @section Capacity
    ///

    bool empty() const noexcept;

    size_type size() const noexcept;

    size_type max_size() const noexcept;

    ///
    //! @section Element access
    ///

    graphed_type &operator[](const key_type &);
    graphed_type &operator[](key_type &&);

#if defined(GRAPH_HAS_CPP_17)
    const std::optional<graphed_type> operator[](key_type &&) const;
#else
    const graphed_type operator[](key_type &&) const;
#endif

    Cost &operator()(iterator,         iterator);
    Cost &operator()(const key_type &, const key_type &);

#if defined(GRAPH_HAS_CPP_17)
    const std::optional<Cost> operator()(const_iterator,   const_iterator)   const;
    const std::optional<Cost> operator()(const key_type &, const key_type &) const;
#else
    const Cost operator()(const_iterator,   const_iterator)   const;
    const Cost operator()(const key_type &, const key_type &) const;
#endif
    ///
    //! @section Modifiers
    ///

    //! Adders
    /// @return pair<position, new node> insertion
    [[deprecated]] std::pair<iterator, bool> insert(const value_type &);

    iterator insert(const_iterator position, const value_type &);
    iterator insert(const_iterator position, const key_type &, graphed_type &);
    iterator insert(const_iterator position, const key_type &, const node &);

    /// @return pair<position, new node> emplacement
    std::pair<iterator, bool> emplace(const key_type &);
    std::pair<iterator, bool> emplace(const key_type &, const graphed_type &);
    std::pair<iterator, bool> emplace(const key_type &, const node &);

    std::pair<iterator, bool> add_node(const key_type &);
    std::pair<iterator, bool> add_node(const key_type &, const graphed_type &);
    std::pair<iterator, bool> add_node(const key_type &, const node &);

    bool add_edge(const_iterator,   const_iterator,   Cost = std::numeric_limits<Cost>::epsilon());
    bool add_edge(const key_type &, const key_type &, Cost = std::numeric_limits<Cost>::epsilon());

    void link_all_nodes(Cost);

    //! Deleters

    iterator  erase(const_iterator);
    iterator  erase(const_iterator first, const_iterator last);
    size_type erase(const key_type &);

    iterator  del_node (const_iterator);
    iterator  del_nodes(const_iterator first, const_iterator last);
    size_type del_node (const key_type &);

    void clear() noexcept;

    size_type del_edge(const_iterator,   const_iterator);
    size_type del_edge(const key_type &, const key_type &);

    void clear_edges();

    size_type clear_edges(const_iterator);
    size_type clear_edges(const key_type &);

    //! Others
    void swap(graph &);

    ///
    //! @section Functions
    ///

    /// Non-modifying sequence operations
    size_type count(const key_type &) const;

    iterator       find(const key_type &);
    const_iterator find(const key_type &) const;

    ///
    //! @section Operations
    ///

    bool existing_node(const_iterator)   const;
    bool existing_node(const key_type &) const;

    bool existing_edge(const_iterator,   const_iterator)   const;
    bool existing_edge(const key_type &, const key_type &) const;

    inline size_type get_nbr_nodes() const noexcept;
    inline size_type get_nbr_edges() const noexcept;

    inline Nature get_nature() const;

    Degree degree(const_iterator)   const;
    Degree degree(const key_type &) const;

    std::pair<const_iterator, Degree> degree_max() const;
    std::pair<const_iterator, Degree> degree_min() const;

    std::map<key_type, Degree> degrees() const;

    template <class = typename std::enable_if<detail::is_directed<Nat>::value>>
    std::vector<typename node::edge> get_in_edges (const_iterator)   const;
    template <class = typename std::enable_if<detail::is_directed<Nat>::value>>
    inline std::vector<typename node::edge> get_in_edges (const key_type &) const;

    template <class = typename std::enable_if<detail::is_directed<Nat>::value>>
    inline std::vector<typename node::edge> get_out_edges(const_iterator)   const;
    template <class = typename std::enable_if<detail::is_directed  <Nat>::value>>
    inline std::vector<typename node::edge> get_out_edges(const key_type &) const;

    template <class = typename std::enable_if<detail::is_undirected<Nat>::value>>
    inline std::vector<typename node::edge> get_edges    (const_iterator)   const;
    template <class = typename std::enable_if<detail::is_undirected<Nat>::value>>
    inline std::vector<typename node::edge> get_edges    (const key_type &) const;

    template <class = typename std::enable_if<detail::is_directed<Nat>::value>>
    bool is_cyclic() const;

    // TODO
    bool is_isomorphic() const;

    ///
    //! @section Text functions
    ///

    /// Display a JSON representation
    template<class K, class D, class C, Nature N> friend std::ostream &operator<<(std::ostream &, const graph<K, D, C, N> &);
    /// Try to load from JSON and then from FILE
    template<class K, class D, class C, Nature N> friend std::istream &operator>>(std::istream &,       graph<K, D, C, N> &);

    /// Load a graph from a file; .DOT not supported
    void load(const char* filename);

    /// .DOT format manipulation
    /// @param graph_name Optional; accepted characters: [a-zA-Z0-9_-]
    std::unique_ptr<std::string> generate_dot(const std::string &graph_name = "") const;
    void save_to_dot (const char* filename,   const std::string &graph_name = "") const;

    /// .JSON format manipulation
    std::unique_ptr<nlohmann::json> generate_json() const;
    void save_to_json  (const char* filename) const;
    void parse_from_json(std::istream &);
    [[deprecated]] void DEBUG_load_from_json_rust(const char*);

    /// graph format manipulation
    std::unique_ptr<std::string> generate_grp() const;
    void save_to_grp  (const char* filename) const;
    void parse_from_grp(std::istream &);

    ///
    //! @section Bool operators
    ///

    template<class K, class D, class C, Nature N> bool operator==(const graph<K, D, C, N> &) const noexcept;
    template<class K, class D, class C, Nature N> bool operator!=(const graph<K, D, C, N> &) const noexcept;

    /// CRTP: https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
    class node : public basic_node<graphed_type, Cost, iterator, const_iterator> {
      public:
        explicit node();

        explicit node(const graphed_type &);

        node &operator=(const graphed_type &);

      private:
        friend class graph;

        void set_iterator_values(iterator this_, iterator end, const_iterator cend);
    };

    ///
    //! @section Search algorithms
    ///

    ///
    /// @brief Breadth-First Search class
    /// @see https://en.wikipedia.org/wiki/Breadth-First_Search
    /// @since version 1.1
    ///
    search_path bfs(key_type       start, key_type                            target)      const;
    search_path bfs(key_type       start, std::list<key_type>                 target_list) const;
    search_path bfs(key_type       start, std::function<bool(key_type)>       is_goal)     const;

    search_path bfs(const_iterator start, const_iterator                      target)      const;
    search_path bfs(const_iterator start, std::list<const_iterator>           target_list) const;
    search_path bfs(const_iterator start, std::function<bool(const_iterator)> is_goal)     const;

    ///
    /// @brief Depth-First Search class
    /// @see https://en.wikipedia.org/wiki/Depth-First_Search
    /// @since version 1.1
    ///
    search_path dfs(key_type       start, key_type                            target)      const;
    search_path dfs(key_type       start, std::list<key_type>                 target_list) const;
    search_path dfs(key_type       start, std::function<bool(key_type)>       is_goal)     const;

    search_path dfs(const_iterator start, const_iterator                      target)      const;
    search_path dfs(const_iterator start, std::list<const_iterator>           target_list) const;
    search_path dfs(const_iterator start, std::function<bool(const_iterator)> is_goal)     const;

    ///
    /// @brief Depth-Limited Search class
    /// @see https://en.wikipedia.org/wiki/Iterative_Deepening_Depth-First_Search
    /// @param depth Predetermined depth limit to create a Depth-Limited Search (fix DFS's loop problem). Have to be well choose, in function of the problem.
    /// @since version 1.1
    ///
    search_path dls(key_type       start, key_type                            target,      size_type depth) const;
    search_path dls(key_type       start, std::list<key_type>                 target_list, size_type depth) const;
    search_path dls(key_type       start, std::function<bool(key_type)>       is_goal,     size_type depth) const;

    search_path dls(const_iterator start, const_iterator                      target,      size_type depth) const;
    search_path dls(const_iterator start, std::list<const_iterator>           target_list, size_type depth) const;
    search_path dls(const_iterator start, std::function<bool(const_iterator)> is_goal,     size_type depth) const;

    ///
    /// @brief Iterative-Deepening Depth-First Search
    /// @see https://en.wikipedia.org/wiki/Iterative_Deepening_Depth-First_Search
    /// @since version 1.1
    ///
    search_path iddfs(key_type       start, key_type                            target)      const;
    search_path iddfs(key_type       start, std::list<key_type>                 target_list) const;
    search_path iddfs(key_type       start, std::function<bool(key_type)>       is_goal)     const;

    search_path iddfs(const_iterator start, const_iterator                      target)      const;
    search_path iddfs(const_iterator start, std::list<const_iterator>           target_list) const;
    search_path iddfs(const_iterator start, std::function<bool(const_iterator)> is_goal)     const;

    ///
    /// @brief Uniform-Cost Search
    /// @see https://en.wikipedia.org/wiki/Talk:Uniform-Cost_Search
    /// @since version 1.1
    ///
    search_path ucs(key_type       start, key_type                            target)      const;
    search_path ucs(key_type       start, std::list<key_type>                 target_list) const;
    search_path ucs(key_type       start, std::function<bool(key_type)>       is_goal)     const;

    search_path ucs(const_iterator start, const_iterator                      target)      const;
    search_path ucs(const_iterator start, std::list<const_iterator>           target_list) const;
    search_path ucs(const_iterator start, std::function<bool(const_iterator)> is_goal)     const;

    ///
    /// @brief A* Search
    /// @see https://en.wikipedia.org/wiki/A*_search_algorithm
    /// @since version 1.1
    ///
    search_path astar(key_type       start, key_type                            target,      std::function<Cost(const_iterator)> heuristic) const;
    search_path astar(key_type       start, std::list<key_type>                 target_list, std::function<Cost(const_iterator)> heuristic) const;
    search_path astar(key_type       start, std::function<bool(key_type)>       is_goal,     std::function<Cost(const_iterator)> heuristic) const;

    search_path astar(const_iterator start, const_iterator                      target,      std::function<Cost(const_iterator)> heuristic) const;
    search_path astar(const_iterator start, std::list<const_iterator>           target_list, std::function<Cost(const_iterator)> heuristic) const;
    search_path astar(const_iterator start, std::function<bool(const_iterator)> is_goal,     std::function<Cost(const_iterator)> heuristic) const;

    ///
    /// @brief Dijkstra Search
    /// @see https://en.wikipedia.org/wiki/dijkstra%27s_algorithm
    /// @since version 1.1
    ///
    shortest_paths dijkstra(key_type       start)                                                  const;
    shortest_paths dijkstra(key_type       start, key_type                            target)      const;
    shortest_paths dijkstra(key_type       start, std::list<key_type>                 target_list) const;
    shortest_paths dijkstra(key_type       start, std::function<bool(key_type)>       is_goal)     const;

    shortest_paths dijkstra(const_iterator start)                                                  const;
    shortest_paths dijkstra(const_iterator start, const_iterator                      target)      const;
    shortest_paths dijkstra(const_iterator start, std::list<const_iterator>           target_list) const;
    shortest_paths dijkstra(const_iterator start, std::function<bool(const_iterator)> is_goal)     const;

    class search_path final : private std::deque<std::pair<graph::const_iterator, Cost>> {
        template <bool> friend search_path graph::abstract_first_search(graph::const_iterator, std::function<bool(const_iterator)>)                             const;
        friend search_path        graph::dls                  (graph::const_iterator, std::function<bool(const_iterator)>, size_type)                           const;
        friend search_path        graph::iddfs                (graph::const_iterator, std::function<bool(const_iterator)>)                                      const;
        friend search_path        graph::ucs                  (graph::const_iterator, std::function<bool(const_iterator)>)                                      const;
        friend search_path        graph::astar                (graph::const_iterator, std::function<bool(const_iterator)>, std::function<Cost(const_iterator)>) const;

        friend class shortest_paths;

        friend bool path_comparator::operator()(const search_path &, const search_path &) const;

        using Container = std::deque<std::pair<graph::const_iterator, Cost>>;

      public:
        using value_type             = typename Container::value_type;
        using reference              = typename Container::reference;
        using const_reference        = typename Container::const_reference;
        using iterator               = typename Container::iterator;
        using const_iterator         = typename Container::const_iterator;
        using reverse_iterator       = typename Container::reverse_iterator;
        using const_reverse_iterator = typename Container::const_reverse_iterator;

        using Container::begin;
        using Container::cbegin;
        using Container::rbegin;
        using Container::crbegin;
        using Container::end;
        using Container::cend;
        using Container::rend;
        using Container::crend;

        search_path() = default;
        search_path(const search_path &);
        virtual ~search_path() = default;

        using Container::empty;
        using Container::size;
        using Container::clear;
        using Container::front;
        using Container::pop_front;
        using Container::swap;

        using Container::push_back;
        using Container::emplace_back;

        Cost total_cost() const;

        bool contain(const graph::const_iterator &) const;

        friend std::ostream &operator<<(std::ostream &os, const typename graph::search_path &sp) {
            Cost count{};
            for (const std::pair<typename graph<Key, T, Cost, Nat>::const_iterator, Cost> &p : sp) {
                os << "-> " << p.first->first << " (" << (count += p.second) << ") ";
            }
            return os;
        }
    };

    ///
    /// @brief Return data of shortest paths from a single source node to all the other nodes
    ///
    /// Data structure as a map<destination_node, pair<previous_node, distance>>
    /// Used by Dijkstra and Bellman-Ford algorithms
    ///
    /// @since version 1.1
    ///
    class shortest_paths final : private std::map<graph::const_iterator, std::pair<graph::const_iterator, Cost>, iterator_comparator> {
        graph::const_iterator _start;

        friend shortest_paths graph::dijkstra(graph::const_iterator, std::function<bool(const_iterator)>) const;

        using Container = std::map<graph::const_iterator, std::pair<graph::const_iterator, Cost>, iterator_comparator>;

        shortest_paths(graph::const_iterator start);

      public:
        using value_type             = typename Container::value_type;
        using mapped_type            = typename Container::mapped_type;
        using reference              = typename Container::reference;
        using const_reference        = typename Container::const_reference;
        using iterator               = typename Container::iterator;
        using const_iterator         = typename Container::const_iterator;
        using reverse_iterator       = typename Container::reverse_iterator;
        using const_reverse_iterator = typename Container::const_reverse_iterator;

        using Container::begin;
        using Container::cbegin;
        using Container::rbegin;
        using Container::crbegin;
        using Container::end;
        using Container::cend;
        using Container::rend;
        using Container::crend;

        shortest_paths(const shortest_paths &);
        virtual ~shortest_paths() = default;

        using Container::empty;
        using Container::size;

        //! @return the father of current in the optimal path from _start to current
        inline graph::const_iterator get_previous(graph::const_iterator current) const;

        //! @return the re-build path from the start node to the target
        search_path get_path(graph::key_type target) const;
        search_path get_path(graph::const_iterator target) const;

        friend std::ostream &operator<<(std::ostream &os, const shortest_paths &sp) {
            for (auto p : sp) {
                os << sp.get_path(p.first) << std::endl;
            }
        }
    };

  private:
    //! Helper functions and classes
    class path_comparator {
      private:
        std::function<Cost(const_iterator)> _heuristic;

      public:
        explicit path_comparator(std::function<Cost(const_iterator)> heuristic);

        bool operator() (const search_path &, const search_path &) const;
    };

    struct iterator_comparator {
        bool operator()(const const_iterator &, const const_iterator &) const;
    };

    bool is_cyclic_rec(const_iterator current, std::list<const_iterator> path) const;

    /// @tparam insertFront Specialization parameter between dfs (`true`) and bfs (`false`) using respectively a `std::stack` and a `std::queue`
    template <bool insertFront> search_path abstract_first_search(const_iterator start, std::function<bool(const_iterator)> is_goal)     const;
};

template <class Key, class T, class Cost = std::size_t>
using graph_directed   = graph<Key, T, Cost, DIRECTED>;

template <class Key, class T, class Cost = std::size_t>
using graph_undirected = graph<Key, T, Cost, UNDIRECTED>;

#include "Graph.cpp"

#endif /// ROOT_GRAPH_H
