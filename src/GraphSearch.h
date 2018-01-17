///
/// Created by Terae on 05/01/18.
///

#include <queue>
#include <stack>
#include "Graph.h"

namespace search {
    template <class stateType, class costType>
    class path {
        template <class, class>                      friend class pathComparator;
        template <class, class, class, Nature>       friend class abstractSearch;
        template <class, class, class, Nature, bool> friend class abstractFirstSearch;
        template <class, class, class, Nature>       friend class DLS;
        template <class, class, class, Nature>       friend class UCS;
        template <class, class, class, Nature>       friend class AStar;

      public:
        path();
        path(const path<stateType, costType> &);

        costType total_cost() const;
        inline std::size_t get_nr_steps() const;

        inline bool empty() const;

        inline bool contain_state(const stateType &) const;

        std::pair<stateType, costType> pop_front();

      private:
        inline void addState(const stateType &);
        inline void addCost (const  costType &);


        inline stateType back_state()  const;

        std::deque<stateType> _states;
        std::deque<costType>  _costs;
    };

    template <class stateType, class costType>
    class pathComparator {
      private:
        stateType _target;
        std::function<double(stateType, stateType)> _heuristic;

      public:
        pathComparator(stateType target, std::function<double(stateType, stateType)> heuristic);

        bool operator() (const path<stateType, costType> &, const path<stateType, costType> &);
    };

    ///
    /// @brief Abstract class of all search algorithms
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class abstractSearch {
      protected:
        const graph<Key, T, Cost, Nat> &g;
        abstractSearch(const graph<Key, T, Cost, Nat> &);
    };

    ///
    /// @brief Abstract class for Depth-First and Breadth-First Searches
    /// @tparam insertFront Specialization parameter between DFS (true) and BFS(false) using respectively a `std::stack` and a `std::queue`
    ///
    template <class Key, class T, class Cost, Nature Nat, bool insertFront>
    class abstractFirstSearch : public abstractSearch<Key, T, Cost, Nat> {
      public:
        using state = typename graph<Key, T, Cost, Nat>::const_iterator;
        abstractFirstSearch(const graph<Key, T, Cost, Nat> &);

        path<state, Cost> run(state begin, state target);
    };

    ///
    /// @brief Breadth-First Search class
    /// @see https://en.wikipedia.org/wiki/Breadth-First_Search
    ///
    template <class Key, class T, class Cost, Nature Nat>
    using BFS = abstractFirstSearch<Key, T, Cost, Nat, false>;

    template <class Key, class T, class Cost, Nature Nat>
    constexpr BFS<Key, T, Cost, Nat> make_BFS(const graph<Key, T, Cost, Nat> &);

    ///
    /// @brief Depth-First Search class
    /// @see https://en.wikipedia.org/wiki/Depth-First_Search
    ///
    template <class Key, class T, class Cost, Nature Nat>
    using DFS = abstractFirstSearch<Key, T, Cost, Nat, true>;

    template <class Key, class T, class Cost, Nature Nat>
    constexpr DFS<Key, T, Cost, Nat> make_DFS(const graph<Key, T, Cost, Nat> &);

    ///
    /// @brief Depth-Limited Search class
    /// @see https://en.wikipedia.org/wiki/Iterative_Deepening_Depth-First_Search
    /// @tparam l Predetermined depth limit 'l' to create a Depth-Limited Search (fix DFS's loop problem). Have to be well choose, in function of the problem.
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class DLS : public abstractSearch<Key, T, Cost, Nat> {
      public:
        using state = typename graph<Key, T, Cost, Nat>::const_iterator;
        DLS(const graph<Key, T, Cost, Nat> &);

        path<state, Cost> run(state begin, state target, int depth);
    };

    template <class Key, class T, class Cost, Nature Nat>
    constexpr DLS<Key, T, Cost, Nat> make_DLS(const graph<Key, T, Cost, Nat> &);

    ///
    /// @brief Iterative-Deepening Depth-First Search
    /// @see https://en.wikipedia.org/wiki/Iterative_Deepening_Depth-First_Search
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class IDDFS : public abstractSearch<Key, T, Cost, Nat> {
      public:
        using state = typename graph<Key, T, Cost, Nat>::const_iterator;
        IDDFS(const graph<Key, T, Cost, Nat> &);

        path<state, Cost> run(state begin, state target);
    };

    template <class Key, class T, class Cost, Nature Nat>
    constexpr IDDFS<Key, T, Cost, Nat> make_IDDFS(const graph<Key, T, Cost, Nat> &);

    ///
    /// @brief Uniform-Cost Search
    /// @see https://en.wikipedia.org/wiki/Talk:Uniform-Cost_Search
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class UCS : public abstractSearch<Key, T, Cost, Nat> {
      public:
        using state = typename graph<Key, T, Cost, Nat>::const_iterator;
        UCS(const graph<Key, T, Cost, Nat> &);

        path<state, Cost> run(state begin, state target);
    };

    template <class Key, class T, class Cost, Nature Nat>
    constexpr UCS<Key, T, Cost, Nat> make_UCS(const graph<Key, T, Cost, Nat> &);

    ///
    /// @brief A* Search class
    /// @see https://en.wikipedia.org/wiki/A*_search_algorithm
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class AStar : public abstractSearch<Key, T, Cost, Nat> {
      public:
        using state = typename graph<Key, T, Cost, Nat>::const_iterator;
        AStar(const graph<Key, T, Cost, Nat> &g);

        path<state, Cost> run(state begin, state target, std::function<double(state, state)> heuristic);
    };

    template <class Key, class T, class Cost, Nature Nat>
    constexpr AStar<Key, T, Cost, Nat> make_AStar(const graph<Key, T, Cost, Nat> &);
}

#include "GraphSearch.cpp"