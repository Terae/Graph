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
template <class> /// enable_if_t<DIRECTED>
bool graph<Key, T, Cost, Nat>::is_cyclic() const {
    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        std::list<const_iterator> initPath;
        if (is_cyclic_rec(it, initPath)) {
            return true;
        }
    }
    return false;
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::is_cyclic_rec(const_iterator current, std::list<const_iterator> path) const {
    for (const_iterator it : path) {
        if (it == current && (get_nature() == DIRECTED || path.back() != current)) {
            return true;
        }
    }
    /*if (std::find(path.cbegin(), path.cend(), current) != path.cend()) {
        return true;
    }*/

    path.push_back(current);
    std::vector<typename node::edge> adjacent;
    if (get_nature() == DIRECTED) {
        adjacent = get_out_edges(current);
    } else { /// get_nature() == UNDIRECTED
        adjacent = get_edges(current);
    }

    for (typename std::vector<typename node::edge>::const_iterator it{adjacent.cbegin()}; it != adjacent.cend(); ++it) {
        if (is_cyclic_rec(it->target(), path)) {
            return true;
        }
        path.remove(it->target());
    }

    return false;
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::is_isomorphic() const {

}

template <class Key, class T, class Cost, Nature Nat>
std::ostream &operator<<(std::ostream &os, const graph<Key, T, Cost, Nat> &g) {
    return os << *g.generate_grp() << std::endl;
    //return os << std::setw(4) << *g.generate_json() << std::endl;
}

template <class Key, class T, class Cost, Nature Nat>
std::istream &operator>>(std::istream &is, graph<Key, T, Cost, Nat> &g) {
    GRAPH_TRY {
        g.parse_from_grp(is);
    } GRAPH_CATCH (...) {
        ///GRAPH_TRY {
        ///    g.parse_from_json(is);
        ///} GRAPH_THROW (std::exception&e) {
        ///    std::cerr << e.what() << std::endl;
        GRAPH_THROW_WITH(parse_error, 0, "istream corrupted, fail to parse the graph")
        ///}
    }

    return is;
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::load(const char* filename) {
    const char* dot = strrchr(filename, '.');
    std::string extension{""};
    if (dot != NULL && dot != filename) {
        extension = dot + 1;
    }

    std::ifstream in(filename);
    if (!in) {
        GRAPH_THROW_WITH(invalid_argument, ("Unexistant file: '" + std::string(filename) + "'").c_str())
    }

    if (extension == "json") {
        parse_from_json(static_cast<std::istream &>(in));
    } else if (extension == "dot") {
        GRAPH_THROW_WITH(invalid_argument, "Load from DOT file non supported yet")
    } else {
        GRAPH_TRY {
            parse_from_grp(static_cast<std::istream &>(in));
        } GRAPH_CATCH (exception & e) {
            std::cerr << "Fail to load the file '" << filename << "'; wrong extension or corrupted file." << std::endl;
        }
    }
    in.close();
}

template <class Key, class T, class Cost, Nature Nat>
std::unique_ptr<std::string> graph<Key, T, Cost, Nat>::generate_dot(const std::string &graph_name) const {
    std::string dot;

    const std::string tab{"    "};

    //! Displaying nature + graph name
    if (graph_name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-") != graph_name.npos) {
        GRAPH_THROW_WITH(invalid_argument, "Wrong graph name given; accepted characters: [a-zA-Z0-9_-]")
    }

    if (get_nature() == DIRECTED) {
        dot += "di";
    }
    dot += "graph ";
    if (!graph_name.empty()) {
        dot += graph_name + " ";
    }
    dot += "{\n";

    //! Displaying nodes' name
    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        std::stringstream ss;
        ss << it->first;
        dot += tab + ss.str() + "\n";
    }

    if (get_nature() == DIRECTED) {
        for_each(cbegin(), cend(), [ =, &dot](const value_type & element) {
            std::list<typename node::edge> child{element.second->get_edges()};
            for_each(child.cbegin(), child.cend(), [ =, &dot](const typename node::edge & i) {
                if (i.cost() != infinity) {
                    std::stringstream ss;
                    ss << tab << element.first << " -> " << i.target()->first << '\n';
                    dot += ss.str();
                }
            });
        });
    } else { /// get_nature() == UNDIRECTED
        std::set<std::pair<Key, Key>> list_edges;
        for_each(cbegin(), cend(), [ =, &dot, &list_edges](const value_type & element) {
            std::list<typename node::edge> child{element.second->get_edges()};
            for_each(child.cbegin(), child.cend(), [ =, &dot, &list_edges](const typename node::edge & i) {
                const Key min{std::min(element.first, i.target()->first)};
                const Key max{std::max(element.first, i.target()->first)};
                list_edges.emplace(std::make_pair(min, max));
            });
        });

        for_each(list_edges.cbegin(), list_edges.cend(), [ =, &dot](const std::pair<Key, Key> &p) {
            std::stringstream ss;
            ss << tab << p.first << " -- " << p.second << '\n';
            dot += ss.str();
        });
    }
    dot += '}';

    return std::unique_ptr<std::string>(new std::string(std::forward<std::string>(dot)));
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::save_to_dot(const char* filename, const std::string &graph_name) const {
    std::ofstream out(filename);
    out << *generate_dot(graph_name) << std::endl;
    out.close();
}

template <class Key, class T, class Cost, Nature Nat>
std::unique_ptr<nlohmann::json> graph<Key, T, Cost, Nat>::generate_json() const {
    nlohmann::json json;

    using Set = std::list<typename node::edge>;

    /// Displaying nature
    json["nature"] = get_nature();

    /// Displaying nodes
    size_type n{0};
    for (const_iterator node{cbegin()}; node != cend(); ++node, ++n) {
        json["nodes"][n]["key"]   = node->first;
        json["nodes"][n]["value"] = node->second->get();
    }

    /// Displaying adjacences
    size_type p{0};
    for (const_iterator node{cbegin()}; node != cend(); ++node) {
        Set child{node->second->get_edges()};
        for (typename Set::const_iterator edge{child.begin()}; edge != child.end(); ++edge, ++p) {
            json["edges"][p]["from"] = node->first;
            json["edges"][p]["to"]   = edge->target()->first;
            json["edges"][p]["cost"] = edge->cost();
        }
    }
    return std::unique_ptr<nlohmann::json>(new nlohmann::json(std::forward<nlohmann::json>(json)));
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::save_to_json(const char* filename) const {
    std::ofstream out(filename);
    out << std::setw(4) << *generate_json() << std::endl;
    out.close();
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::parse_from_json(std::istream &is) {
    nlohmann::json json;
    is >> json;

    if (static_cast<Nature>(json["nature"]) != get_nature()) {
        std::string error_msg{"Bad graph nature (expected '"};
        error_msg += (Nat == DIRECTED ? "graph')" : "digraph')");
        GRAPH_THROW_WITH(invalid_argument, error_msg.c_str())
    }

    clear();
    for (const nlohmann::json &node : json["nodes"]) {
        key_type k = node["key"];
        graphed_type x = node["value"];
        add_node(k, x);
    }
    for (const nlohmann::json &edge : json["edges"]) {
        key_type from = edge["from"];
        key_type to = edge["to"];
        Cost c;
        if (edge["cost"].is_null()) {
            c = infinity;
        } else {
            c = edge["cost"];
        }
        add_edge(from, to, c);
    }
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::DEBUG_load_from_json_rust(const char* path) {
    nlohmann::json json;
    std::ifstream is(path);
    if (!is) {
        std::cerr << "Unexistant file '" << path << "'!" << std::endl;
        exit(1);
    }
    is >> json;

    clear();
    for (const nlohmann::json &node : json["nodes"]) {
        add_node(node);
    }

    for (const nlohmann::json &edge : json["edges"]) {
        const_iterator begin{std::next(cbegin(), edge[0])};
        const_iterator end  {std::next(cbegin(), edge[1])};
        add_edge(begin, end);
    }
}

template <class Key, class T, class Cost, Nature Nat>
std::unique_ptr<std::string> graph<Key, T, Cost, Nat>::generate_grp() const {
    using std::max;
    using std::ostringstream;
    using std::setw;
    using std::string;
    using std::stringstream;

    const string tab{string(4, ' ')};

    string data;

    //! Displaying nature + graph types
    if (get_nature() == DIRECTED) {
        data += "di";
    }

    data += "graph<" + detail::type_name<Key>() + ", " +
            detail::type_name<T>() + ", " +
            detail::type_name<Cost>() + "> {\n" +
            tab + "nodes: {\n";

    //! Displaying nodes:  "<name>"; "<value>",
    size_type max_size{3};
    for_each(cbegin(), cend(), [&max_size](const value_type & element) {
        ostringstream out;
        out << element.first;
        size_type size{static_cast<size_type>(out.tellp())};

        max_size = max(max_size, size);
    });

    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        ostringstream out;
        out << it->first << "\",";

        stringstream ss;
        ss << tab << tab
           << '"' << std::left << setw(static_cast<int>(max_size + 3)) << out.str()
           << '"' << it->second->get() << '"';
        data += ss.str();

        if (it != --cend()) {
            data += ";";
        }
        data += "\n";
    }
    data += tab + "}";

    //! Displaying edges:  "<node 1>"; "<node 2>"; "<cost>",
    if (get_nbr_edges() == 0) {
        data += "\n}";
    } else {
        data += ",\n" + tab + "edges: {\n";
        size_type max_size_1{0}, max_size_2{0};

        for_each(cbegin(), cend(), [&max_size_1, &max_size_2, this](const value_type & element) {
            std::list<typename node::edge> child{element.second->get_edges()};
            for_each(child.cbegin(), child.cend(), [ &, this](const typename node::edge & i) {
                ostringstream out_1, out_2;
                out_1 << element.first;
                out_2 << i.target()->first;

                size_type size_1{static_cast<size_type>(out_1.tellp())},
                          size_2{static_cast<size_type>(out_2.tellp())};

                max_size_1 = max(max_size_1, size_1);
                max_size_2 = max(max_size_2, size_2);
            });
        });

        size_type p{0};
        for_each(cbegin(), cend(), [ =, &data, &p](const value_type & element) {
            std::list<typename node::edge> child{element.second->get_edges()};
            for_each(child.cbegin(), child.cend(), [ =, &data, &p](const typename node::edge & i) {
                ostringstream out_1, out_2;
                out_1 << '"' << element.first     << "\",";
                out_2 << '"' << i.target()->first << "\",";

                stringstream ss;
                ss << std::left << tab << tab
                   << setw(static_cast<int>(max_size_1 + 4)) << out_1.str()
                   << setw(static_cast<int>(max_size_2 + 4)) << out_2.str();
                if (i.cost() == infinity) {
                    ss << "infinity";
                } else {
                    ss << '"' << i.cost() << '"';
                }

                if (p < (this->get_nature() == DIRECTED ?
                         this->get_nbr_edges() : 2 * this->get_nbr_edges()) - 1) {
                    ss << ';';
                }
                ss << '\n';
                data += ss.str();
                p++;
            });
        });
        data += tab + "}\n}";
    }

    return std::unique_ptr<std::string>(new std::string(std::forward<std::string>(data)));
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::save_to_grp(const char* filename) const {
    std::ofstream out(filename);
    out << *generate_grp() << std::endl;
    out.close();
}

template <class Key, class T, class Cost, Nature Nat>
void graph<Key, T, Cost, Nat>::parse_from_grp(std::istream &is) {
    //! Nature
    std::string line;
    getline(is, line);
    if (line.substr(0, 5) == "graph") {
        if (get_nature() != UNDIRECTED) {
            GRAPH_THROW_WITH(invalid_argument, "Bad graph nature (expected 'graph')")
        }
    } else if (line.substr(0, 7) == "digraph") {
        if (get_nature() != DIRECTED) {
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

    clear();
    while (getline(is, line) && line.find("}") == std::string::npos) {
        std::istringstream iss{line};
        Key key;
        T value;
        detail::read_T(iss, key);
        detail::read_T(iss, value);

        add_node(key, value);
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

            add_edge(from, to, cost);
        }

        getline(is, line);
        if (line != "}") {
            GRAPH_THROW_WITH(parse_error, static_cast<std::size_t>(is.tellg()), "Bad format at the end of the graph")
        }
    }
}

template <class Key, class T, class Cost, Nature Nat>
template <class K,   class D, class C,    Nature N>
bool graph<Key, T, Cost, Nat>::operator==(const graph<K, D, C, N> &other) const noexcept {
    typedef typename graph<Key, T, Cost, Nat>::const_iterator Iterator1;
    typedef typename graph<K,   D,    C,   N>::const_iterator Iterator2;

    typedef typename basic_node<T, Cost, typename graph<Key, T, Cost, Nat>::iterator, Iterator1>::edge Edge1;
    typedef typename basic_node<D,    C, typename graph<K,   D, C,      N>::iterator, Iterator2>::edge Edge2;

    if (get_nature()        != other.get_nature()    ||
            get_nbr_nodes() != other.get_nbr_nodes() ||
            get_nbr_edges() != other.get_nbr_edges()) {
        return false;
    }

    Iterator1 it1{cbegin()};
    Iterator2 it2{other.cbegin()};

    for (; (it1 != cend() || it2 != other.cend()); ++it1, ++it2) {
        //! Test nodes value
        if (!(it1->first                == it2->first)         ||
                !(it1->second->get()    == it2->second->get()) ||
                !(it1->second->degree() == it2->second->degree())) {
            return false;
        }

        //! Test out edges
        std::list<Edge1> edges1{it1->second->get_edges()};
        std::list<Edge2> edges2{it2->second->get_edges()};

        /// Transform an unordered list into a map to browse edges in the same order
        std::map<Key, Edge1> child1;
        std::map<Key, Edge2> child2;

        std::transform(edges1.begin(), edges1.end(), std::inserter(child1, child1.end()), [](const Edge1 & e) {
            return std::make_pair(e.target()->first, e);
        });
        std::transform(edges2.begin(), edges2.end(), std::inserter(child2, child2.end()), [](const Edge2 & e) {
            return std::make_pair(e.target()->first, e);
        });

        typename std::map<Key, Edge1>::const_iterator edge1{child1.cbegin()};
        typename std::map<Key, Edge2>::const_iterator edge2{child2.cbegin()};

        for (; (edge1 != child1.cend() || edge2 != child2.cend()); ++edge1, ++edge2) {
            Iterator1 target1{edge1->second.target()};
            Iterator2 target2{edge2->second.target()};

            if (!(target1->first == target2->first && edge1->second.cost() == edge2->second.cost())) {
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

///////////////////////////////////////////////
///// IMPLEMENTATION OF SEARCH ALGORITHMS /////
///////////////////////////////////////////////

////////////////////////////////////////
///// First Searches (BFS and DFS) /////
////////////////////////////////////////
template <class Key, class T, class Cost, Nature Nat>
template <bool insertFront>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::abstract_first_search(const_iterator start, std::function<bool(const_iterator)> is_goal) const {
    if (start == cend()) {
        GRAPH_THROW_WITH(invalid_argument, "Start point equals to graph::cend()")
    }

    std::list<const_iterator> expanded;
    std::deque<search_path> frontier;

    {
        search_path p;
        p.push_back({start, Cost()});
        frontier.push_front(std::move(p));
    }

    while (!frontier.empty()) {
        search_path p{frontier.front()};
        frontier.pop_front();

        /// expanded does not contain path's last state
        if (std::find(expanded.cbegin(), expanded.cend(), p.back().first) == expanded.end()) {
            const_iterator last{p.back().first};
            expanded.push_back(last);

            if (is_goal(last)) {
                return p;
            }

            std::vector<typename node::edge> legalActions{get_nature() == DIRECTED ? get_out_edges(last) : get_edges(last)};

            for (typename std::vector<typename node::edge>::const_iterator it{legalActions.cbegin()}; it != legalActions.cend(); ++it) {
                search_path newPath{p};
                newPath.push_back({it->target(), it->cost()});
                if (insertFront) {
                    frontier.push_front(newPath);
                } else {
                    frontier.push_back(newPath);
                }
            }
        }
    }

    /// Could not find a solution
    search_path empty;
    return empty;
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::bfs(key_type start, Key target) const {
    std::list<const_iterator> l;
    l.emplace_back(find(target));
    return bfs(find(start), l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::bfs(key_type start, std::list<key_type> target_list) const {
    std::list<const_iterator> l;
    for (key_type k : target_list) {
        l.emplace_back(find(k));
    }
    return bfs(find(start), l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::bfs(key_type start, std::function<bool(key_type)> is_goal) const {
    return bfs(find(start), [ &is_goal](const_iterator it) {
        return is_goal(it->first);
    });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::bfs(const_iterator start, const_iterator target) const {
    std::list<const_iterator> l;
    l.emplace_back(target);
    return bfs(start, l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::bfs(const_iterator start, std::list<const_iterator> target_list) const {
    return bfs(start, [ &target_list](const_iterator node) -> bool { return std::find(target_list.cbegin(), target_list.cend(), node) != target_list.cend(); });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::bfs(const_iterator start, std::function<bool(const_iterator)> is_goal) const {
    return abstract_first_search<false>(start, is_goal);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dfs(key_type start, Key target) const {
    std::list<const_iterator> l;
    l.emplace_back(find(target));
    return dfs(find(start), l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dfs(key_type start, std::list<key_type> target_list) const {
    std::list<const_iterator> l;
    for (key_type k : target_list) {
        l.emplace_back(find(k));
    }
    return dfs(find(start), l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dfs(key_type start, std::function<bool(key_type)> is_goal) const {
    return dfs(find(start), [ &is_goal](const_iterator it) {
        return is_goal(it->first);
    });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dfs(const_iterator start, const_iterator target) const {
    std::list<const_iterator> l;
    l.emplace_back(target);
    return dfs(start, l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dfs(const_iterator start, std::list<const_iterator> target_list) const {
    return dfs(start, [ &target_list](const_iterator node) -> bool { return std::find(target_list.cbegin(), target_list.cend(), node) != target_list.cend(); });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dfs(const_iterator start, std::function<bool(const_iterator)> is_goal) const {
    return abstract_first_search<true>(start, is_goal);
}

////////////////////////////////
///// Depth-Limited Search /////
////////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dls(key_type start, Key target, size_type depth) const {
    std::list<const_iterator> l;
    l.emplace_back(find(target));
    return dls(find(start), l, depth);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dls(key_type start, std::list<key_type> target_list, size_type depth) const {
    std::list<const_iterator> l;
    for (key_type k : target_list) {
        l.emplace_back(find(k));
    }
    return dls(find(start), l, depth);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dls(key_type start, std::function<bool(key_type)> is_goal, size_type depth) const {
    return dls(find(start), [ &is_goal](const_iterator it) {
        return is_goal(it->first);
    }, depth);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dls(const_iterator start, const_iterator target, size_type depth) const {
    std::list<const_iterator> l;
    l.emplace_back(target);
    return dls(start, l, depth);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dls(const_iterator start, std::list<const_iterator> target_list, size_type depth) const {
    return dls(start, [ &target_list](const_iterator node) -> bool { return std::find(target_list.cbegin(), target_list.cend(), node) != target_list.cend(); }, depth);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::dls(const_iterator start, std::function<bool(const_iterator)> is_goal, size_type depth) const {
    if (start == cend()) {
        GRAPH_THROW_WITH(invalid_argument, "Start point equals to graph::cend()")
    }

    std::list<const_iterator> expanded;
    std::deque<search_path> frontier;

    {
        search_path p;
        p.push_back({start, Cost()});
        frontier.push_front(std::move(p));
    }

    int l{0};
    while (!frontier.empty() && l < depth) {
        search_path p{frontier.front()};
        frontier.pop_front();

        /// expanded does not contain path's last state
        if (std::find(expanded.cbegin(), expanded.cend(), p.back().first) == expanded.end()) {
            const_iterator last{p.back().first};
            expanded.push_back(last);

            if (is_goal(last)) {
                return p;
            }

            std::vector<typename node::edge> legalActions{get_nature() == DIRECTED ? get_out_edges(last) : get_edges(last)};

            for (typename std::vector<typename node::edge>::const_iterator it{legalActions.cbegin()}; it != legalActions.cend(); ++it) {
                search_path newPath{p};
                newPath.push_back({it->target(), it->cost()});
                frontier.push_front(newPath);
            }
        }
        ++l;
    }

    /// Could not find a solution
    search_path empty;
    return empty;
}

//////////////////////////////////////////////////
///// Iterative-Deepening Depth-First Search /////
//////////////////////////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::iddfs(key_type start, Key target) const {
    std::list<const_iterator> l;
    l.emplace_back(find(target));
    return iddfs(find(start), l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::iddfs(key_type start, std::list<key_type> target_list) const {
    std::list<const_iterator> l;
    for (key_type k : target_list) {
        l.emplace_back(find(k));
    }
    return iddfs(find(start), l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::iddfs(key_type start, std::function<bool(key_type)> is_goal) const {
    return iddfs(find(start), [ &is_goal](const_iterator it) {
        return is_goal(it->first);
    });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::iddfs(const_iterator start, const_iterator target) const {
    std::list<const_iterator> l;
    l.emplace_back(target);
    return iddfs(start, l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::iddfs(const_iterator start, std::list<const_iterator> target_list) const {
    return iddfs(start, [ &target_list](const_iterator node) -> bool { return std::find(target_list.cbegin(), target_list.cend(), node) != target_list.cend(); });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::iddfs(const_iterator start, std::function<bool(const_iterator)> is_goal) const {
    if (start == cend()) {
        GRAPH_THROW_WITH(invalid_argument, "Start point equals to graph::cend()")
    }

    for (size_type i{0}; i < std::numeric_limits<size_type>::max(); ++i) {
        search_path found{dls(start, is_goal, i)};
        if (!found.empty()) {
            return found;
        }
    }
    search_path empty;
    return empty;
}

///////////////////////////////
///// Uniform Cost Search /////
///////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::ucs(key_type start, Key target) const {
    std::list<const_iterator> l;
    l.emplace_back(find(target));
    return ucs(find(start), l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::ucs(key_type start, std::list<key_type> target_list) const {
    std::list<const_iterator> l;
    for (key_type k : target_list) {
        l.emplace_back(find(k));
    }
    return ucs(find(start), l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::ucs(key_type start, std::function<bool(key_type)> is_goal) const {
    return ucs(find(start), [ &is_goal](const_iterator it) {
        return is_goal(it->first);
    });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::ucs(const_iterator start, const_iterator target) const {
    std::list<const_iterator> l;
    l.emplace_back(target);
    return ucs(start, l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::ucs(const_iterator start, std::list<const_iterator> target_list) const {
    return ucs(start, [ &target_list](const_iterator node) -> bool { return std::find(target_list.cbegin(), target_list.cend(), node) != target_list.cend(); });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::ucs(const_iterator start, std::function<bool(const_iterator)> is_goal) const {
    if (start == cend()) {
        GRAPH_THROW_WITH(invalid_argument, "Start point equals to graph::cend()")
    }

    std::list<const_iterator> expanded;
    std::priority_queue<search_path, std::vector<search_path>, path_comparator> frontier((path_comparator([](const_iterator) {
        return Cost();
    })));

    {
        search_path p;
        p.push_back({start, Cost()});
        frontier.push(std::move(p));
    }

    while (!frontier.empty()) {
        search_path p{frontier.top()};
        frontier.pop();

        /// expanded does not contain path's last state
        if (std::find(expanded.cbegin(), expanded.cend(), p.back().first) == expanded.end()) {
            const_iterator last{p.back().first};
            expanded.push_back(last);

            if (is_goal(last)) {
                return p;
            }

            std::vector<typename node::edge> legalActions{get_nature() == DIRECTED ? get_out_edges(last) : get_edges(last)};

            for (typename std::vector<typename node::edge>::const_iterator it{legalActions.cbegin()}; it != legalActions.cend(); ++it) {
                search_path newPath{p};
                newPath.push_back({it->target(), it->cost()});
                frontier.push(newPath);
            }
        }
    }

    /// Could not find a solution
    search_path empty;
    return empty;
}

/////////////////////
///// A* Search /////
/////////////////////

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::astar(key_type start, Key target, std::function<Cost(const_iterator)> heuristic) const {
    const_iterator it{find(target)};
    return astar(find(start), [ &it](const_iterator node) -> bool { return it == node; }, heuristic);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::astar(key_type start, std::list<key_type> target_list, std::function<Cost(const_iterator)> heuristic) const {
    std::list<const_iterator> l;
    for (key_type k : target_list) {
        l.emplace_back(find(k));
    }
    return astar(find(start), l, heuristic);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::astar(key_type start, std::function<bool(key_type)> is_goal, std::function<Cost(const_iterator)> heuristic) const {
    return astar(find(start), [ &is_goal](const_iterator it) {
        return is_goal(it->first);
    }, heuristic);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::astar(const_iterator start, const_iterator target, std::function<Cost(const_iterator)> heuristic) const {
    return astar(start, [ &target](const_iterator node) -> bool { return target == node; }, heuristic);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::astar(const_iterator start, std::list<const_iterator> target_list, std::function<Cost(const_iterator)> heuristic) const {
    return astar(start, [ &target_list](const_iterator node) -> bool { return std::find(target_list.cbegin(), target_list.cend(), node) != target_list.cend(); }, heuristic);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::astar(const_iterator start, std::function<bool(const_iterator)> is_goal, std::function<Cost(const_iterator)> heuristic) const {
    if (start == cend()) {
        GRAPH_THROW_WITH(invalid_argument, "Start point equals to graph::cend()")
    }

    std::list<const_iterator> expanded;
    std::priority_queue<search_path, std::vector<search_path>, path_comparator> frontier((path_comparator(heuristic)));

    {
        search_path p;
        p.push_back({start, Cost()});
        frontier.push(std::move(p));
    }

    while (!frontier.empty()) {
        search_path p{frontier.top()};
        frontier.pop();

        /// expanded does not contain path's last state
        if (std::find(expanded.cbegin(), expanded.cend(), p.back().first) == expanded.end()) {
            const_iterator last{p.back().first};
            expanded.push_back(last);

            if (is_goal(last)) {
                return p;
            }

            std::vector<typename node::edge> legalActions{get_nature() == DIRECTED ? get_out_edges(last) : get_edges(last)};

            for (typename std::vector<typename node::edge>::const_iterator it{legalActions.cbegin()}; it != legalActions.cend(); ++it) {
                search_path newPath{p};
                newPath.push_back({it->target(), it->cost()});
                frontier.push(newPath);
            }
        }
    }

    /// Could not find a solution
    search_path empty;
    return empty;
}

////
template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::shortest_paths graph<Key, T, Cost, Nat>::dijkstra(key_type start) const {
    return dijkstra(find(start), [](const_iterator) {
        return false;
    });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::shortest_paths graph<Key, T, Cost, Nat>::dijkstra(key_type start, key_type target) const {
    const_iterator it{find(target)};
    return dijkstra(find(start), [ &it](const_iterator node) -> bool { return it == node; });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::shortest_paths graph<Key, T, Cost, Nat>::dijkstra(key_type start, std::list<key_type> target_list) const {
    std::list<const_iterator> l;
    for (key_type k : target_list) {
        l.emplace_back(find(k));
    }
    return dijkstra(find(start), l);
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::shortest_paths graph<Key, T, Cost, Nat>::dijkstra(key_type start, std::function<bool(key_type)> is_goal) const {
    return dijkstra(find(start), [ &is_goal](const_iterator it) {
        return is_goal(it->first);
    });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::shortest_paths graph<Key, T, Cost, Nat>::dijkstra(const_iterator start) const {
    return dijkstra(start, [](const_iterator) {
        return false;
    });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::shortest_paths graph<Key, T, Cost, Nat>::dijkstra(const_iterator start, const_iterator target) const {
    return dijkstra(start, [ &target](const_iterator node) -> bool { return target == node; });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::shortest_paths graph<Key, T, Cost, Nat>::dijkstra(const_iterator start, std::list<const_iterator> target_list) const {
    return dijkstra(start, [ &target_list](const_iterator node) -> bool { return std::find(target_list.cbegin(), target_list.cend(), node) != target_list.cend(); });
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::shortest_paths graph<Key, T, Cost, Nat>::dijkstra(const_iterator start, std::function<bool(const_iterator)> is_goal) const {
    struct pair_iterator_comparator {
        bool operator()(const std::pair<const_iterator, Cost> &lhs, const std::pair<const_iterator, Cost> &rhs) const {
            return lhs.second < rhs.second;
        }
    };

    //! Initialization

    const Cost nul_cost{Cost()};
    std::priority_queue<std::pair<const_iterator, Cost>, std::vector<std::pair<const_iterator, Cost>>, pair_iterator_comparator> Q;

    shortest_paths result(start);
    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        result.emplace(it, std::make_pair(cend(), infinity));
    }
    Q.emplace(start, nul_cost);
    /// d(start, start) == nul_cost;
    result[start].first = start;
    result[start].second = nul_cost;

    //! Dijkstra's algorithm

    while (!Q.empty()) {
        const_iterator u{Q.top().first};
        Q.pop();

        std::vector<typename node::edge> adj{get_out_edges(u)};
        for (typename std::vector<typename node::edge>::const_iterator it{adj.cbegin()}; it != adj.cend(); ++it) {
            const_iterator v{it->target()};

            Cost cost{v->second->get_cost(u)};

            Cost dist_u{result[u].second};
            Cost dist_v{result[v].second};

            Cost alt{dist_u + cost};

            /// Existing shortest path to v through u with a cost of alt
            if (alt < dist_v) {
                result[v].second = alt;
                result[v].first = u;
                Q.emplace(v, alt);
            }
        }

        if (is_goal(u)) {
            break;
        }
    }

    return result;
}

/////////////////////////////
///// search_path class /////
/////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::search_path::search_path(const search_path &p) : Container(p) {}

template <class Key, class T, class Cost, Nature Nat>
Cost graph<Key, T, Cost, Nat>::search_path::total_cost() const {
    Cost total{};
    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        total += it->second;
    }
    return total;
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::search_path::contain(const graph::const_iterator &i) const {
    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        if (it->first == i) {
            return true;
        }
    }
    return false;
}

////////////////////////////////
///// shortest_paths class /////
////////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::shortest_paths::shortest_paths(graph::const_iterator start) {
    _start = start;
}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::shortest_paths::shortest_paths(const shortest_paths &p) : Container(p) {
    _start = p._start;
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::const_iterator graph<Key, T, Cost, Nat>::shortest_paths::get_previous(graph::const_iterator current) const {
    return find(current)->second.first;
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::shortest_paths::get_path(graph::key_type target) const {
    for (const_iterator it{cbegin()}; it != cend(); ++it) {
        if (it->first->first == target) {
            return get_path(it->first);
        }
    }
    return graph::search_path();
}

template <class Key, class T, class Cost, Nature Nat>
typename graph<Key, T, Cost, Nat>::search_path graph<Key, T, Cost, Nat>::shortest_paths::get_path(graph::const_iterator target) const {
    graph::search_path result;
    graph::shortest_paths::const_iterator current{find(target)}, previous{find(current->second.first)};

    do {
        result.emplace_front(current->first, current->second.second - previous->second.second);
        current = previous;
        previous = find(current->second.first);
    } while (current->first != _start);

    if (target != _start) {
        result.emplace_front(_start, Cost());
    }

    return result;
}

///template <class Key, class T, class Cost, Nature Nat>
///std::ostream &graph<Key, T, Cost, Nat>::operator<<(std::ostream &os, const typename graph::search_path &sp) {
///    Cost count{};
///    for (const std::pair<typename graph<Key, T, Cost, Nat>::const_iterator, Cost> &p : sp) {
///       count += p.second;
///        os << "-> " << p.first->first << " (" << count << ") ";
///    }
///    return os;
///}

template <class Key, class T, class Cost, Nature Nat>
graph<Key, T, Cost, Nat>::path_comparator::path_comparator(std::function<Cost(const_iterator)> heuristic) : _heuristic(heuristic) {}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::path_comparator::operator() (const search_path &p1, const search_path &p2) const {
    return (p2.total_cost() + _heuristic(p2.back().first)) < (p1.total_cost() + _heuristic(p1.back().first));
}

template <class Key, class T, class Cost, Nature Nat>
bool graph<Key, T, Cost, Nat>::iterator_comparator::operator()(const const_iterator &lhs, const const_iterator &rhs) const {
    return lhs->first < rhs->first;
}
