///
/// Created by Terae on 09/03/17.
///

#ifndef ROOT_GRAPH_H
#define ROOT_GRAPH_H

#include <algorithm> /// find
#include <fstream>   /// setw, operator<<
#include <iomanip>   /// setw
#include <map>       /// map
#include <vector>    /// vector

#if (defined(__cplusplus) && __cplusplus >= 201703L) || (defined(_MSC_VER) && _MSC_VER > 1900 && defined(_HAS_CXX17) && _HAS_CXX17 == 1)
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
    #if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) < 50000
        #error "unsupported GCC version - see https://github.com/terae/structure#supported-compilers"
    #endif
#endif

#include "Node.h"

enum Nature {
    DIRECTED   = 'd',
    UNDIRECTED = 'u'
};

///////////////////////////
///// ADJACENCY GRAPH /////
///////////////////////////

/*!
 * @brief a generalized class of Graph
 *
 * @tparam Key type of the keys. Each element in a Graph is uniquely identified by its key value. Aliased as member type Graph::key_type
 * @tparam T type of graphed value stored into a node. Aliased as member type Graph::graphed_type
 * @tparam Cost type of the cost between nodes.
 */
template <class Key, class T, class Cost = std::size_t>
class graph {
public:
    class node;
    class Degree;
    
private:
    using PtrNode  = std::shared_ptr<node>;
    using MapNodes = std::map<Key, PtrNode>;
    
    MapNodes _nodes;
    Nature   _nature;
    std::size_t _num_edges = 0;
    
    const Cost infinity = std::numeric_limits<Cost>::has_infinity ? std::numeric_limits<Cost>::infinity() :
                          std::numeric_limits<Cost>::max();
    
    std::ostream &print(std::ostream &os) const;

public:
    //!
    //! @section exceptions
    //!
    
    using bad_graph          = detail::bad_graph;
    using exception          = detail::exception;
    using invalid_argument   = detail::invalid_argument;
    using negative_edge      = detail::negative_edge;
    using not_complete       = detail::not_complete;
    using parse_error        = detail::parse_error;
    using unexpected_nullptr = detail::unexpected_nullptr;
    
    //!
    //! @section container types
    //!
    
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
    using iterator               = typename std::map<Key, PtrNode>::iterator;
    /// a const iterator for a graph container
    using const_iterator         = typename MapNodes::const_iterator;
    /// a reverse iterator for a graph container
    using reverse_iterator       = typename MapNodes::reverse_iterator;
    /// a const reverse iterator for a graph container
    using const_reverse_iterator = typename MapNodes::const_reverse_iterator;
    
    /// @}
    
    //!
    //! @section Iterators
    //!
    
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
    
    //!
    //! @section Constructors
    //!
    
    /// default constructor
    explicit graph(Nature nature = UNDIRECTED);
    
    /// copy constructor
    graph(const graph &);
    
    /// move constructor
    graph(graph &&) noexcept;
    
    /// copy assignment operator
    graph &operator=(const graph &);
    
    /// move assignment operator
    graph &operator=(graph &&);
    
    virtual ~graph();
    
    //!
    //! @section Capacity
    //!

    bool empty() const noexcept;
    
    size_type size() const noexcept;
    
    size_type max_size() const noexcept;
    
    //!
    //! @section Element access
    //!

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
    const Cost operator()(const_iterator it1, const_iterator it2)  const;
    const Cost operator()(const key_type &k1, const key_type &k2)  const;
#endif
    //!
    //! @section Modifiers
    //!
    
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
    
    void link_all_nodes(Cost cost);
    
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
    
    //!
    //! @section Functions
    //!
    
    /// Non-modifying sequence operations
    size_type count(const key_type &) const;
    
    iterator       find(const key_type &);
    const_iterator find(const key_type &) const;
    
    //!
    //! @section Operations
    //!
    
    bool existing_node(const_iterator)   const;
    bool existing_node(const key_type &) const;
    
    bool existing_edge(const_iterator,   const_iterator)   const;
    bool existing_edge(const key_type &, const key_type &) const;
    
    inline size_type get_nbr_nodes() const noexcept;
    inline size_type get_nbr_edges() const noexcept;
    
    inline Nature get_nature() const;
    
    void set_nature(Nature new_nature);
    
    Degree degree(const_iterator)   const;
    Degree degree(const key_type &) const;
    
    std::pair<const_iterator, Degree> degree_max() const;
    std::pair<const_iterator, Degree> degree_min() const;
    
    std::map<key_type, Degree> degrees() const;
    
    //!
    //! @section Text functions
    //!
    
    template<class K, class D, class C> friend std::ostream &operator<<(std::ostream &os, const graph<K, D, C> &g);
    template<class K, class D, class C> friend std::istream &operator>>(std::istream &is, graph<K, D, C> &g);
    
    void save  (const char *filepath) const;
    graph &load(const char *filepath);
    
    //!
    //! @section Bool operators
    //!
    
    template<class K, class D, class C> bool operator==(const graph<K, D, C> &other) const noexcept;
    template<class K, class D, class C> bool operator!=(const graph<K, D, C> &other) const noexcept;

    /// CRTP: https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
    class node : public basic_node<T, Cost, iterator, const_iterator> {
    public:
        explicit node();
        
        explicit node(const T &);
        
        node &operator=(const T &);
    
    private:
        friend class graph;
        
        void set_iterator_values(iterator this_, iterator end, const_iterator cend);
    };
    
    class Degree {
    public:
        using degree_directed   = std::pair<size_type, size_type>;
        using degree_undirected = size_type;
        
        Degree(degree_undirected degree);
        
        Degree(const degree_directed& degree);
        Degree(size_type in,   size_type out);
    
        degree_directed   get_degree_directed()   const;
        degree_undirected get_degree_undirected() const;
    
        bool operator==(const Degree&)          const;
        bool operator==(const degree_directed&) const;
        bool operator==(degree_undirected)      const;
        
        bool operator<(const Degree&)          const;
        
        static Degree max(Nature);
        static Degree min(Nature);
        
    private:
        Nature n;
        degree_directed   _directed_deg;
        degree_undirected _undirected_deg;
    };
};

#include "Graph.cpp"

#endif /// ROOT_GRAPH_H
