///
/// Created by Terae on 06/12/17.
///

#include <set>

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::begin() noexcept {
    return _nodes.begin();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::end() noexcept {
    return _nodes.end();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_iterator graph<Key, T, Cost, Nat>::begin() const noexcept {
    return _nodes.begin();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_iterator graph<Key, T, Cost, Nat>::cbegin() const noexcept {
    return _nodes.cbegin();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_iterator graph<Key, T, Cost, Nat>::end() const noexcept {
    return _nodes.end();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_iterator graph<Key, T, Cost, Nat>::cend() const noexcept {
    return _nodes.cend();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::reverse_iterator graph<Key, T, Cost, Nat>::rbegin() noexcept {
    return _nodes.rbegin();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::reverse_iterator graph<Key, T, Cost, Nat>::rend() noexcept {
    return _nodes.rend();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_reverse_iterator graph<Key, T, Cost, Nat>::rbegin() const noexcept {
    return _nodes.rbegin();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_reverse_iterator graph<Key, T, Cost, Nat>::crbegin() const noexcept {
    return _nodes.crbegin();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_reverse_iterator graph<Key, T, Cost, Nat>::rend() const noexcept {
    return _nodes.rend();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_reverse_iterator graph<Key, T, Cost, Nat>::crend() const noexcept {
    return _nodes.crend();
}

//////////////////////////////////////////////////
///// IMPLEMENTATION OF THE ADJACENCY GRAPHS /////
//////////////////////////////////////////////////

/// Constructors
template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::graph() {
    _nodes.clear();
}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::graph(std::istream &is) {
    is >> *this;
}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::graph(const graph &g) {
    *this = g;
}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::graph(graph &&g) noexcept {
    swap(g);
}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat> &graph<Key, T, Cost, Nat>::operator=(const graph &g) {
    clear();

    for (const_iterator it{g.cbegin()}; it != g.cend(); ++it) {
        add_node(it->first, it->second->get());
    }

    for (const_iterator it{g.cbegin()}; it != g.cend(); ++it) {
        std::list<typename node::edge> list = it->second->get_edges();
        for (typename node::edge e : list) {
            graph::const_iterator i{e.target()};
            add_edge(it->first, i->first, e.cost());
        }
    }

    return *this;
}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat> &graph<Key, T, Cost, Nat>::operator=(graph &&g) {
    //! If there is a self-reference: bug in the client part that should be fixed
    if (this == &g) {
        GRAPH_THROW_WITH(invalid_argument, "Self-reference in the client part")
    }
    swap(g);
    return *this;
}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::~graph() {}

/// Capacity

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::empty() const noexcept {
    return _nodes.empty();
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::size() const noexcept {
    return _nodes.size();
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::max_size() const noexcept {
    return _nodes.max_size();
}

/// Element access
template <class Key, class T, class Cost, Nature Nat>
T &graph<Key, T, Cost, Nat>::operator[](const key_type &k) {
    return add_node(k).first->second->get();
}

template <class Key, class T, class Cost, Nature Nat>
T &graph<Key, T, Cost, Nat>::operator[](key_type &&k) {
    return add_node(k).first->second->get();
}

#if defined(GRAPH_HAS_CPP_17)
template <class Key, class T, class Cost, Nature Nat>
const std::optional<T> graph<Key, T, Cost, Nat>::operator[](key_type &&k) const {
    const_iterator it{find(k)};
    return it == cend() ? std::nullopt : std::optional<T>(it->second->get());
}
#else
template <class Key, class T, class Cost, Nature Nat>
const T graph<Key, T, Cost, Nat>::operator[](key_type &&k) const {
    const_iterator it{find(k)};
    if (it == cend()) {
        GRAPH_THROW_WITH(invalid_argument, "Unexistant node")
    }

    return it->second->get();
}
#endif

template <class Key, class T, class Cost, Nature Nat>
Cost &graph<Key, T, Cost, Nat>::operator()(iterator it1, iterator it2) {
    if (!existing_edge(it1, it2)) {
        add_edge(it1, it2, infinity);
    }

    return it1->second->get_cost(it2);
}

template <class Key, class T, class Cost, Nature Nat>
Cost &graph<Key, T, Cost, Nat>::operator()(const key_type &k1, const key_type &k2) {
    iterator it1{add_node(k1).first};
    iterator it2{add_node(k2).first};

    return operator()(it1, it2);
}

#if defined(GRAPH_HAS_CPP_17)
template <class Key, class T, class Cost, Nature Nat>
const std::optional<Cost> graph<Key, T, Cost, Nat>::operator()(const_iterator it1, const_iterator it2) const {
    return existing_edge(it1, it2) ? std::optional<Cost>(it1->second->get_cost(it2)) : std::nullopt;
}

template <class Key, class T, class Cost, Nature Nat>
const std::optional<Cost> graph<Key, T, Cost, Nat>::operator()(const key_type &k1, const key_type &k2) const {
    return operator()(find(k1), find(k2));
}
#else
template <class Key, class T, class Cost, Nature Nat>
const Cost graph<Key, T, Cost, Nat>::operator()(const_iterator it1, const_iterator it2) const {
    if (!existing_edge(it1, it2)) {
        GRAPH_THROW_WITH(invalid_argument, "Unexistant edge")
    }

    return it1->second->get_cost(it2);
}

template <class Key, class T, class Cost, Nature Nat>
const Cost graph<Key, T, Cost, Nat>::operator()(const key_type &k1, const key_type &k2) const {
    return operator()(find(k1), find(k2));
}
#endif

template <class Key, class T, class Cost, Nature Nat>
std::pair<typename graph<Key, T, Cost, Nat>::iterator, bool> graph<Key, T, Cost, Nat>::insert(const graph<Key, T, Cost, Nat>::value_type &val) {
    std::pair<iterator, bool> p{_nodes.insert(val)};
    p.first->second->set_iterator_values(p.first, end(), cend());
    return p;
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::insert(const_iterator position, const value_type &val) {
    iterator it{_nodes.insert(position, val)};
    it->second->set_iterator_values(it, end(), cend());
    return it;
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::insert(const_iterator position, const key_type &k, graphed_type &x) {
    return insert(position, std::make_pair(k, std::make_shared<node>(x)));
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::insert(const_iterator position, const key_type &k, const node &n) {
    return insert(position, std::make_pair(k, std::make_shared<node>(n)));
}

template <class Key, class T, class Cost, Nature Nat>
std::pair<typename graph<Key, T, Cost, Nat>::iterator, bool> graph<Key, T, Cost, Nat>::emplace(const key_type &k) {
    return emplace(k, node());
}

template <class Key, class T, class Cost, Nature Nat>
std::pair<typename graph<Key, T, Cost, Nat>::iterator, bool> graph<Key, T, Cost, Nat>::emplace(const key_type &k, const graphed_type &x) {
    return emplace(k, node(x));
}

template <class Key, class T, class Cost, Nature Nat>
std::pair<typename graph<Key, T, Cost, Nat>::iterator, bool> graph<Key, T, Cost, Nat>::emplace(const key_type &k, const node &n) {
    std::pair<iterator, bool> p{_nodes.emplace(k, std::make_shared<node>(n))};
    p.first->second->set_iterator_values(p.first, end(), cend());
    return p;
}

template <class Key, class T, class Cost, Nature Nat>
std::pair<typename graph<Key, T, Cost, Nat>::iterator, bool> graph<Key, T, Cost, Nat>::add_node(const key_type &k) {
    return emplace(k);
}

template <class Key, class T, class Cost, Nature Nat>
std::pair<typename graph<Key, T, Cost, Nat>::iterator, bool> graph<Key, T, Cost, Nat>::add_node(const key_type &k, const graphed_type &x) {
    return emplace(k, x);
}

template <class Key, class T, class Cost, Nature Nat>
std::pair<typename graph<Key, T, Cost, Nat>::iterator, bool> graph<Key, T, Cost, Nat>::add_node(const key_type &k, const node &n) {
    return emplace(k, n);
}

/// Modifiers

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::add_edge(const_iterator it1, const_iterator it2, Cost cost) {
    std::pair<typename std::list<typename node::edge>::const_iterator, bool> new_edge{it1->second->add_edge(it2, cost)};

    if (get_nature() == UNDIRECTED) {
        it2->second->set_edge(it1, new_edge.first->_cost);
    }

    if (new_edge.second) {
        _num_edges++;
    }

    return true;
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::add_edge(const key_type &k1, const key_type &k2, Cost cost) {
    return add_edge(emplace(k1).first, emplace(k2).first, cost);
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::link_all_nodes(Cost cost) {
    clear_edges();

    if (cost != infinity) {
        for (iterator it1{begin()}; it1 != end(); ++it1) {
            for (iterator it2{begin()}; it2 != end(); ++it2) {
                if (it1 != it2) {
                    add_edge(it1, it2, cost);
                }
            }
        }
    }
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::erase(const_iterator position) {
    clear_edges(position);

    return _nodes.erase(position);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::erase(const_iterator first, const_iterator last) {
    for (const_iterator it{first}; it != last && it != cend(); ++it) {
        clear_edges(it);
    }

    return _nodes.erase(first, last);
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::erase(const key_type &k) {
    const_iterator it{find(k)};
    if (it == cend()) {
        return 0;
    }

    clear_edges(it);
    return _nodes.erase(k);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::del_node(const_iterator position) {
    return erase(position);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::del_nodes(const_iterator first, const_iterator last) {
    return erase(first, last);
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::del_node(const key_type &k) {
    return erase(k);
}
template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::clear() noexcept {
    *this = std::move(graph());
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::del_edge(const_iterator it1, const_iterator it2) {
    size_type result{0};
    if (it1 != end() && it2 != end()) {
        result = static_cast<size_type>(it1->second->del_edge(it2));

        if (get_nature() == UNDIRECTED) {
            it2->second->del_edge(it1);
        }

        _num_edges -= result;
    }
    return result;
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::del_edge(const key_type &k1, const key_type &k2) {
    return del_edge(find(k1), find(k2));
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::clear_edges() {
    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        clear_edges(it);
    }
    _num_edges = 0;
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::clear_edges(const_iterator it) {
    size_type result{0};
    if (it != cend()) {
        for (const_iterator it1{cbegin()}; it1 != cend(); ++it1) {
            del_edge(it1, it);
        }

        if (get_nature() == DIRECTED) {
            result += it->second->clear_edges();
        }

        _num_edges -= result;
    }
    return result;
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::clear_edges(const key_type &k) {
    return clear_edges(find(k));
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::swap(graph &other) {
    std::swap(_nodes,     other._nodes);
    std::swap(_num_edges, other._num_edges);
}

/// specialisation of std::swap
namespace std {
    template <class Key, class T, class Cost, Nature Nat>
    inline void swap(graph<Key, T, Cost, Nat> &g1, graph<Key, T, Cost, Nat> &g2) noexcept {
        g1.swap(g2);
    }
}

/// Operations

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::count(const key_type &k) const {
    return _nodes.count(k);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::iterator graph<Key, T, Cost, Nat>::find(const key_type &k) {
    return _nodes.find(k);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_iterator graph<Key, T, Cost, Nat>::find(const key_type &k) const {
    return _nodes.find(k);
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::existing_node(const_iterator it) const {
    return it != cend();
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::existing_node(const key_type &k) const {
    return existing_node(find(k));
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::existing_edge(const_iterator it1, const_iterator it2) const {
    if (it1 != cend() && it2 != cend()) {
        if (get_nature() == DIRECTED) {
            return it1->second->existing_adjacent_node(it2);
        } else {
            return it1->second->existing_adjacent_node(it2) && it2->second->existing_adjacent_node(it1);
        }
    }

    return false;
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::existing_edge(const key_type &k1, const key_type &k2) const {
    return existing_edge(find(k1), find(k2));
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::get_nbr_nodes() const noexcept {
    return _nodes.size();
}

template <class Key, class T, class Cost, Nature Nat>
std::size_t graph<Key, T, Cost, Nat>::get_nbr_edges() const noexcept {
    return _num_edges;
}

template <class Key, class T, class Cost, Nature Nat>
inline Nature graph<Key, T, Cost, Nat>::get_nature() const {
    return Nat;
}

///template <class Key, class T, class Cost>
///void graph<Key, T, Cost>::set_nature(Nature new_nature) {
///    if (get_nature() != new_nature) {
///        typedef std::list<typename basic_node<T, Cost, iterator, const_iterator>::edge> Set;
///        if (new_nature == DIRECTED) {
///            for (const_iterator it{cbegin()}; it != cend(); ++it) {
///                Set child{it->second->get_edges()};
///                typename Set::const_iterator edge{child.cbegin()};
///                for (; edge != child.cend(); ++edge)
///                    if (edge->cost.use_count() != 2) { /// not unique, counting one's on the edge
///                        bool existing_edge{edge->get_container()->second->del_edge(it)};
///                        edge->get_container()->second->add_edge(it, *edge->cost);
///                        if(existing_edge)
///                            _num_edges++;
///                    }
///            }
///        } else { /// new_nature == UNDIRECTED
///            for (const_iterator it{cbegin()}; it != cend(); ++it) {
///                Set child{it->second->get_edges()};
///                typename Set::const_iterator edge{child.cbegin()};
///                for (; edge != child.cend(); ++edge)
///                    edge->get_container()->second->set_edge(it, edge->cost);
///            }
///        }
///        _nature = new_nature;
///    }
///}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::Degree graph<Key, T, Cost, Nat>::degree(const_iterator position) const {
    if (get_nature() == DIRECTED) {
        return position == cend() ? Degree::min() : Degree(position->second->degree().first, position->second->degree().second);
    }
    return position == cend() ? Degree::min() : Degree(position->second->degree().first, 0);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::Degree graph<Key, T, Cost, Nat>::degree(const key_type &k) const {
    return degree(find(k));
}

template <class Key, class T, class Cost, Nature Nat>
std::pair<typename graph<Key, T, Cost, Nat>::const_iterator, typename graph<Key, T, Cost, Nat>::Degree> graph<Key, T, Cost, Nat>::degree_max() const {
    if (empty()) {
        return std::make_pair(cend(), Degree::min());
    }

    Degree max{Degree::min()};
    const_iterator cit;

    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        Degree tmp{degree(it)};
        if (max < tmp) {
            max = tmp;
            cit = it;
        }
    }

    return std::make_pair(cit, max);
}

template <class Key, class T, class Cost, Nature Nat>
std::pair<typename graph<Key, T, Cost, Nat>::const_iterator, typename graph<Key, T, Cost, Nat>::Degree> graph<Key, T, Cost, Nat>::degree_min() const {
    if (empty()) {
        return std::make_pair(cend(), Degree::min());
    }

    Degree min{Degree::max()};
    const_iterator cit;

    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        Degree tmp{degree(it)};
        if (tmp < min) {
            min = tmp;
            cit = it;
        }
    }

    return std::make_pair(cit, min);
}

template <class Key, class T, class Cost, Nature Nat>
std::map<Key, typename graph<Key, T, Cost, Nat>::Degree> graph<Key, T, Cost, Nat>::degrees() const {
    std::map<key_type, Degree> result;

    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        result.insert(std::make_pair(it->first, degree(it)));
    }

    return result;
}

template <class Key, class T, class Cost, Nature Nat>
template <class> /// enable_if_t<DIRECTED>
std::vector<typename graph<Key, T, Cost, Nat>::node::edge> graph<Key, T, Cost, Nat>::get_in_edges(const_iterator to) const {
    std::vector<typename node::edge> result;

    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        if (it != to) {
            for (typename node::edge e : it->second->_out_edges) {
                if (e.target() == to) {
                    result.push_back(e);
                }
            }
        }
    }

    return result;
}

template <class Key, class T, class Cost, Nature Nat>
template <class> /// enable_if_t<DIRECTED>
std::vector<typename graph<Key, T, Cost, Nat>::node::edge> graph<Key, T, Cost, Nat>::get_in_edges(const key_type &to) const {
    return get_in_edges(find(to));
}

template <class Key, class T, class Cost, Nature Nat>
template <class> /// enable_if_t<DIRECTED>
std::vector<typename graph<Key, T, Cost, Nat>::node::edge> graph<Key, T, Cost, Nat>::get_out_edges(const_iterator from) const {
    return std::vector<typename node::edge>(from->second->_out_edges.begin(), from->second->_out_edges.end());
}

template <class Key, class T, class Cost, Nature Nat>
template <class> /// enable_if_t<DIRECTED>
std::vector<typename graph<Key, T, Cost, Nat>::node::edge> graph<Key, T, Cost, Nat>::get_out_edges(const key_type &from) const {
    return get_out_edges(find(from));
}

template <class Key, class T, class Cost, Nature Nat>
template <class> /// enable_if_t<UNDIRECTED>
std::vector<typename graph<Key, T, Cost, Nat>::node::edge> graph<Key, T, Cost, Nat>::get_edges(const_iterator i) const {
    return std::vector<typename node::edge>(i->second->_out_edges.begin(), i->second->_out_edges.end());
}

template <class Key, class T, class Cost, Nature Nat>
template <class> /// enable_if_t<UNDIRECTED>
std::vector<typename graph<Key, T, Cost, Nat>::node::edge> graph<Key, T, Cost, Nat>::get_edges(const key_type &i) const {
    return get_edges(find(i));
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::is_cyclic() const {

}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::is_isomorphic() const {

}

template <class Key, class T, class Cost, Nature Nat>
std::ostream &graph<Key, T, Cost, Nat>::print(std::ostream &os) const {
    using std::setw;
    using std::ostringstream;

    const std::string tab{"    "};
    const std::string separator{","};

    //! Displaying nature + graph types
    if (get_nature() == DIRECTED) {
        os << "di";
    }

    os << "graph<" << detail::type_name<Key>()  << ", "
       << detail::type_name<T>()    << ", "
       << detail::type_name<Cost>() << "> {\n"
       << tab << "nodes: {\n";

    //! Displaying nodes:  "<name>"; "<value>",
    size_type max_size{3};
    for_each(cbegin(), cend(), [&max_size](const value_type & element) {
        ostringstream out;
        out << element.first;
        size_type size{static_cast<size_type>(out.tellp())};

        if (size > max_size) {
            max_size = size;
        }
    });

    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        ostringstream out;
        out << it->first << '"' << separator;

        os << tab << tab
           << '"' << std::left << setw(static_cast<int>(max_size + 2 + separator.size())) << out.str()
           << '"' << it->second->get() << '"';

        if (it != --cend()) {
            os << ';';
        }
        os << '\n';
    }
    os << tab << '}';

    //! Displaying edges:  "<node 1>"; "<node 2>"; "<cost>",
    if (get_nbr_edges() == 0) {
        os << "\n}";
    } else {
        os << ",\n" << tab << "edges: {\n";
        size_type max_size_1{0}, max_size_2{0};

        for_each(cbegin(), cend(), [&max_size_1, &max_size_2, this](const value_type & element) {
            std::list<typename node::edge> child{element.second->get_edges()};
            for_each(child.cbegin(), child.cend(), [ &, this](const typename node::edge & i) {
                ostringstream out_1, out_2;
                out_1 << element.first;
                out_2 << i.target()->first;

                size_type size_1{static_cast<size_type>(out_1.tellp())},
                          size_2{static_cast<size_type>(out_2.tellp())};

                if (size_1 > max_size_1) {
                    max_size_1 = size_1;
                }

                if (size_2 > max_size_2) {
                    max_size_2 = size_2;
                }
            });
        });

        size_type p{0};
        for_each(cbegin(), cend(), [ =, &os, &p](const value_type & element) {
            std::list<typename node::edge> child{element.second->get_edges()};
            for_each(child.cbegin(), child.cend(), [ =, &os, &p](const typename node::edge & i) {
                ostringstream out_1, out_2;
                out_1 << element.first << '"' << separator;
                out_2 << i.target()->first << '"' << separator;

                os << tab << tab
                   << '"' << setw(static_cast<int>(max_size_1 + 2 + separator.size())) << out_1.str()
                   << '"' << setw(static_cast<int>(max_size_2 + 2 + separator.size())) << out_2.str();
                if (i.cost() == infinity) {
                    os << "infinity";
                } else {
                    os << '"' << i.cost() << '"';
                }

                if (p < (this->get_nature() == DIRECTED ?
                         this->get_nbr_edges() : 2 * this->get_nbr_edges()) - 1) {
                    os << ';';
                }
                os << '\n';
                p++;
            });
        });
        os << tab << "}\n}";
    }

    return os;
}

template <class Key, class T, class Cost, Nature Nat>
std::ostream &operator<<(std::ostream &os, const graph<Key, T, Cost, Nat> &g) {
    return g.print(os) << std::endl;
}

template <class Key, class T, class Cost, Nature Nat>
std::istream &operator>>(std::istream &is, graph<Key, T, Cost, Nat> &g) {
    g.clear();

    //! Nature
    std::string line;
    getline(is, line);
    if (line.substr(0, 5) == "graph") {
        if (g.get_nature() != UNDIRECTED) {
            GRAPH_THROW_WITH(invalid_argument, "Bad graph nature (expected 'graph')")
        }
    } else if (line.substr(0, 7) == "digraph") {
        if (g.get_nature() != DIRECTED) {
            GRAPH_THROW_WITH(invalid_argument, "Bad graph nature (expected 'digraph')")
        }
    } else {
        GRAPH_THROW_WITH(parse_error, static_cast<std::size_t>(is.tellg()), "Bad graph nature (expected '[di]graph')")
    }

    //! Nodes
    getline(is, line);
    if (line != "    nodes: {") {
        GRAPH_THROW_WITH(parse_error, static_cast<std::size_t>(is.tellg()), "Bad format for nodes")
    }
    while (getline(is, line) && line.find("}") == std::string::npos) {
        std::istringstream iss{line};
        Key key;
        T value;
        detail::read_T(iss, key);
        detail::read_T(iss, value);

        g[key] = value;
    }

    //! Edges
    getline(is, line);
    if (line == "}") {}
    else if (line != "    edges: {") {
        GRAPH_THROW_WITH(parse_error, static_cast<std::size_t>(is.tellg()), "Bad format for edges")
    } else {
        while (getline(is, line) && line.find("}") == std::string::npos) {
            std::istringstream iss{line};
            Key from;
            Key to;
            Cost cost;
            detail::read_T(iss, from);
            detail::read_T(iss, to);
            detail::read_cost(iss, cost);

            g(from, to) = cost;
        }

        getline(is, line);
        if (line != "}") {
            GRAPH_THROW_WITH(parse_error, static_cast<std::size_t>(is.tellg()), "Bad format at the end of the graph")
        }
    }

    return is;
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::save(const char* filepath) const {
    std::ofstream out(filepath);
    out << *this << std::endl;
    out.close();
}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat> &graph<Key, T, Cost, Nat>::load(const char* filepath) {
    std::ifstream in(filepath);
    if (in) {
        in >> *this;
        in.close();
    }

    return *this;
}

template <class Key, class T, class Cost, Nature Nat>
std::ostream &graph<Key, T, Cost, Nat>::generate_dot(std::ostream &os, const std::string &name) const {
    const std::string tab{"    "};

    //! Displaying nature + graph name
    if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-") != name.npos) {
        GRAPH_THROW_WITH(invalid_argument, "Wrong graph name given; accepted characters: [a-zA-Z0-9_-]")
    }

    if (get_nature() == DIRECTED) {
        os << "di";
    }
    os << "graph ";
    if (!name.empty()) {
        os << name << ' ';
    }
    os << "{\n";

    //! Displaying nodes' name
    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        os << tab << it->first << '\n';
    }

    if (get_nature() == DIRECTED) {
        for_each(cbegin(), cend(), [ =, &os](const value_type & element) {
            std::list<typename node::edge> child{element.second->get_edges()};
            for_each(child.cbegin(), child.cend(), [ =, &os](const typename node::edge & i) {
                if (i.cost() != infinity) {
                    os << tab << element.first << " -> " << i.target()->first << '\n';
                }
            });
        });
    } else { /// get_nature() == UNDIRECTED
        std::set<std::pair<Key, Key>> list_edges;
        for_each(cbegin(), cend(), [ =, &os, &list_edges](const value_type & element) {
            std::list<typename node::edge> child{element.second->get_edges()};
            for_each(child.cbegin(), child.cend(), [ =, &os, &list_edges](const typename node::edge & i) {
                if (i.cost() != infinity) {
                    const Key min{std::min(element.first, i.target()->first)};
                    const Key max{std::max(element.first, i.target()->first)};
                    list_edges.emplace(std::make_pair(min, max));
                }
            });
        });

        for_each(list_edges.cbegin(), list_edges.cend(), [ =, &os](const std::pair<Key, Key> &p) {
            os << tab << p.first << " -- " << p.second << '\n';
        });
    }
    os << '}';

    return os;
}

template <class Key, class T, class Cost, Nature Nat>
template <class K,   class D, class C,    Nature N>
bool graph<Key, T, Cost, Nat>::operator==(const graph<K, D, C, N> &other) const noexcept {
    typedef typename graph<Key, T, Cost, Nat>::const_iterator Iterator1;
    typedef typename graph<K,   D,    C,   N>::const_iterator Iterator2;
    typedef std::list<typename basic_node<T, Cost, typename graph<Key, T, Cost, Nat>::iterator, Iterator1>::edge> Set1;
    typedef std::list<typename basic_node<D,    C, typename graph<K,   D, C,      N>::iterator, Iterator2>::edge> Set2;

    if (get_nature()        != other.get_nature()    ||
            get_nbr_nodes() != other.get_nbr_nodes() ||
            get_nbr_edges() != other.get_nbr_edges()) {
        return false;
    }

    Iterator1 it1{cbegin()};
    Iterator2 it2{other.cbegin()};

    for (; (it1 != cend() || it2 != other.cend()); ++it1, ++it2) {
        /// Test nodes value
        if (!(it1->first                == it2->first)         ||
                !(it1->second->get()    == it2->second->get()) ||
                !(it1->second->degree() == it2->second->degree())) {
            return false;
        }

        /// Test out edges
        Set1 child1{it1->second->get_edges()};
        Set2 child2{it2->second->get_edges()};
        typename Set1::const_iterator edge1{child1.cbegin()};
        typename Set2::const_iterator edge2{child2.cbegin()};

        for (; (edge1 != child1.cend() || edge2 != child2.cend()); ++edge1, ++edge2) {
            Iterator1 target1{edge1->target()};
            Iterator2 target2{edge2->target()};

            if (!(target1->first == target2->first && edge1->cost() == edge2->cost())) {
                return false;
            }
        }
    }

    return true;
}

template <class Key, class T, class Cost, Nature Nat>
template <class K,   class D, class C,    Nature N>
bool graph<Key, T, Cost, Nat>::operator!=(const graph<K, D, C, N> &other) const noexcept {
    return !(*this == other);
}

///////////////////////////////////
///// IMPLEMENTATION OF NODES /////
///////////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::node::node() : node(graphed_type()) {}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::node::node(const graphed_type &d) {
    this->_data = d;
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::node &graph<Key, T, Cost, Nat>::node::operator=(const graphed_type &d) {
    this->set(d);
    return *this;
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::node::set_iterator_values(iterator this_, iterator end, const_iterator cend) {
    this->container_from_this = this_;
    this->end_container       = end;
    this->cend_container      = cend;
}
