///
/// Created by Terae on 09/03/17.
///

#ifndef ROOT_GRAPH_H
#define ROOT_GRAPH_H

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

// C++ language standard detection
#if (defined(__cplusplus) && __cplusplus >= 202302L) || \
    (defined(_MSVC_LANG) && _MSVC_LANG >= 202302L)
    #define GRAPH_HAS_CPP_23
    #define GRAPH_HAS_CPP_20
    #define GRAPH_HAS_CPP_17
    #define GRAPH_HAS_CPP_14
#elif (defined(__cplusplus) && __cplusplus >= 202002L) || \
    (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
    #define GRAPH_HAS_CPP_20
    #define GRAPH_HAS_CPP_17
    #define GRAPH_HAS_CPP_14
#elif (defined(__cplusplus) && __cplusplus >= 201703L) || \
    (defined(_MSC_VER) && _MSC_VER > 1900 && defined(_HAS_CXX17) && _HAS_CXX17 == 1)
    #define GRAPH_HAS_CPP_17
    #define GRAPH_HAS_CPP_14
#elif (defined(__cplusplus) && __cplusplus >= 201402L) || \
    (defined(_HAS_CXX14) && _HAS_CXX14 == 1)
    #define GRAPH_HAS_CPP_14
#endif

// Include optional for C++17 and later
#if defined(GRAPH_HAS_CPP_17)
    #include <optional>
#endif

// Include concepts for C++20 and later
#if defined(GRAPH_HAS_CPP_20)
#include <concepts>

/// @brief Concept for types that can be used as graph keys
/// Requires that the type is comparable and copyable
template<typename T>
concept GraphKey = std::copyable<T> && std::equality_comparable<T> && std::totally_ordered<T>;

/// @brief Concept for types that can be used as graph data
/// Requires that the type is copyable
template<typename T>
concept GraphData = std::copyable<T>;

/// @brief Concept for types that can be used as graph costs
/// Requires that the type is numeric and comparable
template<typename T>
concept GraphCost = std::copyable<T> && std::equality_comparable<T> && std::totally_ordered<T> && requires(T t) {
    t + t;
    t - t;
    t * t;
    t / t;
};
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

/**
 * @brief A generalized class of Graph
 *
 * This class implements a graph data structure that can be either directed
 * or undirected. It supports various operations including node and edge
 * manipulation, graph traversal algorithms, and serialization.
 *
 * The graph is implemented as an adjacency list where each node contains
 * a list of its adjacent nodes and the cost of the connecting edges.
 *
 * @tparam Key Type of the keys. Each element in a Graph is uniquely identified
 *             by its key value. Aliased as member type Graph::key_type
 * @tparam T Type of graphed value stored into a node.
 *           Aliased as member type Graph::graphed_type
 * @tparam Cost Type of the cost between nodes. Default is std::size_t
 * @tparam Nat Nature of the graph (DIRECTED or UNDIRECTED). Default is UNDIRECTED
 *
 * @since version 1.0.0
 */
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

    const Cost infinity = std::numeric_limits<cost_type>::has_infinity ? std::numeric_limits<Cost>::infinity() :
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

    /// @name Container types
    /// The canonical container types to use @ref Graph like any other STL container
    /// @{

    // Concept validation for better error messages in C++20+
#if defined(GRAPH_HAS_CPP_20)
    static_assert(GraphKey<Key>, "Key type must satisfy GraphKey concept (copyable, equality_comparable, and totally_ordered)");
    static_assert(GraphData<T>, "T type must satisfy GraphData concept (copyable)");
    static_assert(GraphCost<Cost>, "Cost type must satisfy GraphCost concept (copyable, equality_comparable, totally_ordered, and supports arithmetic operations)");
#endif

    /// The type of elements in a graph container
    using value_type   = std::pair<const Key, PtrNode>;
    /// The type of element reference
    using reference    = value_type &;
    /// The type of the unique and identifying key
    using key_type     = Key;
    /// The type of stored values on a graph
    using graphed_type = T;
    /// The type of edge cost value
    using cost_type    = Cost;
    /// A type to represent container sizes
    using size_type    = std::size_t;

    /// An iterator for a graph container
    using iterator               = typename MapNodes::iterator;
    /// A const iterator for a graph container
    using const_iterator         = typename MapNodes::const_iterator;
    /// A reverse iterator for a graph container
    using reverse_iterator       = typename MapNodes::reverse_iterator;
    /// A const reverse iterator for a graph container
    using const_reverse_iterator = typename MapNodes::const_reverse_iterator;

    /// @}

    ///
    /// @name Iterators
    /// Methods to access iterators for traversing the graph
    /// @{

    /**
     * @brief Returns an iterator to the first element of the graph
     * @return Iterator to the first element
     */
    iterator begin() noexcept;

    /**
     * @brief Returns an iterator to the element following the last element of the graph
     * @return Iterator to the element following the last element
     */
    iterator end() noexcept;

    /**
     * @brief Returns a const iterator to the first element of the graph
     * @return Const iterator to the first element
     */
    const_iterator begin() const noexcept;

    /**
     * @brief Returns a const iterator to the first element of the graph
     * @return Const iterator to the first element
     */
    const_iterator cbegin() const noexcept;

    /**
     * @brief Returns a const iterator to the element following the last element of the graph
     * @return Const iterator to the element following the last element
     */
    const_iterator end() const noexcept;

    /**
     * @brief Returns a const iterator to the element following the last element of the graph
     * @return Const iterator to the element following the last element
     */
    const_iterator cend() const noexcept;

    /**
     * @brief Returns a reverse iterator to the first element of the reversed graph
     * @return Reverse iterator to the first element
     */
    reverse_iterator rbegin() noexcept;

    /**
     * @brief Returns a reverse iterator to the element following the last element of the reversed graph
     * @return Reverse iterator to the element following the last element
     */
    reverse_iterator rend() noexcept;

    /**
     * @brief Returns a const reverse iterator to the first element of the reversed graph
     * @return Const reverse iterator to the first element
     */
    const_reverse_iterator rbegin() const noexcept;

    /**
     * @brief Returns a const reverse iterator to the first element of the reversed graph
     * @return Const reverse iterator to the first element
     */
    const_reverse_iterator crbegin() const noexcept;

    /**
     * @brief Returns a const reverse iterator to the element following the last element of the reversed graph
     * @return Const reverse iterator to the element following the last element
     */
    const_reverse_iterator rend() const noexcept;

    /**
     * @brief Returns a const reverse iterator to the element following the last element of the reversed graph
     * @return Const reverse iterator to the element following the last element
     */
    const_reverse_iterator crend() const noexcept;

    /// @}

    ///
    /// @name Constructors and Destructor
    /// Methods for creating and destroying graph objects
    /// @{

    /**
     * @brief Default constructor
     *
     * Creates an empty graph with no nodes or edges.
     */
    explicit graph();

    /**
     * @brief Construct a graph from an input stream
     *
     * Creates a graph by parsing data from the provided input stream.
     *
     * @param is Input stream containing graph data
     */
    explicit graph(std::istream& is);

    /**
     * @brief Copy constructor
     *
     * Creates a new graph as a copy of an existing graph.
     *
     * @param other Graph to copy
     */
    graph(const graph& other);

    /**
     * @brief Move constructor
     *
     * Creates a new graph by moving the contents of an existing graph.
     * The moved-from graph is left in a valid but unspecified state.
     *
     * @param other Graph to move from
     */
    graph(graph&& other) noexcept;

    /**
     * @brief Copy assignment operator
     *
     * Assigns the contents of one graph to another by copying.
     *
     * @param other Graph to copy from
     * @return Reference to this graph
     */
    graph &operator=(const graph& other);

    /**
     * @brief Move assignment operator
     *
     * Assigns the contents of one graph to another by moving.
     * The moved-from graph is left in a valid but unspecified state.
     *
     * @param other Graph to move from
     * @return Reference to this graph
     */
    graph &operator=(graph&& other) noexcept;

    /**
     * @brief Virtual destructor
     *
     * Destroys the graph and frees all associated resources.
     */
    virtual ~graph();

    /// @}

    ///
    /// @name Capacity
    /// Methods for querying the size of the graph
    /// @{

    /**
     * @brief Checks whether the graph is empty
     *
     * @return true if the graph contains no nodes, false otherwise
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns the number of nodes in the graph
     *
     * @return The number of nodes in the graph
     */
    [[nodiscard]] size_type size() const noexcept;

    /**
     * @brief Returns the maximum possible number of nodes in the graph
     *
     * @return Maximum possible number of nodes
     */
    [[nodiscard]] size_type max_size() const noexcept;

    /// @}

    ///
    /// @name Element Access
    /// Methods for accessing elements in the graph
    /// @{

    /**
     * @brief Access or insert a node with the specified key
     *
     * If a node with the given key exists, returns a reference to its data.
     * Otherwise, creates a new node with default-constructed data.
     *
     * @param key Key of the node to access
     * @return Reference to the node's data
     */
    graphed_type &operator[](const key_type& key);

    /**
     * @brief Access or insert a node with the specified key (move version)
     *
     * If a node with the given key exists, returns a reference to its data.
     * Otherwise, creates a new node with default-constructed data.
     *
     * @param key Key of the node to access (may be moved from)
     * @return Reference to the node's data
     */
    graphed_type &operator[](key_type&& key);

#if defined(GRAPH_HAS_CPP_17)
    /**
     * @brief Access a node with the specified key (const version)
     *
     * If a node with the given key exists, returns an optional containing its data.
     * Otherwise, returns an empty optional.
     *
     * @param key Key of the node to access
     * @return Optional containing the node's data or empty if not found
     */
    std::optional<graphed_type> operator[](key_type&& key) const;
#else
    /**
     * @brief Access a node with the specified key (const version)
     *
     * If a node with the given key exists, returns its data.
     * Otherwise, throws an invalid_argument exception.
     *
     * @param key Key of the node to access
     * @return Copy of the node's data
     * @throws invalid_argument if node with key does not exist
     */
    graphed_type operator[](key_type&& key) const;
#endif

    /**
     * @brief Access the cost of an edge between two nodes (iterator version)
     *
     * If an edge between the nodes exists, returns a reference to its cost.
     * Otherwise, creates a new edge with infinite cost.
     *
     * @param from Iterator to the source node
     * @param to Iterator to the target node
     * @return Reference to the edge cost
     */
    cost_type &operator()(iterator from, iterator to);

    /**
     * @brief Access the cost of an edge between two nodes (key version)
     *
     * If an edge between the nodes exists, returns a reference to its cost.
     * Otherwise, creates a new edge with infinite cost.
     *
     * @param from_key Key of the source node
     * @param to_key Key of the target node
     * @return Reference to the edge cost
     */
    cost_type &operator()(const key_type& from_key, const key_type& to_key);

#if defined(GRAPH_HAS_CPP_17)
    /**
     * @brief Access the cost of an edge between two nodes (const iterator version)
     *
     * If an edge between the nodes exists, returns an optional containing its cost.
     * Otherwise, returns an empty optional.
     *
     * @param from Iterator to the source node
     * @param to Iterator to the target node
     * @return Optional containing the edge cost or empty if edge doesn't exist
     */
    std::optional<cost_type> operator()(const_iterator from, const_iterator to) const;

    /**
     * @brief Access the cost of an edge between two nodes (const key version)
     *
     * If an edge between the nodes exists, returns an optional containing its cost.
     * Otherwise, returns an empty optional.
     *
     * @param from_key Key of the source node
     * @param to_key Key of the target node
     * @return Optional containing the edge cost or empty if edge doesn't exist
     */
    std::optional<cost_type> operator()(const key_type& from_key, const key_type& to_key) const;
#else
    /**
     * @brief Access the cost of an edge between two nodes (const iterator version)
     *
     * If an edge between the nodes exists, returns its cost.
     * Otherwise, throws an invalid_argument exception.
     *
     * @param from Iterator to the source node
     * @param to Iterator to the target node
     * @return Copy of the edge cost
     * @throws invalid_argument if edge doesn't exist
     */
    cost_type operator()(const_iterator from, const_iterator to) const;

    /**
     * @brief Access the cost of an edge between two nodes (const key version)
     *
     * If an edge between the nodes exists, returns its cost.
     * Otherwise, throws an invalid_argument exception.
     *
     * @param from_key Key of the source node
     * @param to_key Key of the target node
     * @return Copy of the edge cost
     * @throws invalid_argument if edge doesn't exist
     */
    cost_type operator()(const key_type& from_key, const key_type& to_key) const;
#endif

    /// @}
    ///
    /// @name Modifiers
    /// Methods for modifying the graph
    /// @{

    /// @name Adders
    /// Methods for adding nodes and edges to the graph
    /// @{

    /**
     * @brief Insert a node (deprecated)
     * @deprecated Use add_node instead
     */
    [[deprecated]] std::pair<iterator, bool> insert(const value_type &);

    /**
     * @brief Insert a node at a specific position
     *
     * @param position Hint for where to insert the node
     * @param value Node to insert
     * @return Iterator to the inserted node
     */
    iterator insert(const_iterator position, const value_type &);

    /**
     * @brief Insert a node with specified key and data
     *
     * @param position Hint for where to insert the node
     * @param key Key for the new node
     * @param data Data for the new node
     * @return Iterator to the inserted node
     */
    iterator insert(const_iterator position, const key_type &, graphed_type &);

    /**
     * @brief Insert a node with specified key and node
     *
     * @param position Hint for where to insert the node
     * @param key Key for the new node
     * @param node Node to insert
     * @return Iterator to the inserted node
     */
    iterator insert(const_iterator position, const key_type &, const node &);

    /**
     * @brief Emplace a node with the specified key
     *
     * Constructs a new node in-place with the given key.
     *
     * @param key Key for the new node
     * @return Pair containing iterator to the node and boolean indicating if insertion occurred
     */
    std::pair<iterator, bool> emplace(const key_type &);

    /**
     * @brief Emplace a node with the specified key and data
     *
     * Constructs a new node in-place with the given key and data.
     *
     * @param key Key for the new node
     * @param data Data for the new node
     * @return Pair containing iterator to the node and boolean indicating if insertion occurred
     */
    std::pair<iterator, bool> emplace(const key_type &, const graphed_type &);

    /**
     * @brief Emplace a node with the specified key and node
     *
     * Constructs a new node in-place with the given key and node.
     *
     * @param key Key for the new node
     * @param node Node to emplace
     * @return Pair containing iterator to the node and boolean indicating if insertion occurred
     */
    std::pair<iterator, bool> emplace(const key_type &, const node &);

    /**
     * @brief Add a node with the specified key
     *
     * If a node with the given key doesn't exist, creates a new node with default data.
     *
     * @param key Key for the node
     * @return Pair containing iterator to the node and boolean indicating if insertion occurred
     */
    std::pair<iterator, bool> add_node(const key_type &);

    /**
     * @brief Add a node with the specified key and data
     *
     * If a node with the given key doesn't exist, creates a new node with the given data.
     *
     * @param key Key for the node
     * @param data Data for the node
     * @return Pair containing iterator to the node and boolean indicating if insertion occurred
     */
    std::pair<iterator, bool> add_node(const key_type &, const graphed_type &);

    /**
     * @brief Add a node with the specified key and node
     *
     * If a node with the given key doesn't exist, creates a new node with the given node's data.
     *
     * @param key Key for the node
     * @param node Node to copy data from
     * @return Pair containing iterator to the node and boolean indicating if insertion occurred
     */
    std::pair<iterator, bool> add_node(const key_type &, const node &);

    /**
     * @brief Add an edge between two nodes (iterator version)
     *
     * Creates an edge between the nodes pointed to by the iterators.
     *
     * @param from Iterator to the source node
     * @param to Iterator to the target node
     * @param cost Cost of the edge (default: epsilon)
     * @return true if edge was added, false if it already existed
     */
    bool add_edge(const_iterator from, const_iterator to, cost_type cost = std::numeric_limits<cost_type>::epsilon());

    /**
     * @brief Add an edge between two nodes (key version)
     *
     * Creates an edge between the nodes with the given keys.
     *
     * @param from_key Key of the source node
     * @param to_key Key of the target node
     * @param cost Cost of the edge (default: epsilon)
     * @return true if edge was added, false if it already existed
     */
    bool add_edge(const key_type& from_key, const key_type& to_key, cost_type cost = std::numeric_limits<cost_type>::epsilon());

    /**
     * @brief Make the graph complete with specified edge cost
     *
     * Adds edges between all pairs of nodes with the given cost.
     *
     * @param cost Cost for all edges
     */
    void make_complete(cost_type cost);

    /// @}

    /// @name Deleters
    /// Methods for removing nodes and edges from the graph
    /// @{

    /**
     * @brief Erase a node
     *
     * Removes the node pointed to by the iterator.
     *
     * @param position Iterator to the node to erase
     * @return Iterator following the erased node
     */
    iterator erase(const_iterator position);

    /**
     * @brief Erase a range of nodes
     *
     * Removes the nodes in the range [first, last).
     *
     * @param first Iterator to the first node to erase
     * @param last Iterator to one past the last node to erase
     * @return Iterator following the last erased node
     */
    iterator erase(const_iterator first, const_iterator last);

    /**
     * @brief Erase a node by key
     *
     * Removes the node with the given key.
     *
     * @param key Key of the node to erase
     * @return Number of nodes removed (0 or 1)
     */
    size_type erase(const key_type& key);

    /**
     * @brief Delete a node
     *
     * Removes the node pointed to by the iterator.
     *
     * @param position Iterator to the node to delete
     * @return Iterator following the deleted node
     */
    iterator del_node(const_iterator position);

    /**
     * @brief Delete a range of nodes
     *
     * Removes the nodes in the range [first, last).
     *
     * @param first Iterator to the first node to delete
     * @param last Iterator to one past the last node to delete
     * @return Iterator following the last deleted node
     */
    iterator del_nodes(const_iterator first, const_iterator last);

    /**
     * @brief Delete a node by key
     *
     * Removes the node with the given key.
     *
     * @param key Key of the node to delete
     * @return Number of nodes removed (0 or 1)
     */
    size_type del_node(const key_type& key);

    /**
     * @brief Clear all nodes and edges from the graph
     */
    void clear() noexcept;

    /**
     * @brief Delete an edge between two nodes (iterator version)
     *
     * Removes the edge between the nodes pointed to by the iterators.
     *
     * @param from Iterator to the source node
     * @param to Iterator to the target node
     * @return Number of edges removed (0 or 1)
     */
    size_type del_edge(const_iterator from, const_iterator to);

    /**
     * @brief Delete an edge between two nodes (key version)
     *
     * Removes the edge between the nodes with the given keys.
     *
     * @param from_key Key of the source node
     * @param to_key Key of the target node
     * @return Number of edges removed (0 or 1)
     */
    size_type del_edge(const key_type& from_key, const key_type& to_key);

    /**
     * @brief Clear all edges from the graph
     */
    void clear_edges();

    /**
     * @brief Clear edges from a specific node (iterator version)
     *
     * Removes all edges from the node pointed to by the iterator.
     *
     * @param position Iterator to the node
     * @return Number of edges removed
     */
    size_type clear_edges(const_iterator position);

    /**
     * @brief Clear edges from a specific node (key version)
     *
     * Removes all edges from the node with the given key.
     *
     * @param key Key of the node
     * @return Number of edges removed
     */
    size_type clear_edges(const key_type& key);

    /// @}

    /// @name Other modifiers
    /// Additional modification methods
    /// @{

    /**
     * @brief Swap the contents of two graphs
     *
     * Exchanges the contents of this graph with another graph.
     *
     * @param other Graph to swap with
     */
    void swap(graph& other) noexcept;

    /// @}

    /// @}

    ///
    /// @name Functions
    /// Non-modifying operations on the graph
    /// @{

    /**
     * @brief Count the number of nodes with a specific key
     *
     * @param key Key to count
     * @return Number of nodes with the given key (0 or 1)
     */
    size_type count(const key_type &) const;

    /**
     * @brief Find a node with a specific key
     *
     * @param key Key to search for
     * @return Iterator to the node if found, end() otherwise
     */
    iterator find(const key_type &);

    /**
     * @brief Find a node with a specific key (const version)
     *
     * @param key Key to search for
     * @return Const iterator to the node if found, cend() otherwise
     */
    const_iterator find(const key_type &) const;

    /// @}

    ///
    /// @name Operations
    /// Query operations on the graph
    /// @{

    /**
     * @brief Check if a node exists (iterator version)
     *
     * @param position Iterator to the node to check
     * @return true if the node exists, false otherwise
     */
    bool existing_node(const_iterator position) const;

    /**
     * @brief Check if a node exists (key version)
     *
     * @param key Key of the node to check
     * @return true if the node exists, false otherwise
     */
    bool existing_node(const key_type& key) const;

    /**
     * @brief Check if an edge exists between two nodes (iterator version)
     *
     * @param from Iterator to the source node
     * @param to Iterator to the target node
     * @return true if the edge exists, false otherwise
     */
    bool existing_edge(const_iterator from, const_iterator to) const;

    /**
     * @brief Check if an edge exists between two nodes (key version)
     *
     * @param from_key Key of the source node
     * @param to_key Key of the target node
     * @return true if the edge exists, false otherwise
     */
    bool existing_edge(const key_type& from_key, const key_type& to_key) const;

    /**
     * @brief Check if there exists a path connecting two nodes (iterator version)
     *
     * If from and to are equal, this function returns true.
     *
     * @param from Iterator to the source node
     * @param to Iterator to the target node
     * @return true if a path exists, false otherwise
     */
    bool has_path_connecting(const_iterator from, const_iterator to) const;

    /**
     * @brief Check if there exists a path connecting two nodes (key version)
     *
     * If from_key and to_key are equal, this function returns true.
     *
     * @param from_key Key of the source node
     * @param to_key Key of the target node
     * @return true if a path exists, false otherwise
     */
    bool has_path_connecting(const key_type& from_key, const key_type& to_key) const;

    /**
     * @brief Get the number of nodes in the graph
     *
     * @return Number of nodes
     */
    [[nodiscard]] size_type get_nbr_nodes() const noexcept;

    /**
     * @brief Get the number of edges in the graph
     *
     * @return Number of edges
     */
    [[nodiscard]] size_type get_nbr_edges() const noexcept;

    /**
     * @brief Get the nature of the graph
     *
     * @return Nature of the graph (DIRECTED or UNDIRECTED)
     */
    [[nodiscard]] Nature get_nature() const;

    /**
     * @brief Get the degree of a node (iterator version)
     *
     * @param position Iterator to the node
     * @return Degree of the node
     */
    Degree degree(const_iterator position) const;

    /**
     * @brief Get the degree of a node (key version)
     *
     * @param key Key of the node
     * @return Degree of the node
     */
    Degree degree(const key_type& key) const;

    /**
     * @brief Get the node with maximum degree
     *
     * @return Pair containing iterator to the node and its degree
     */
    std::pair<const_iterator, Degree> degree_max() const;

    /**
     * @brief Get the node with minimum degree
     *
     * @return Pair containing iterator to the node and its degree
     */
    std::pair<const_iterator, Degree> degree_min() const;

    /**
     * @brief Get degrees of all nodes
     *
     * @return Map of node keys to their degrees
     */
    std::map<key_type, Degree> degrees() const;

    /**
     * @brief Get incoming edges of a node (directed graphs only)
     *
     * @tparam enable_if<detail::is_directed<Nat>::value> SFINAE constraint for directed graphs
     * @param position Iterator to the node
     * @return Vector of incoming edges
     */
    template <class = std::enable_if<detail::is_directed<Nat>::value >>
    std::vector<typename node::edge> get_in_edges(const_iterator position) const;

    /**
     * @brief Get incoming edges of a node (directed graphs only, key version)
     *
     * @tparam enable_if<detail::is_directed<Nat>::value> SFINAE constraint for directed graphs
     * @param key Key of the node
     * @return Vector of incoming edges
     */
    template <class = std::enable_if<detail::is_directed<Nat>::value >>
    std::vector<typename node::edge> get_in_edges(const key_type& key) const;

    /**
     * @brief Get outgoing edges of a node (directed graphs only)
     *
     * @tparam enable_if<detail::is_directed<Nat>::value> SFINAE constraint for directed graphs
     * @param position Iterator to the node
     * @return Vector of outgoing edges
     */
    template <class = std::enable_if<detail::is_directed<Nat>::value >>
    std::vector<typename node::edge> get_out_edges(const_iterator position) const;

    /**
     * @brief Get outgoing edges of a node (directed graphs only, key version)
     *
     * @tparam enable_if<detail::is_directed<Nat>::value> SFINAE constraint for directed graphs
     * @param key Key of the node
     * @return Vector of outgoing edges
     */
    template <class = std::enable_if<detail::is_directed<Nat>::value >>
    std::vector<typename node::edge> get_out_edges(const key_type& key) const;

    /**
     * @brief Get edges of a node (undirected graphs only)
     *
     * @tparam enable_if<detail::is_undirected<Nat>::value> SFINAE constraint for undirected graphs
     * @param position Iterator to the node
     * @return Vector of edges
     */
    template <class = std::enable_if<detail::is_undirected<Nat>::value >>
    std::vector<typename node::edge> get_edges(const_iterator position) const;

    /**
     * @brief Get edges of a node (undirected graphs only, key version)
     *
     * @tparam enable_if<detail::is_undirected<Nat>::value> SFINAE constraint for undirected graphs
     * @param key Key of the node
     * @return Vector of edges
     */
    template <class = std::enable_if<detail::is_undirected<Nat>::value >>
    std::vector<typename node::edge> get_edges(const key_type& key) const;

    /**
     * @brief Check if the graph contains cycles (directed graphs only)
     *
     * @tparam enable_if<detail::is_directed<Nat>::value> SFINAE constraint for directed graphs
     * @return true if the graph contains cycles, false otherwise
     */
    template <class = std::enable_if<detail::is_directed<Nat>::value >>
    [[nodiscard]] bool is_cyclic() const;

    /// @}

    /*
    // TODO
    bool is_isomorphic() const;

    /// Perform a topological sort of a directed graph
    /// If the graph was acyclic, return a `vector> of nodes in topological order: each node is ordered before its successors. Otherwise, it throws a `has_cycle` error. Self loops are also cycles.
    // TODO
    std::vector<const_iterator> toposort() const;

    /// Computes the _strongly connected components_ using [Kosaraju's algorithm](https://en.wikipedia.org/wiki/Kosaraju%27s_algorithm)
    /// Return a `set` where each element is a strongly connected component (scc). The order of the scc is their postorder (reverse topological sort)
    /// For an undirected graph, the sccs are simply the connected components
    /// This implementation is iterative and does two passes over the nodes
    // TODO
    std::set<std::vector<const_iterator>, iterator_comparator> kosaraju_scc() const;

    /// Computes the _strongly connected components_ using [Tarjan's algorithm](https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm)
    /// Return a `set` where each element is a strongly connected component (scc). The order of the scc is their postorder (reverse topological sort)
    /// For an undirected graph, the sccs are simply the connected components
    /// This implementation is iterative and does one pass over the nodes
    // TODO
    std::set<std::vector<const_iterator>, iterator_comparator> tarjan_scc() const;

    /// Return the number of connected components of the graph
    /// For a directed graph, this is the _weakly_ connected components
    // TODO
    size_type connected_components() const;

    /// Condenses every strongly connected component into a single node
    /// @param make_acyclic If equals to `true`, self-loops are ignored, guaranteeing that the output is acyclic
    // TODO
    graph &condensate(bool make_acyclic = true);

    /// Computes the [maximum clique](https://en.wikipedia.org/wiki/Clique_(graph_theory)) of the graph, e.g. the clique  such that there is not clique with more nodes
    /// Return a `vector` where each element is in the clique
    // TODO
    std::vector<const_iterator> maximum_clique() const;
    */

    ///
    /// @name Text Functions
    /// Methods for text representation and serialization
    /// @{

    /**
     * @brief Output stream operator
     *
     * Displays a JSON representation of the graph.
     *
     * @tparam K Key type
     * @tparam D Data type
     * @tparam C Cost type
     * @tparam N Nature type
     * @param os Output stream
     * @param g Graph to output
     * @return Reference to the output stream
     */
    template<class K, class D, class C, Nature N>
    friend std::ostream & operator<<(std::ostream& os, const graph<K, D, C, N> &g);

    /**
     * @brief Input stream operator
     *
     * Tries to load from JSON and then from FILE.
     *
     * @tparam K Key type
     * @tparam D Data type
     * @tparam C Cost type
     * @tparam N Nature type
     * @param is Input stream
     * @param g Graph to load into
     * @return Reference to the input stream
     */
    template<class K, class D, class C, Nature N>
    friend std::istream & operator>>(std::istream& is, graph<K, D, C, N> &g);

    /**
     * @brief Load a graph from a file
     *
     * @param filename Path to the file to load
     * @note .DOT format is not supported
     */
    void load(const char* filename);

    /**
     * @brief Generate a DOT representation of the graph
     *
     * @param graph_name Optional name for the graph (accepted characters: [a-zA-Z0-9_-])
     * @return Unique pointer to the DOT string representation
     */
    [[nodiscard]] std::unique_ptr<std::string> generate_dot(const std::string& graph_name = "") const;

    /**
     * @brief Save the graph to a DOT file
     *
     * @param filename Path to the file to save to
     * @param graph_name Optional name for the graph (accepted characters: [a-zA-Z0-9_-])
     */
    void save_to_dot(const char* filename, const std::string& graph_name = "") const;

    /**
     * @brief Generate a JSON representation of the graph
     *
     * @return Unique pointer to the JSON representation
     */
    [[nodiscard]] std::unique_ptr<nlohmann::json> generate_json() const;

    /**
     * @brief Save the graph to a JSON file
     *
     * @param filename Path to the file to save to
     */
    void save_to_json(const char* filename) const;

    /**
     * @brief Parse a graph from a JSON input stream
     *
     * @param is Input stream containing JSON data
     */
    void parse_from_json(std::istream& is);

    /**
     * @brief Debug function to load from JSON Rust format (deprecated)
     * @deprecated Use parse_from_json instead
     */
    [[deprecated]] void DEBUG_load_from_json_rust(const char*);

    /**
     * @brief Generate a GRP representation of the graph
     *
     * @return Unique pointer to the GRP string representation
     */
    [[nodiscard]] std::unique_ptr<std::string> generate_grp() const;

    /**
     * @brief Save the graph to a GRP file
     *
     * @param filename Path to the file to save to
     */
    void save_to_grp(const char* filename) const;

    /**
     * @brief Parse a graph from a GRP input stream
     *
     * @param is Input stream containing GRP data
     */
    void parse_from_grp(std::istream& is);

    /// @}

    ///
    /// @name Boolean Operators
    /// Comparison operators for graphs
    /// @{

    /**
     * @brief Equality operator
     *
     * Compares two graphs for equality.
     *
     * @tparam K Key type of other graph
     * @tparam D Data type of other graph
     * @tparam C Cost type of other graph
     * @tparam N Nature type of other graph
     * @param other Graph to compare with
     * @return true if graphs are equal, false otherwise
     */
    template<class K, class D, class C, Nature N>
    bool operator==(const graph<K, D, C, N> &other) const noexcept;

    /**
     * @brief Inequality operator
     *
     * Compares two graphs for inequality.
     *
     * @tparam K Key type of other graph
     * @tparam D Data type of other graph
     * @tparam C Cost type of other graph
     * @tparam N Nature type of other graph
     * @param other Graph to compare with
     * @return true if graphs are not equal, false otherwise
     */
    template<class K, class D, class C, Nature N>
    bool operator!=(const graph<K, D, C, N> &other) const noexcept;

    /// @}

    /// CRTP: https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
    class node : public basic_node<graphed_type, cost_type, iterator, const_iterator> {
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
    /// @param start
    /// @param target
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
    /// @brief A* shortest path algorithm
    ///
    /// Computes the shortest path from @param start to @param target, including the total path cost.
    /// @param target is implicitly given via the @is_goal call_back, which should return `true` if the given node is the target node.
    /// Edge costs must be non-negative. Use Bellman-Ford's algorithm instead.
    /// The function @param heuristic should return the estimated cost to the target for a particular node. For the algorithm to find the actual shortest path, it should be admissible, meaning that it should never overestimate the actual cost to get nearest goal node. Estimate costs must also be non-negative.
    ///
    /// @see https://en.wikipedia.org/wiki/A*_search_algorithm
    /// @since version 1.1
    ///
    search_path astar(key_type       start, key_type                            target,      std::function<cost_type(const_iterator)> heuristic) const;
    search_path astar(key_type       start, std::list<key_type>                 target_list, std::function<cost_type(const_iterator)> heuristic) const;
    search_path astar(key_type       start, std::function<bool(key_type)>       is_goal,     std::function<cost_type(const_iterator)> heuristic) const;

    search_path astar(const_iterator start, const_iterator                      target,      std::function<cost_type(const_iterator)> heuristic) const;
    search_path astar(const_iterator start, std::list<const_iterator>           target_list, std::function<cost_type(const_iterator)> heuristic) const;
    search_path astar(const_iterator start, std::function<bool(const_iterator)> is_goal,     std::function<cost_type(const_iterator)> heuristic) const;

    ///
    /// @brief Dijkstra Search
    ///
    /// Computes the shortest paths from @param start to every reachable node.
    /// Edge costs must be non-negative.
    /// If a particular target is given, then the algorithm terminates once the goal node's cost is calculated.
    ///
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

    ///
    /// @brief Bellman-Ford Search
    ///
    /// Computes shortest paths from node @param start to every reachable node.
    /// Negative edge costs are permitted, but the graph must not have a cycle of negative weights.
    ///
    /// @see https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
    /// @since version 1.1
    ///
    shortest_paths bellman_ford(key_type       start) const;
    shortest_paths bellman_ford(const_iterator start) const;

    class search_path final : std::deque<std::pair<graph::const_iterator, cost_type >> {
        template <bool> friend search_path graph::abstract_first_search(graph::const_iterator, std::function<bool(const_iterator)>) const;

        friend search_path graph::dls   (graph::const_iterator, std::function<bool(const_iterator)>, size_type)                                const;
        friend search_path graph::iddfs (graph::const_iterator, std::function<bool(const_iterator)>)                                           const;
        friend search_path graph::ucs   (graph::const_iterator, std::function<bool(const_iterator)>)                                           const;
        friend search_path graph::astar (graph::const_iterator, std::function<bool(const_iterator)>, std::function<cost_type(const_iterator)>) const;

        friend class shortest_paths;

        friend bool path_comparator::operator()(const search_path &, const search_path &) const;

        using Container = std::deque<std::pair<graph::const_iterator, cost_type >>;

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
        ~search_path() = default;

        using Container::empty;
        using Container::size;
        using Container::clear;
        using Container::front;
        using Container::pop_front;
        using Container::swap;

        using Container::push_back;
        using Container::emplace_back;

        cost_type total_cost() const;

        bool contain(const graph::const_iterator &) const;

        friend std::ostream &operator << (std::ostream &os, const graph::search_path &sp) {
            cost_type count{};
            for (const std::pair<graph::const_iterator, cost_type> &p : sp) {
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
    class shortest_paths final : std::map<graph::const_iterator, std::pair<graph::const_iterator, cost_type>, iterator_comparator> {
        graph::const_iterator _start;

        friend shortest_paths graph::dijkstra    (graph::const_iterator, std::function<bool(const_iterator)>) const;
        friend shortest_paths graph::bellman_ford(graph::const_iterator)                                      const;

        using Container = std::map<graph::const_iterator, std::pair<graph::const_iterator, cost_type>, iterator_comparator>;

        explicit shortest_paths(graph::const_iterator start);

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
        ~shortest_paths() = default;

        using Container::empty;
        using Container::size;

        //! @return the father of current in the optimal path from _start to current
        graph::const_iterator get_previous(graph::const_iterator current) const;

        //! @return the re-build path from the start node to the target
        search_path get_path(graph::key_type target) const;
        search_path get_path(graph::const_iterator target) const;

        friend std::ostream & operator<<(std::ostream & os, const shortest_paths & sp) {
            for (auto p : sp) {
                os << sp.get_path(p.first) << std::endl;
            }
            return os;
        }
    };

  private:
    //! Helper functions and classes
    class path_comparator : public std::function<bool(search_path, search_path)> {
        std::function<cost_type(const_iterator)> _heuristic;

      public:
        explicit path_comparator(std::function<cost_type(const_iterator)> heuristic);

        bool operator() (const search_path &, const search_path &) const;
    };

    struct iterator_comparator : std::function<bool(const_iterator, const_iterator)> {
        bool operator()(const const_iterator &, const const_iterator &) const;
    };

    bool is_cyclic_rec(const_iterator current, std::list<const_iterator> path) const;

    /// @tparam insertFront Specialization parameter between dfs (`true`) and bfs (`false`) using respectively a `std::stack` and a `std::queue`
    template <bool insertFront> search_path abstract_first_search(const_iterator start, std::function<bool(const_iterator)> is_goal) const;
};

template <class Key, class T, class Cost = std::size_t>
using graph_directed   = graph<Key, T, Cost, DIRECTED>;

template <class Key, class T, class Cost = std::size_t>
using graph_undirected = graph<Key, T, Cost, UNDIRECTED>;

#include "Graph.cpp"

#endif /// ROOT_GRAPH_H
