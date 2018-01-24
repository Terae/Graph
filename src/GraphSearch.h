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
        template <class, class, class, Nature, bool> friend class abstractFirstSearch;
        template <class, class, class, Nature>       friend class DLS;
        template <class, class, class, Nature>       friend class UCS;
        template <class, class, class, Nature>       friend class AStar;
        template <class, class, class, Nature>       friend class Dijkstra;

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
        std::function<double(stateType)> _heuristic;

      public:
        pathComparator(std::function<double(stateType)> heuristic);

        bool operator() (const path<stateType, costType> &, const path<stateType, costType> &);
    };

    ///
    /// @brief Abstract class for all search algorithms
    ///
    /// Subclasses:
    /// - @ref abstractTargetedSearch Abstract class for all search algorithms with a specified target
    /// - @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// - @ref BFS Breadth-First Search algorithm
    /// - @ref DFS Depth-First Search algorithm
    /// - @ref DLS Depth-Limited Search algorithm
    /// - @ref IDDFS Iterative-Deepening Depth-First Search algorithm
    /// - @ref UCS Uniform-Cost Search algorithm
    /// - @ref AStar A* Search algorithm
    /// - @ref Dijkstra Dijkstra algorithm
    ///
    /// @since version 1.1
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class abstractSearch {
      public:
        using state = typename graph<Key, T, Cost, Nat>::const_iterator;

        explicit abstractSearch(const graph<Key, T, Cost, Nat> &, std::function<bool(state)> is_goal);

      protected:
        const graph<Key, T, Cost, Nat> &g;

        std::function<bool(state)> _is_goal;
    };

    ///
    /// @brief Abstract class for all search algorithms with a specified target
    ///
    /// Allow the user to give one or several nodes-goal to the algorithm
    /// instead of giving a function<bool(state)> parameter in entry
    ///
    /// Subclasses:
    /// - @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// - @ref BFS Breadth-First Search algorithm
    /// - @ref DFS Depth-First Search algorithm
    /// - @ref DLS Depth-Limited Search algorithm
    /// - @ref IDDFS Iterative-Deepening Depth-First Search algorithm
    /// - @ref UCS Uniform-Cost Search algorithm
    /// - @ref AStar A* Search algorithm
    ///
    /// @sa @ref abstractSearch Abstract class for all search algorithms
    /// @sa @ref Dijkstra Dijkstra algorithm
    ///
    /// @since version 1.1
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class abstractTargetedSearch : public abstractSearch<Key, T, Cost, Nat> {
      public:
        using abstractSearch<Key, T, Cost, Nat>::abstractSearch;

        using state = typename graph<Key, T, Cost, Nat>::const_iterator;

        explicit abstractTargetedSearch(const graph<Key, T, Cost, Nat> &, state target);
        explicit abstractTargetedSearch(const graph<Key, T, Cost, Nat> &, const std::list<state> &list_target);

      protected:
        std::list<state> _targets;
    };

    ///
    /// @brief Abstract class for Depth-First and Breadth-First Searches
    ///
    /// @tparam insertFront Specialization parameter between DFS (true) and BFS(false) using respectively a `std::stack` and a `std::queue`
    ///
    /// Subclasses:
    /// - @ref BFS Breadth-First Search algorithm
    /// - @ref DFS Depth-First Search algorithm
    ///
    /// @sa @ref abstractSearch Abstract class for all search algorithms
    /// @sa @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// @sa @ref DLS Depth-Limited Search algorithm
    /// @sa @ref IDDFS Iterative-Deepening Depth-First Search algorithm
    /// @sa @ref UCS Uniform-Cost Search algorithm
    /// @sa @ref AStar A* Search algorithm
    /// @sa @ref Dijkstra Dijkstra algorithm
    ///
    template <class Key, class T, class Cost, Nature Nat, bool insertFront>
    class abstractFirstSearch : public abstractTargetedSearch<Key, T, Cost, Nat> {
      public:
        using abstractTargetedSearch<Key, T, Cost, Nat>::abstractTargetedSearch;

        using state = typename abstractTargetedSearch<Key, T, Cost, Nat>::state;

        path<state, Cost> run(state begin) const;
    };

    ///
    /// @brief Breadth-First Search class
    /// @see https://en.wikipedia.org/wiki/Breadth-First_Search
    ///
    /// @sa @ref abstractSearch Abstract class for all search algorithms
    /// @sa @ref abstractTargetedSearch Abstract class for all search algorithms with a specified target
    /// @sa @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// @sa @ref DFS Depth-First Search algorithm
    /// @sa @ref DLS Depth-Limited Search algorithm
    /// @sa @ref IDDFS Iterative-Deepening Depth-First Search algorithm
    /// @sa @ref UCS Uniform-Cost Search algorithm
    /// @sa @ref AStar A* Search algorithm
    /// @sa @ref Dijkstra Dijkstra algorithm
    ///
    /// @since version 1.1
    ///
    template <class Key, class T, class Cost, Nature Nat>
    using BFS = abstractFirstSearch<Key, T, Cost, Nat, false>;

    template <class Key, class T, class Cost, Nature Nat, class... Args>
    constexpr BFS<Key, T, Cost, Nat> make_BFS(const graph<Key, T, Cost, Nat> &, Args &&...);

    ///
    /// @brief Depth-First Search class
    /// @see https://en.wikipedia.org/wiki/Depth-First_Search
    ///
    /// @sa @ref abstractSearch Abstract class for all search algorithms
    /// @sa @ref abstractTargetedSearch Abstract class for all search algorithms with a specified target
    /// @sa @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// @sa @ref BFS Breadth-First Search algorithm
    /// @sa @ref DLS Depth-Limited Search algorithm
    /// @sa @ref IDDFS Iterative-Deepening Depth-First Search algorithm
    /// @sa @ref UCS Uniform-Cost Search algorithm
    /// @sa @ref AStar A* Search algorithm
    /// @sa @ref Dijkstra Dijkstra algorithm
    ///
    /// @since version 1.1
    ///
    template <class Key, class T, class Cost, Nature Nat>
    using DFS = abstractFirstSearch<Key, T, Cost, Nat, true>;

    template <class Key, class T, class Cost, Nature Nat, class... Args>
    constexpr DFS<Key, T, Cost, Nat> make_DFS(const graph<Key, T, Cost, Nat> &, Args &&...);

    ///
    /// @brief Depth-Limited Search class
    /// @see https://en.wikipedia.org/wiki/Iterative_Deepening_Depth-First_Search
    ///
    /// @tparam l Predetermined depth limit 'l' to create a Depth-Limited Search (fix DFS's loop problem). Have to be well choose, in function of the problem.
    ///
    /// @sa @ref abstractSearch Abstract class for all search algorithms
    /// @sa @ref abstractTargetedSearch Abstract class for all search algorithms with a specified target
    /// @sa @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// @sa @ref BFS Breadth-First Search algorithm
    /// @sa @ref DFS Depth-First Search algorithm
    /// @sa @ref IDDFS Iterative-Deepening Depth-First Search algorithm
    /// @sa @ref UCS Uniform-Cost Search algorithm
    /// @sa @ref AStar A* Search algorithm
    /// @sa @ref Dijkstra Dijkstra algorithm
    ///
    /// @since version 1.1
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class DLS : public abstractTargetedSearch<Key, T, Cost, Nat> {
      public:
        using abstractTargetedSearch<Key, T, Cost, Nat>::abstractTargetedSearch;

        using state = typename abstractTargetedSearch<Key, T, Cost, Nat>::state;

        path<state, Cost> run(state begin, int depth) const;
    };

    template <class Key, class T, class Cost, Nature Nat, class... Args>
    constexpr DLS<Key, T, Cost, Nat> make_DLS(const graph<Key, T, Cost, Nat> &, Args &&...);

    ///
    /// @brief Iterative-Deepening Depth-First Search
    /// @see https://en.wikipedia.org/wiki/Iterative_Deepening_Depth-First_Search
    ///
    /// @sa @ref abstractSearch Abstract class for all search algorithms
    /// @sa @ref abstractTargetedSearch Abstract class for all search algorithms with a specified target
    /// @sa @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// @sa @ref BFS Breadth-First Search algorithm
    /// @sa @ref DFS Depth-First Search algorithm
    /// @sa @ref DLS Depth-Limited Search algorithm
    /// @sa @ref UCS Uniform-Cost Search algorithm
    /// @sa @ref AStar A* Search algorithm
    /// @sa @ref Dijkstra Dijkstra algorithm
    ///
    /// @since version 1.1
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class IDDFS : public abstractTargetedSearch<Key, T, Cost, Nat> {
      public:
        using abstractTargetedSearch<Key, T, Cost, Nat>::abstractTargetedSearch;

        using state = typename abstractTargetedSearch<Key, T, Cost, Nat>::state;

        path<state, Cost> run(state begin) const;
    };

    template <class Key, class T, class Cost, Nature Nat, class... Args>
    constexpr IDDFS<Key, T, Cost, Nat> make_IDDFS(const graph<Key, T, Cost, Nat> &, Args &&...);

    ///
    /// @brief Uniform-Cost Search
    /// @see https://en.wikipedia.org/wiki/Talk:Uniform-Cost_Search
    ///
    /// @sa @ref abstractSearch Abstract class for all search algorithms
    /// @sa @ref abstractTargetedSearch Abstract class for all search algorithms with a specified target
    /// @sa @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// @sa @ref BFS Breadth-First Search algorithm
    /// @sa @ref DFS Depth-First Search algorithm
    /// @sa @ref DLS Depth-Limited Search algorithm
    /// @sa @ref IDDFS Iterative-Deepening Depth-First Search algorithm
    /// @sa @ref AStar A* Search algorithm
    /// @sa @ref Dijkstra Dijkstra algorithm
    ///
    /// @since version 1.1
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class UCS : public abstractTargetedSearch<Key, T, Cost, Nat> {
      public:
        using abstractTargetedSearch<Key, T, Cost, Nat>::abstractTargetedSearch;

        using state = typename abstractTargetedSearch<Key, T, Cost, Nat>::state;

        path<state, Cost> run(state begin) const;
    };

    template <class Key, class T, class Cost, Nature Nat, class... Args>
    constexpr UCS<Key, T, Cost, Nat> make_UCS(const graph<Key, T, Cost, Nat> &, Args &&...);

    ///
    /// @brief A* Search class
    /// @see https://en.wikipedia.org/wiki/A*_search_algorithm
    ///
    /// @sa @ref abstractSearch Abstract class for all search algorithms
    /// @sa @ref abstractTargetedSearch Abstract class for all search algorithms with a specified target
    /// @sa @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// @sa @ref BFS Breadth-First Search algorithm
    /// @sa @ref DFS Depth-First Search algorithm
    /// @sa @ref DLS Depth-Limited Search algorithm
    /// @sa @ref IDDFS Iterative-Deepening Depth-First Search algorithm
    /// @sa @ref UCS Uniform-Cost Search algorithm
    /// @sa @ref Dijkstra Dijkstra algorithm
    ///
    /// @since version 1.1
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class AStar : public abstractTargetedSearch<Key, T, Cost, Nat> {
      public:
        using abstractTargetedSearch<Key, T, Cost, Nat>::abstractTargetedSearch;

        using state = typename abstractTargetedSearch<Key, T, Cost, Nat>::state;

        path<state, Cost> run(state begin, std::function<double(state)> heuristic) const;
    };

    template <class Key, class T, class Cost, Nature Nat, class... Args>
    constexpr AStar<Key, T, Cost, Nat> make_AStar(const graph<Key, T, Cost, Nat> &, Args &&...);

    ///
    /// @brief Dijkstra class
    /// @see https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
    ///
    /// @sa @ref abstractSearch Abstract class for all search algorithms
    /// @sa @ref abstractTargetedSearch Abstract class for all search algorithms with a specified target
    /// @sa @ref abstractFirstSearch Abstract class for First Search algorithms (BFS, DFS)
    /// @sa @ref BFS Breadth-First Search algorithm
    /// @sa @ref DFS Depth-First Search algorithm
    /// @sa @ref DLS Depth-Limited Search algorithm
    /// @sa @ref IDDFS Iterative-Deepening Depth-First Search algorithm
    /// @sa @ref UCS Uniform-Cost Search algorithm
    /// @sa @ref AStar A* Search algorithm
    ///
    /// @since version 1.1
    ///
    template <class Key, class T, class Cost, Nature Nat>
    class Dijkstra : public abstractSearch<Key, T, Cost, Nat> {
      public:
        using abstractSearch<Key, T, Cost, Nat>::abstractSearch;

        using state = typename abstractSearch<Key, T, Cost, Nat>::state;

        // TODO
        std::map<state, path<state, Cost>> run(state begin) const;
    };

    template <class Key, class T, class Cost, Nature Nat, class... Args>
    constexpr Dijkstra<Key, T, Cost, Nat> make_Dijkstra(const graph<Key, T, Cost, Nat> &, Args &&...);
}

#include "GraphSearch.cpp"