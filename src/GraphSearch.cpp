///
/// Created by Terae on 05/01/18.
///

//////////////////////////////////////
///// IMPLEMENTATION OF THE PATH /////
//////////////////////////////////////

template <class stateType, class costType>
search::path<stateType, costType>::path() {}

template <class stateType, class costType>
search::path<stateType, costType>::path(const search::path<stateType, costType> &p) {
    _states = p._states;
    _costs  = p._costs;
}

template <class stateType, class costType>
void search::path<stateType, costType>::addState(const stateType &state) {
    _states.push_back(state);
}

template <class stateType, class costType>
void search::path<stateType, costType>::addCost(const costType &cost) {
    _costs.push_back(cost);
}

template <class stateType, class costType>
stateType search::path<stateType, costType>::back_state() const {
    return _states.back();
}

template <class stateType, class costType>
costType search::path<stateType, costType>::total_cost() const {
    costType total{};
    std::for_each(_costs.cbegin(), _costs.cend(), [&total](const costType & c) {
        total += c;
    });
    return total;
}

template <class stateType, class costType>
std::size_t search::path<stateType, costType>::get_nr_steps() const {
    return _states.size();
}

template <class stateType, class costType>
bool search::path<stateType, costType>::empty() const {
    return _states.empty();
}

template <class stateType, class costType>
bool search::path<stateType, costType>::contain_state(const stateType &state) const {
    return std::find(_states.begin(), _states.end(), state) != _states.end();
}

template <class stateType, class costType>
std::pair<stateType, costType> search::path<stateType, costType>::pop_front() {
    costType cost{0};
    if (_states.size() == _costs.size()) {
        cost = _costs.front();
        _costs.pop_front();
    }

    stateType state{_states.front()};
    _states.pop_front();

    return make_pair(state, cost);
}

/////////////////////////////////////////////////
///// IMPLEMENTATION OF THE PATH COMPARATOR /////
/////////////////////////////////////////////////

template <class stateType, class costType>
search::path_comparator<stateType, costType>::path_comparator(std::function<double(stateType)> h) : _heuristic(h) {}

template <class stateType, class costType>
bool search::path_comparator<stateType, costType>::operator() (const path<stateType, costType> &p1, const path<stateType, costType> &p2) {
    return (p1.total_cost() + _heuristic(p1.back_state())) > (p2.total_cost() + _heuristic(p2.back_state()));
}

/////////////////////////////////////////////////
///// IMPLEMENTATION OF THE ABSTRACT SEARCH /////
/////////////////////////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
search::abstract_search<Key, T, Cost, Nat>::abstract_search(const graph<Key, T, Cost, Nat> &g_, std::function<bool(state)> is_goal) : g(g_), _is_goal(is_goal) {}

template <class Key, class T, class Cost, Nature Nat>
search::abstract_targeted_search<Key, T, Cost, Nat>::abstract_targeted_search(const graph<Key, T, Cost, Nat> &g, state target) : abstract_search<Key, T, Cost, Nat>(g, [this](state node) -> bool { return std::find(_targets.cbegin(), _targets.cend(), node) != _targets.cend(); }) {
    this->_targets.emplace_back(target);
}

template <class Key, class T, class Cost, Nature Nat>
search::abstract_targeted_search<Key, T, Cost, Nat>::abstract_targeted_search(const graph<Key, T, Cost, Nat> &g, const std::list<state> &list_target) : abstract_search<Key, T, Cost, Nat>(g, [this](state node) -> bool { return std::find(_targets.cbegin(), _targets.cend(), node) != _targets.cend(); }),
    _targets(list_target) {}

/////////////////////////////////////////////////////
///// IMPLEMENTATION OF FIRST SEARCH ALGORITHMS /////
/////////////////////////////////////////////////////

template <class Key, class T, class Cost, Nature Nat, bool insertFront>
search::path<typename search::abstract_first_search<Key, T, Cost, Nat, insertFront>::state, Cost> search::abstract_first_search<Key, T, Cost, Nat, insertFront>::run(state begin) const {
    std::list<state> expanded;
    std::deque<path<state, Cost>> frontier;

    {
        path<state, Cost> p;
        p.addState(begin);
        frontier.push_front(p);
    }

    while (!frontier.empty()) {
        path<state, Cost> p{frontier.front()};
        frontier.pop_front();

        /// expanded does not contain path's last state
        if (std::find(expanded.cbegin(), expanded.cend(), p.back_state()) == expanded.end()) {
            state last{p.back_state()};
            expanded.push_back(last);

            if (this->_is_goal(last)) {
                return p;
            }

            std::vector<typename graph<Key, T, Cost, Nat>::node::edge> legalActions{this->g.get_nature() == DIRECTED ? this->g.get_out_edges(last) : this->g.get_edges(last)};

            for (typename std::vector<typename graph<Key, T, Cost, Nat>::node::edge>::iterator it{legalActions.begin()}; it != legalActions.end(); ++it) {
                path <state, Cost> newPath{p};
                newPath.addState(it->target());
                newPath.addCost (it->cost());
                if (insertFront) {
                    frontier.push_front(newPath);
                } else {
                    frontier.push_back(newPath);
                }
            }
        }
    }

    /// Could not find a solution
    path<state, Cost> empty;
    return empty;
}

template <class Key, class T, class Cost, Nature Nat, class... Args>
constexpr typename search::bfs<Key, T, Cost, Nat> search::make_bfs(const graph<Key, T, Cost, Nat> &g, Args &&... args) {
    return bfs<Key, T, Cost, Nat>(g, std::forward<Args>(args)...);
}

template <class Key, class T, class Cost, Nature Nat, class... Args>
constexpr typename search::dfs<Key, T, Cost, Nat> search::make_dfs(const graph<Key, T, Cost, Nat> &g, Args &&... args) {
    return dfs<Key, T, Cost, Nat>(g, std::forward<Args>(args)...);
}

////////////////////////////////////////////////////////////
///// IMPLEMENTATION OF DEPTH-LIMITED SEARCH ALGORITHM /////
////////////////////////////////////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
search::path<typename search::dls<Key, T, Cost, Nat>::state, Cost> search::dls<Key, T, Cost, Nat>::run(state begin, int depth) const {
    std::list<state> expanded;
    std::deque<path<state, Cost>> frontier;

    {
        path<state, Cost> p;
        p.addState(begin);
        frontier.push_front(p);
    }

    int l{0};
    while (!frontier.empty() && l < depth) {
        path<state, Cost> p{frontier.front()};
        frontier.pop_front();

        /// expanded does not contain path's last state
        if (std::find(expanded.cbegin(), expanded.cend(), p.back_state()) == expanded.end()) {
            state last{p.back_state()};
            expanded.push_back(last);

            if (this->_is_goal(last)) {
                return p;
            }

            std::vector<typename graph<Key, T, Cost, Nat>::node::edge> legalActions{this->g.get_nature() == DIRECTED ? this->g.get_out_edges(last) : this->g.get_edges(last)};

            for (typename std::vector<typename graph<Key, T, Cost, Nat>::node::edge>::iterator it{legalActions.begin()}; it != legalActions.end(); ++it) {
                path <state, Cost> newPath{p};
                newPath.addState(it->target());
                newPath.addCost (it->cost());
                frontier.push_front(newPath);
            }
        }
        ++l;
    }

    /// Could not find a solution
    path<state, Cost> empty;
    return empty;

    /* // FIXME: Recursive way does not work yet
    if (depth == 0 && node == target) {
        path<state, Cost> p;
        p.addStateBack(node);
        return p;
    }

    if (depth > 0) {
        std::vector<typename graph<Key, T, Cost, Nat>::node::edge> legalActions{this->g.get_nature() == DIRECTED ? this->g.get_out_edges(node) : this->g.get_edges(node)};

        for (typename graph<Key, T, Cost, Nat>::node::edge child : legalActions) {
            path<state, Cost> found{recursive_function(child.target(), target, depth - 1)};
            if (!found.empty()) {
                //found.addStateFront(child.target());
                //found.addCostFront (child.cost());
                return found;
            }
        }
    }
    return path<state, Cost>();*/
}

template <class Key, class T, class Cost, Nature Nat, class... Args>
constexpr typename search::dls<Key, T, Cost, Nat> search::make_dls(const graph<Key, T, Cost, Nat> &g, Args &&... args) {
    return dls<Key, T, Cost, Nat>(g, std::forward<Args>(args)...);
}

template <class Key, class T, class Cost, Nature Nat>
search::path<typename search::iddfs<Key, T, Cost, Nat>::state, Cost> search::iddfs<Key, T, Cost, Nat>::run(state begin) const {
    search::dls<Key, T, Cost, Nat> dls{make_DLS(this->g, this->_targets)};

    for (int i{0}; i < std::numeric_limits<int>::max(); ++i) {
        path<state, Cost> found{dls.run(begin, i)};
        if (!found.empty()) {
            return found;
        }
    }
    return path<state, Cost>();
}

template <class Key, class T, class Cost, Nature Nat, class... Args>
constexpr typename search::iddfs<Key, T, Cost, Nat> search::make_iddfs(const graph<Key, T, Cost, Nat> &g, Args &&... args) {
    return iddfs<Key, T, Cost, Nat>(g, std::forward<Args>(args)...);
}

////////////////////////////////////////////////////
///// IMPLEMENTATION OF UNIFORM-COST ALGORITHM /////
////////////////////////////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
search::path<typename search::ucs<Key, T, Cost, Nat>::state, Cost> search::ucs<Key, T, Cost, Nat>::run(state begin) const {
    std::list<state> expanded;
    std::priority_queue<path<state, Cost>, std::vector<path<state, Cost>>, path_comparator<state, Cost>> frontier(path_comparator<state, Cost>([](state) -> double { return 0; } ));

    {
        path<state, Cost> p;
        p.addState(begin);
        frontier.push(p);
    }

    while (!frontier.empty()) {
        path<state, Cost> p{frontier.top()};
        frontier.pop();

        /// expanded does not contain path's last state
        if (std::find(expanded.cbegin(), expanded.cend(), p.back_state()) == expanded.end()) {
            state last{p.back_state()};
            expanded.push_back(last);

            if (this->_is_goal(last)) {
                return p;
            }

            std::vector<typename graph<Key, T, Cost, Nat>::node::edge> legalActions{this->g.get_nature() == DIRECTED ? this->g.get_out_edges(last) : this->g.get_edges(last)};

            for (typename std::vector<typename graph<Key, T, Cost, Nat>::node::edge>::iterator it{legalActions.begin()}; it != legalActions.end(); ++it) {
                path<state, Cost> newPath{p};
                newPath.addState(it->target());
                newPath.addCost (it->cost());
                frontier.push(newPath);
            }
        }
    }

    /// Could not find a solution
    path<state, Cost> empty;
    return empty;
}

template <class Key, class T, class Cost, Nature Nat, class... Args>
constexpr typename search::ucs<Key, T, Cost, Nat> search::make_ucs(const graph<Key, T, Cost, Nat> &g, Args &&... args) {
    return ucs<Key, T, Cost, Nat>(g, std::forward<Args>(args)...);
}

///////////////////////////////////////////
///// IMPLEMENTATION OF THE A* SEARCH /////
///////////////////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
search::path<typename search::astar<Key, T, Cost, Nat>::state, Cost> search::astar<Key, T, Cost, Nat>::run(state begin, std::function<double(state)> heuristic) const {
    std::list<state> expanded;
    std::priority_queue<path<state, Cost>, std::vector<path<state, Cost>>, path_comparator<state, Cost>> frontier((path_comparator<state, Cost>(heuristic)));

    {
        path<state, Cost> p;
        p.addState(begin);
        frontier.push(p);
    }

    while (!frontier.empty()) {
        path<state, Cost> p{frontier.top()};
        frontier.pop();

        /// expanded does not contain path's last state
        if (std::find(expanded.cbegin(), expanded.cend(), p.back_state()) == expanded.end()) {
            state last{p.back_state()};
            expanded.push_back(last);

            if (this->_is_goal(last)) {
                return p;
            }

            std::vector<typename graph<Key, T, Cost, Nat>::node::edge> legalActions{this->g.get_nature() == DIRECTED ? this->g.get_out_edges(last) : this->g.get_edges(last)};

            for (typename std::vector<typename graph<Key, T, Cost, Nat>::node::edge>::iterator it{legalActions.begin()}; it != legalActions.end(); ++it) {
                path<state, Cost> newPath{p};
                newPath.addState(it->target());
                newPath.addCost (it->cost());
                frontier.push(newPath);
            }
        }
    }

    /// Could not find a solution
    path<state, Cost> empty;
    return empty;
}

template <class Key, class T, class Cost, Nature Nat, class... Args>
constexpr typename search::astar<Key, T, Cost, Nat> search::make_astar(const graph<Key, T, Cost, Nat> &g, Args &&... args) {
    return astar<Key, T, Cost, Nat>(g, std::forward<Args>(args)...);
}

/////////////////////////////////////////////////
///// IMPLEMENTATION OF THE Dijkstra SEARCH /////
/////////////////////////////////////////////////

template <class Key, class T, class Cost, Nature Nat>
std::map<typename search::dijkstra<Key, T, Cost, Nat>::state, search::path<typename search::dijkstra<Key, T, Cost, Nat>::state, Cost>> search::dijkstra<Key, T, Cost, Nat>::run(state begin) const {

}

template <class Key, class T, class Cost, Nature Nat, class... Args>
constexpr typename search::dijkstra<Key, T, Cost, Nat> search::make_dijkstra(const graph<Key, T, Cost, Nat> &g, Args &&... args) {
    return dijkstra<Key, T, Cost, Nat>(g, std::forward<Args>(args)...);
}
