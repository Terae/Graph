///
/// Created by Terae on 04/04/17.
///

#ifndef ROOT_NODE_H
#define ROOT_NODE_H

#include <iostream> /// cerr, cout
#include <list>     /// list
#include <memory>   /// make_shared, shared_ptr, weak_ptr
#include <tuple>    /// tie, tuple
#include <utility>  /// make_pair, move, pair, swap

#include "detail.hpp"

/**
 * @brief Base class for nodes in a graph
 *
 * This class represents a node in a graph, containing data and connections
 * to other nodes (edges). It is designed to work with the graph class
 * to provide a complete graph data structure.
 *
 * @tparam Data Type of data stored in the node
 * @tparam Cost Type of cost associated with edges
 * @tparam Container Type of container for iterators
 * @tparam constContainer Type of container for const iterators
 */
template <class Data, class Cost, class Container, class constContainer>
class basic_node {
  public:
    /**
     * @brief Represents an edge in the graph
     *
     * An edge connects two nodes and has an associated cost.
     */
    class edge {
        template <class T, class C, class c1, class c2> friend class basic_node;
        template <class K, class T, class C, Nature N> friend class graph;

        std::weak_ptr<basic_node<Data, Cost, Container, constContainer >> _target;
        std::shared_ptr<Cost> _cost;

        /**
         * @brief Create a tuple representation of the edge for comparison
         * @return Tuple containing cost and target node
         */
        std::tuple<Cost, basic_node<Data, Cost, Container, constContainer >> tie() const;

      public:
        /**
         * @brief Construct a new edge object
         * @param ptr Shared pointer to the target node
         * @param cost Cost of the edge
         */
        explicit edge(const std::weak_ptr<basic_node<Data, Cost, Container, constContainer >> &ptr, Cost cost);

        /**
         * @brief Copy constructor
         * @param other Edge to copy
         */
        edge(const edge& other);

        /**
         * @brief Compare edges by cost
         * @param other Edge to compare with
         * @return true if this edge has lower cost than other
         */
        bool operator<(const edge& other) const;

        /**
         * @brief Check equality of edges
         * @param other Edge to compare with
         * @return true if edges are equal
         */
        bool operator==(const edge& other) const;

        /**
         * @brief Get the target node of this edge
         * @return constContainer pointing to the target node
         */
        constContainer target() const;

        /**
         * @brief Get the cost of this edge
         * @return Reference to the cost value
         */
        Cost& cost() const;
    };

    using ListEdges = std::list<edge>;
    using EdgesIterator = typename ListEdges::iterator;
    using ConstEdgesIterator = typename ListEdges::const_iterator;

  private:
    template <class K, class T, class C, Nature N> friend class graph;

    ListEdges _out_edges;

    const Cost infinity = std::numeric_limits<Cost>::has_infinity ? std::numeric_limits<Cost>::infinity() :
                          std::numeric_limits<Cost>::max();

    std::size_t _in_degree{0};

    void increment_in_degree(int n = 1);
    void decrement_in_degree(int n = 1);

    //! used for UNDIRECTED graphs: same Cost in memory for both directions
    bool set_edge(constContainer other, std::shared_ptr<Cost> cost);

    std::tuple<Data, std::size_t, ListEdges> tie() const;

    ListEdges get_edges() const;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator==(const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator!=(const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator< (const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator<=(const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator> (const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator>=(const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    basic_node(const basic_node &n);

    basic_node &operator=(const basic_node &n);

  protected:
    Data _data;

    Container      container_from_this; /// position of this on the container
    Container      end_container;       /// last position of the container (nullptr or end())
    constContainer cend_container;      /// last position of the container (nullptr or cend())

  public:
    ///
    //! @section exceptions
    ///
    using exception          = detail::exception;
    using invalid_argument   = detail::invalid_argument;
    using unexpected_nullptr = detail::unexpected_nullptr;

    ///
    /// @name Constructors and Destructor
    /// @{

    /**
     * @brief Default constructor
     */
    explicit basic_node();

    /**
     * @brief Construct with data
     * @param data Data to store in the node
     */
    explicit basic_node(const Data& data);

    /**
     * @brief Move assignment operator (deleted)
     */
    basic_node &operator=(basic_node&& n) = delete;

    /**
     * @brief Destructor
     */
    ~basic_node();

    /// @}

    ///
    /// @name Element Access
    /// @{

    /**
     * @brief Get reference to node data
     * @return Reference to stored data
     */
    Data &get();

    /**
     * @brief Get copy of node data (const version)
     * @return Copy of stored data
     */
    Data get() const;

    /**
     * @brief Get reference to edge cost
     * @param other Iterator to target node
     * @return Reference to edge cost
     */
    Cost &get_cost(Container other);

    /**
     * @brief Get copy of edge cost (const version)
     * @param other Const iterator to target node
     * @return Copy of edge cost
     */
    Cost get_cost(constContainer other) const;

    /**
     * @brief Access edge cost with operator[]
     * @param other Iterator to target node
     * @return Reference to edge cost
     */
    Cost &operator[](Container other);

    /**
     * @brief Access edge cost with operator[] (const version)
     * @param other Const iterator to target node
     * @return Copy of edge cost
     */
    Cost operator[](constContainer other) const;

    /// @}

    ///
    /// @name Modifiers
    /// @{

    /**
     * @brief Set node data
     * @tparam T_data Type of data to set
     * @param data Data to set
     */
    template<class T_data>
    void set(const T_data& data);

    /**
     * @brief Set edge cost
     * @tparam T_cost Type of cost to set
     * @param other Iterator to target node
     * @param cost Cost to set
     */
    template<class T_cost>
    void set_cost(Container other, const T_cost& cost);

    /**
     * @brief Add an edge to another node
     * @param other Const iterator to target node
     * @param cost Cost of the edge (default: 1)
     * @return Pair containing iterator to edge and boolean indicating if new edge was created
     */
    std::pair<EdgesIterator, bool> add_edge(constContainer other, Cost cost = Cost(1)) {
        std::shared_ptr<basic_node<Data, Cost, Container, constContainer >> ptr{
            detail::get_value(other, cend_container)
        };

        if (ptr == nullptr) {
            GRAPH_THROW(unexpected_nullptr)
        }

        for (EdgesIterator it{_out_edges.begin()}; it != _out_edges.end(); ++it) {
            if (it->_target.lock() == ptr) {
                it->cost() = cost;
                return std::make_pair(it, false);
            }
        }

        // Link doesn't exist
        ptr->increment_in_degree();
        _out_edges.emplace_back(
            std::weak_ptr<basic_node<Data, Cost, Container, constContainer >> (ptr),
            cost
        );
        std::pair<EdgesIterator, bool> result{std::make_pair(--_out_edges.end(), true)};
        return result;
    }

    /**
     * @brief Delete an edge to another node
     * @param other Const iterator to target node
     * @return true if edge was deleted, false otherwise
     */
    bool del_edge(constContainer other);

    /**
     * @brief Delete an edge based on a predicate
     * @param other Const iterator to target node
     * @param predicate Function to determine if edge should be deleted
     * @return true if edge was deleted, false otherwise
     */
    bool del_edge_if(constContainer other, std::function<bool(const edge &)> predicate);

    /**
     * @brief Clear all edges from this node
     * @return Number of edges cleared
     */
    std::size_t clear_edges();

    /// @}

    ///
    /// @name Operations
    /// @{

    /**
     * @brief Get the degree of this node
     * @return Pair containing in-degree and out-degree
     */
    [[nodiscard]] std::pair<std::size_t, std::size_t> degree() const;

    /**
     * @brief Check if an adjacent node exists
     * @param other Iterator to potential adjacent node
     * @return true if adjacent node exists, false otherwise
     */
    [[nodiscard]] bool existing_adjacent_node(constContainer other) const;

    /// @}
};

#include "Node.cpp"

#endif /// ROOT_NODE_H
