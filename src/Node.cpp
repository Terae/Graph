///
/// Created by Terae on 06/12/17.
///

///////////////////////////////////
///// IMPLEMENTATION OF EDGES /////
///////////////////////////////////

template <class Data, class Cost, class Container, class constContainer>
basic_node<Data, Cost, Container, constContainer>::edge::edge(const std::weak_ptr<basic_node<Data, Cost, Container, constContainer>> &ptr, Cost c) : _target(ptr),
    _cost(std::make_shared<Cost>(c)) {}

template <class Data, class Cost, class Container, class constContainer>
basic_node<Data, Cost, Container, constContainer>::edge::edge(const edge &e) : _target(e._target), _cost(e._cost) {}

template <class Data, class Cost, class Container, class constContainer>
std::tuple<Cost, basic_node<Data, Cost, Container, constContainer>> basic_node<Data, Cost, Container, constContainer>::edge::tie() const {
    return std::tie(*_cost, *_target.lock().get());
}

template <class Data, class Cost, class Container, class constContainer>
bool basic_node<Data, Cost, Container, constContainer>::edge::operator<(const edge &other) const {
    return tie() < other.tie();
}

template <class Data, class Cost, class Container, class constContainer>
bool basic_node<Data, Cost, Container, constContainer>::edge::operator==(const edge &other) const {
    return tie() == other.tie();
}

template <class Data, class Cost, class Container, class constContainer>
constContainer basic_node<Data, Cost, Container, constContainer>::edge::target() const {
    return _target.lock()->container_from_this;
}

template <class Data, class Cost, class Container, class constContainer>
Cost &basic_node<Data, Cost, Container, constContainer>::edge::cost() const {
    return *_cost;
}

///////////////////////////////////
///// IMPLEMENTATION OF NODES /////
///////////////////////////////////

template <class Data, class Cost, class Container, class constContainer>
std::tuple<Data, std::size_t, std::list<typename basic_node<Data, Cost, Container, constContainer>::edge>> basic_node<Data, Cost, Container, constContainer>::tie() const {
    return std::tie(_data, _in_degree, _out_edges);
}

template <class Data, class Cost, class Container, class constContainer>
std::list<typename basic_node<Data, Cost, Container, constContainer>::edge> basic_node<Data, Cost, Container, constContainer>::get_edges() const {
    return _out_edges;
}

template <class Data, class Cost, class Container, class constContainer>
void basic_node<Data, Cost, Container, constContainer>::increment_in_degree(int n) {
    _in_degree += n;
}

template <class Data, class Cost, class Container, class constContainer>
void basic_node<Data, Cost, Container, constContainer>::decrement_in_degree(int n) {
    _in_degree -= n;
}

template <class Data, class Cost, class Container, class constContainer>
bool basic_node<Data, Cost, Container, constContainer>::set_edge(constContainer other, std::shared_ptr<Cost> cost) {
    std::pair<basic_node<Data, Cost, Container, constContainer>::EdgesIterator, bool> new_edge{add_edge(other, *cost)};

    new_edge.first->_cost = cost;

    return new_edge.second;
}

/// Constructors

template <class Data, class Cost, class Container, class constContainer>
basic_node<Data, Cost, Container, constContainer>::basic_node() : basic_node(Data()) {}

template <class Data, class Cost, class Container, class constContainer>
basic_node<Data, Cost, Container, constContainer>::basic_node(const Data &d) : _data(d) {}

template <class Data, class Cost, class Container, class constContainer>
basic_node<Data, Cost, Container, constContainer>::basic_node(const basic_node &n) {
    *this = n;
}

template <class Data, class Cost, class Container, class constContainer>
basic_node<Data, Cost, Container, constContainer> &basic_node<Data, Cost, Container, constContainer>::operator=(const basic_node &n) {
    _data               = n._data;
    _out_edges          = n._out_edges;
    _in_degree          = n._in_degree;
    container_from_this = n.container_from_this;

    return *this;
}

template <class Data, class Cost, class Container, class constContainer>
basic_node<Data, Cost, Container, constContainer>::~basic_node() {}

/// Element access

template <class Data, class Cost, class Container, class constContainer>
inline Data &basic_node<Data, Cost, Container, constContainer>::get() {
    return _data;
}

template <class Data, class Cost, class Container, class constContainer>
inline Data basic_node<Data, Cost, Container, constContainer>::get() const {
    return _data;
}

template <class Data, class Cost, class Container, class constContainer>
Cost &basic_node<Data, Cost, Container, constContainer>::get_cost(const Container &other) {
    std::shared_ptr<basic_node<Data, Cost, Container, constContainer>> ptr(detail::get_value(other, end_container));

    if (ptr == nullptr) {
        GRAPH_THROW(unexpected_nullptr)
    }

    for (EdgesIterator it{_out_edges.begin()}; it != _out_edges.end(); ++it)
        if (it->_target.lock() == ptr) {
            return it->cost();
        }

    //! Link doesn't exist
    _out_edges.emplace_back(std::weak_ptr<basic_node<Data, Cost, Container, constContainer>>(detail::get_value(other, end_container)), infinity);
    ptr->increment_in_degree();
    return (--_out_edges.end())->cost();
}

template <class Data, class Cost, class Container, class constContainer>
const Cost basic_node<Data, Cost, Container, constContainer>::get_cost(constContainer other) const {
    std::shared_ptr<basic_node<Data, Cost, Container, constContainer>> ptr(detail::get_value(other, cend_container));

    if (ptr == nullptr) {
        GRAPH_THROW(unexpected_nullptr)
    }

    for (auto/*EdgesIterator*/ it = _out_edges.begin(); it != _out_edges.end(); ++it)
        if (it->_target.lock() == ptr) {
            return it->cost();
        }

    //! Link doesn't exist
    return infinity;
}

template <class Data, class Cost, class Container, class constContainer>
Cost &basic_node<Data, Cost, Container, constContainer>::operator[](Container other) {
    return get_cost(other);
}

template <class Data, class Cost, class Container, class constContainer>
const Cost basic_node<Data, Cost, Container, constContainer>::operator[](constContainer other) const {
    return get_cost(other);
}

/// Modifiers
template <class Data, class Cost, class Container, class constContainer>
template <class T_data>
inline void basic_node<Data, Cost, Container, constContainer>::set(const T_data &d) {
    _data = static_cast<Data>(d);
}

template <class Data, class Cost, class Container, class constContainer>
template <class T_cost>
void basic_node<Data, Cost, Container, constContainer>::set_cost(Container other, const T_cost &c) {
    std::shared_ptr<basic_node<Data, Cost, Container, constContainer>> ptr(detail::get_value(other, end_container));

    if (ptr == nullptr) {
        GRAPH_THROW(unexpected_nullptr)
    }

    Cost new_cost{static_cast<Cost>(c)};
    bool existing_edge{false};

    for (EdgesIterator it{_out_edges.begin()}; it != _out_edges.end(); ++it) {
        if (it->_target.lock() == ptr) {
            it->cost() = new_cost;
            existing_edge = true;
        }
    }

    if (!existing_edge) {
        this->add_edge(other, new_cost);
    }
}

///template <class Data, class Cost, class Container, class constContainer>
///std::pair<std::list<typename basic_node<Data, Cost, Container, constContainer>::edge>::iterator, bool> basic_node<Data, Cost, Container, constContainer>::add_edge(constContainer other, Cost cost) {
///    std::shared_ptr<basic_node<Data, Cost, Container, constContainer> > ptr{detail::get_value(other, cend_container)};
///
///    if (ptr == nullptr) {
///        GRAPH_THROW(unexpected_nullptr)
///    }
///
///    for (EdgesIterator it{_out_edges.begin()}; it != _out_edges.end(); ++it)
///        if (it->target.lock() == ptr) {
///            *it->cost = cost;
///            return std::make_pair(it, false);
///        }
///
///    // Link doesn't exist
///    ptr->increment_in_degree();
///    _out_edges.emplace_back(std::weak_ptr<basic_node<Data, Cost, Container, constContainer> >(ptr), cost);
///    std::pair<EdgesIterator, bool> result{std::make_pair(--_out_edges.end(), true)};
///    return result;
///}

template <class Data, class Cost, class Container, class constContainer>
bool basic_node<Data, Cost, Container, constContainer>::del_edge(constContainer other) {
    return del_edge_if(other, [](const edge &) {
        return true;
    } );
}

template <class Data, class Cost, class Container, class constContainer>
bool basic_node<Data, Cost, Container, constContainer>::del_edge_if(constContainer other, std::function<bool(const edge &)> predicate) {
    for (EdgesIterator it{_out_edges.begin()}; it != _out_edges.end(); ) {
        if (it->_target.lock() == detail::get_value(other, cend_container)) {
            if (predicate(*it)) {
                it = _out_edges.erase(it);
                other->second->decrement_in_degree();
                return true;
            }
            break;
        } else {
            ++it;
        }
    }

    return false;
}

template <class Data, class Cost, class Container, class constContainer>
std::size_t basic_node<Data, Cost, Container, constContainer>::clear_edges() {
    const std::size_t NUM{_out_edges.size()};

    for (EdgesIterator it{_out_edges.begin()}; it != _out_edges.end(); ) {
        it->_target.lock()->decrement_in_degree();
        it = _out_edges.erase(it);
    }

    return NUM;
}

/// Operations

template <class Data, class Cost, class Container, class constContainer>
inline std::pair<std::size_t, std::size_t> basic_node<Data, Cost, Container, constContainer>::degree() const {
    return std::make_pair(_in_degree, _out_edges.size());
}

template <class Data, class Cost, class Container, class constContainer>
bool basic_node<Data, Cost, Container, constContainer>::existing_adjacent_node(constContainer other) const {
    std::shared_ptr<basic_node<Data, Cost, Container, constContainer>> ptr(detail::get_value(other, cend_container));

    if (ptr == nullptr) {
        return false;
    }

    for (auto/*EdgesIterator*/ it = _out_edges.begin(); it != _out_edges.end(); ++it) {
        if (it->_target.lock() == ptr) {
            return true;
        }
    }

    //! Link doesn't exist
    return false;
}

template <class Data, class Cost, class Container, class constContainer>
constexpr bool operator==(const basic_node<Data, Cost, Container, constContainer> &n1, const basic_node<Data, Cost, Container, constContainer> &n2) noexcept {
    return n1.tie() == n2.tie();
}

template <class Data, class Cost, class Container, class constContainer>
constexpr bool operator!=(const basic_node<Data, Cost, Container, constContainer> &n1, const basic_node<Data, Cost, Container, constContainer> &n2) noexcept {
    return !(n1 == n2);
}

template <class Data, class Cost, class Container, class constContainer>
constexpr bool operator<(const basic_node<Data, Cost, Container, constContainer> &n1, const basic_node<Data, Cost, Container, constContainer> &n2) noexcept {
    return n1.tie() < n2.tie();
}

template <class Data, class Cost, class Container, class constContainer>
constexpr bool operator>(const basic_node<Data, Cost, Container, constContainer> &n1, const basic_node<Data, Cost, Container, constContainer> &n2) noexcept {
    return n2 < n1;
}

template <class Data, class Cost, class Container, class constContainer>
constexpr bool operator<=(const basic_node<Data, Cost, Container, constContainer> &n1, const basic_node<Data, Cost, Container, constContainer> &n2) noexcept {
    return !(n2 < n1);
}

template <class Data, class Cost, class Container, class constContainer>
constexpr bool operator>=(const basic_node<Data, Cost, Container, constContainer> &n1, const basic_node<Data, Cost, Container, constContainer> &n2) noexcept {
    return !(n1 < n2);
}
