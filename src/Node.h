///
/// Created by Terae on 04/04/17.
///

#ifndef ROOT_NODE_H
#define ROOT_NODE_H

#include <functional> /// function
#include <iostream>   /// cerr, cout
#include <list>       /// list
#include <memory>     /// make_shared, shared_ptr, weak_ptr
#include <tuple>      /// tie, tuple
#include <utility>    /// make_pair, move, pair, swap

#include "detail.hpp"

template <class Data, class Cost, class Container, class constContainer>
class basic_node {
  private:
    template <class K, class T, class C, Nature N>
    friend class graph;

    struct edge;
    typedef std::list<edge> ListEdges;
    typedef typename ListEdges::iterator EdgesIterator;

    ListEdges _out_edges;

    const Cost infinity = std::numeric_limits<Cost>::has_infinity ? std::numeric_limits<Cost>::infinity() :
                          std::numeric_limits<Cost>::max();

    std::size_t _in_degree{0};

    struct edge {
        std::weak_ptr<basic_node<Data, Cost, Container, constContainer>> target;
        std::shared_ptr<Cost> cost;

        edge(const std::weak_ptr<basic_node<Data, Cost, Container, constContainer>> &ptr, Cost c);

        std::tuple<Cost, basic_node<Data, Cost, Container, constContainer>> tie() const;

        bool operator< (const edge &other) const;

        bool operator==(const edge &other) const;

        constContainer get_container() const;
    };

    inline void increment_in_degree(int n = 1);
    inline void decrement_in_degree(int n = 1);

    //! used for UNDIRECTED graphs: same Cost in memory for both directions
    bool set_edge(constContainer other, std::shared_ptr<Cost> cost);

    std::tuple<Data, std::size_t, ListEdges> tie() const;

    inline ListEdges get_edges() const;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator==(const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator!=(const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator<(const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator<=(const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

    template <class T, class C, class X, class Y>
    friend constexpr bool operator>(const basic_node<T, C, X, Y> &n1, const basic_node<T, C, X, Y> &n2) noexcept;

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
    //!
    //! @section exceptions
    //!
    using exception          = detail::exception;
    using invalid_argument   = detail::invalid_argument;
    using unexpected_nullptr = detail::unexpected_nullptr;

    //!
    //! @section Constructors
    //!
    explicit basic_node();

    explicit basic_node(const Data &d);

    basic_node &operator=(basic_node &&n) = delete;

    ~basic_node();

    //!
    //! @section Element access
    //!

    inline Data &get();

    inline Data get() const;

    Cost &get_cost(const Container &other);
    const Cost  get_cost(constContainer   other) const;

    Cost &operator[](Container      other);
    const Cost  operator[](constContainer other) const;

    //!
    //! @section Modifiers
    //!
    template<class T_data>
    inline void set(const T_data &d);

    template<class T_cost>
    void set_cost(Container other, const T_cost &c);

    //! Adders
    /// @return true if it's a new edge
    std::pair<EdgesIterator, bool> add_edge(constContainer other, Cost cost = Cost(1)) {
        std::shared_ptr<basic_node<Data, Cost, Container, constContainer>> ptr{detail::get_value(other, cend_container)};

        if (ptr == nullptr)
            GRAPH_THROW(unexpected_nullptr)

            for (EdgesIterator it{_out_edges.begin()}; it != _out_edges.end(); ++it)
                if (it->target.lock() == ptr) {
                    *it->cost = cost;
                    return std::make_pair(it, false);
                }

        //! Link doesn't exist
        ptr->increment_in_degree();
        _out_edges.emplace_back(std::weak_ptr<basic_node<Data, Cost, Container, constContainer>>(ptr), cost);
        std::pair<EdgesIterator, bool> result{std::make_pair(--_out_edges.end(), true)};
        return result;
    }

    //! Deleters
    bool del_edge   (constContainer other);
    bool del_edge_if(constContainer other, std::function<bool(edge)> predicate);

    std::size_t clear_edges();

    //!
    //! @section Operations
    //!

    /// @return pair<in degree, out degree>
    inline std::pair<std::size_t, std::size_t> degree() const;

    bool existing_adjacent_node(constContainer other) const;
};

#include "Node.cpp"

#endif /// ROOT_NODE_H
