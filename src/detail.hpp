///
/// Created by Terae on 20/12/17.
///

#ifndef ROOT_DETAIL_H
#define ROOT_DETAIL_H

//! allow to disable exceptions
#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) && not defined(GRAPH_NOEXCEPTION)
    #define GRAPH_THROW(exception_name) throw detail::exception_name::create(__FUNCTION__);
    #if defined(__clang__)
        #define GRAPH_THROW_WITH(exception_name, ...) throw detail::exception_name::create(__FUNCTION__, __VA_ARGS__);
    #elif defined(__GNUC__)
        #define GRAPH_THROW_WITH(exception_name, ...) throw detail::exception_name::create(__FUNCTION__, ##__VA_ARGS__);
    #endif
    #define GRAPH_TRY try
    #define GRAPH_CATCH(exception) catch(exception)
#else
    #define GRAPH_THROW(exception) std::abort()
    #define GRAPH_TRY if (true)
    #define GRAPH_CATCH(exception) if (false)
#endif

#include <functional> /// function
#include <limits>     /// numeric_limits
#include <sstream>

/// #define COUNT_ARGS(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value

enum Nature {
    DIRECTED   = 'd',
    UNDIRECTED = 'u'
};

///
/// @brief unnamed namespace with internal helper functions
///
/// This namespace collects some functions that could not be defined inside the @ref graph class.
///
/// @since version 1.0.0
///
namespace detail {
    ///
    //! SECTION exceptions
    ///

    ///
    /// @brief general exception of the @ref graph class
    ///
    /// This class is an extension of `std::exception` objects.
    /// It is used as the base class for all exceptions thrown
    /// by the @ref graph class for logical errors.
    /// This class can hence be used as "wildcard" to catch exceptions.
    ///
    /// Subclasses:
    /// - @ref invalid_argument for exceptions indicating invalid arguments given to some function
    /// - @ref unexpected_nullptr for exceptions indicating an unexpected nullptr in entry
    /// - @ref parse_error for exceptions indicating a parse error
    /// - @ref bad_graph for exceptions indicating a graph-logical error in the usage of the @ref graph class
    /// - @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
    /// - @ref negative_weight_cycle for exceptions indicating a negative-weight cycle error, throwable in some search algorithms
    /// - @ref not_complete for exceptions indicating a non-complete graph error, throwable in some search algorithms
    /// - @ref has_cycle for exceptions indicating a cycle error, throwable in some algorithms
    ///
    /// @since version 1.0.0
    ///
    struct exception : public std::exception {
        //! returns the explanatory string
        [[nodiscard]] const char* what() const noexcept override {
            return m.what();
        }

      protected:
        explicit exception(const char* what_arg) : m(what_arg) {}

        static std::string name(const std::string &exception_type) {
            return "[graph.exception." + exception_type + "] ";
        }

      private:
        std::logic_error m;
    };

    ///
    /// @brief exception indicating an invalid argument
    ///
    /// This exception is thrown by the library when an invalid argument occurs.
    /// Invalid argument can occur when calling a function with a bad [template] argument.
    ///
    /// @sa @ref exception for the base class of the library exceptions
    /// @sa @ref unexpected_nullptr for exceptions indicating an unexpected nullptr in entry
    /// @sa @ref parse_error for exceptions indicating a parse error
    /// @sa @ref bad_graph for exceptions indicating a graph-logical error in the usage of the @ref graph class
    /// @sa @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
    /// @sa @ref negative_weight_cycle for exceptions indicating a negative-weight cycle error, throwable in some search algorithms
    /// @sa @ref not_complete for exceptions indicating a non-complete graph error, throwable in some search algorithms
    /// @sa @ref has_cycle for exceptions indicating a cycle error, throwable in some algorithms
    ///
    /// @since version 1.0.0
    ///
    struct invalid_argument final : public exception {
        ///
        /// @brief create an invalid argument exception
        /// @param[in] function_name the function from which the exceptions occurs (returned by the __FUNCTION__ macro)
        /// @param[in] what_arg the explanatory string
        /// @return invalid_argument object
        ///
        static invalid_argument create(const std::string &function_name,
                                       const std::string &what_arg = "Invalid argument") {
            std::string w{exception::name("invalid_argument") + what_arg + " when calling '" + function_name + "'."};
            return invalid_argument(w.c_str());
        }

      private:
        explicit invalid_argument(const char* what_arg) : exception(what_arg) {}
    };

    ///
    /// @brief exception indicating an unexpected nullptr
    ///
    /// This exception is thrown by the library when an unexpected nullptr is used.
    /// Unexpected nullptr can appear in entry of a @ref basic_node function,
    /// as a nullptr, a `std::shared_ptr` type or else as an empty iterator.
    ///
    /// @sa @ref exception for the base class of the library exceptions
    /// @sa @ref invalid_argument for exceptions indicating invalid arguments given to some function
    /// @sa @ref parse_error for exceptions indicating a parse error
    /// @sa @ref bad_graph for exceptions indicating a graph-logical error in the usage of the @ref graph class
    /// @sa @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
    /// @sa @ref negative_weight_cycle for exceptions indicating a negative-weight cycle error, throwable in some search algorithms
    /// @sa @ref not_complete for exceptions indicating a non-complete graph error, throwable in some search algorithms
    /// @sa @ref has_cycle for exceptions indicating a cycle error, throwable in some algorithms
    ///
    /// @since version 1.0.0
    ///
    struct unexpected_nullptr final : public exception {
        ///
        /// @brief create an unexpected nullptr exception
        /// @param[in] function_name the function from which the exceptions occurs (returned by the __FUNCTION__ macro)
        /// @param[in] what_arg the explanatory string
        /// @return unexpected_nullptr object
        ///
        static unexpected_nullptr create(const std::string &function_name,
                                         const std::string &what_arg = "Unexpected nullptr") {
            std::string w{exception::name("unexpected_nullptr") + what_arg + " when calling '" + function_name + "'."};
            return unexpected_nullptr(w.c_str());
        }

      private:
        explicit unexpected_nullptr(const char* what_arg) : exception(what_arg) {}
    };

    ///
    /// @brief exception indicating a parse error
    ///
    /// This exception is thrown by the library when a parse error occurs.
    /// Parse errors can occur during a call of `graph::operator>>` function.
    ///
    /// Member @a byte holds the byte index of the last read character in the input file
    ///
    /// @note For an input with n bytes, 1 is the index of the first character and n+1
    ///       is the index of the terminating null byte or the end of file. This also
    ///       holds true when reading a byte vector or any stream.
    ///
    /// @sa @ref exception for the base class of the library exceptions
    /// @sa @ref invalid_argument for exceptions indicating invalid arguments given to some function
    /// @sa @ref unexpected_nullptr for exceptions indicating an unexpected nullptr in entry
    /// @sa @ref bad_graph for exceptions indicating a graph-logical error in the usage of the @ref graph class
    /// @sa @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
    /// @sa @ref negative_weight_cycle for exceptions indicating a negative-weight cycle error, throwable in some search algorithms
    /// @sa @ref not_complete for exceptions indicating a non-complete graph error, throwable in some search algorithms
    /// @sa @ref has_cycle for exceptions indicating a cycle error, throwable in some algorithms
    ///
    /// @since version 1.0.0
    ///
    struct parse_error final : public exception {
        ///
        /// @brief create a parse error exception
        /// @param[in] function_name the function from which the exceptions occurs (returned by the __FUNCTION__ macro)
        /// @param[in] byte the byte index where the error occured (or 0 if the position cannot be determined)
        /// @param[in] what_arg the explanatory string
        /// @return unexpected_nullptr object
        ///
        static parse_error create(const std::string &function_name,
                                  std::size_t byte,
                                  const std::string &what_arg = "Bad format") {
            std::string w{exception::name("parse_error") + "parse error"
                          + (byte != 0 ? " at " + std::to_string(byte) : "")
                          + ": " + what_arg + " when calling '" + function_name + "'."};
            return parse_error(w.c_str(), byte);
        }

        ///
        /// @brief byte index of the parse error
        ///
        /// The byte index of the last read character in the input file
        ///
        /// @note For an input with n bytes, 1 is the index of the first character and n+1
        ///       is the index of the terminating null byte or the end of file. This also
        ///       holds true when reading a byte vector or any stream.
        const std::size_t byte;

      private:
        explicit parse_error(const char* what_arg, std::size_t b) : exception(what_arg), byte(b) {}
    };

    ///
    /// @brief specialized logical exception of the @ref graph class
    ///
    /// This class is an extension of @ref exception objects.
    /// It is used as the base class for all exceptions thrown by the
    /// @ref graph class for logical errors especially for graph logic.
    ///
    /// Subclasses:
    /// - @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
    /// - @ref negative_weight_cycle for exceptions indicating a negative-weight cycle error, throwable in some search algorithms
    /// - @ref not_complete for exceptions indicating a non-complete graph error, throwable in some search algorithms
    /// - @ref has_cycle for exceptions indicating a cycle error, throwable in some algorithms
    ///
    /// @sa @ref exception for the base class of the library exceptions
    /// @sa @ref invalid_argument for exceptions indicating invalid arguments given to some function
    /// @sa @ref unexpected_nullptr for exceptions indicating an unexpected nullptr in entry
    /// @sa @ref parse_error for exceptions indicating a parse error
    /// @sa @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
    /// @sa @ref negative_weight_cycle for exceptions indicating a negative-weight cycle error, throwable in some search algorithms
    /// @sa @ref not_complete for exceptions indicating a non-complete graph error, throwable in some search algorithms
    ///
    /// @since version 1.0.0
    ///
    struct bad_graph : public exception {
      protected:
        explicit bad_graph(const char* what_arg) : exception(what_arg) {}

        static std::string name(const std::string &exception_type) {
            return "[graph.exception.bad_graph." + exception_type + "] ";
        }
    };

    ///
    /// @brief exception indicating a negative edge error
    ///
    /// This exception is thrown by the library when a graph has at least a negative edge.
    /// Negative edges are a problem in some search algorithms.
    ///
    /// @sa @ref exception for the base class of the library exceptions
    /// @sa @ref invalid_argument for exceptions indicating invalid arguments given to some function
    /// @sa @ref unexpected_nullptr for exceptions indicating an unexpected nullptr in entry
    /// @sa @ref parse_error for exceptions indicating a parse error
    /// @sa @ref bad_graph for exceptions indicating a graph-logical error in the usage of the @ref graph class
    /// @sa @ref negative_weight_cycle for exceptions indicating a negative-weight cycle error, throwable in some search algorithms
    /// @sa @ref not_complete for exceptions indicating a non-complete graph error, throwable in some search algorithms
    /// @sa @ref has_cycle for exceptions indicating a cycle error, throwable in some algorithms
    ///
    /// @since version 1.0.0
    ///
    struct negative_edge final : public bad_graph {
        ///
        /// @brief create a negative_edge exception
        /// @param[in] function_name the function from which the exceptions occurs (returned by the __FUNCTION__ macro)
        /// @param[in] what_arg the explanatory string
        /// @return unexpected_nullptr object
        ///
        static negative_edge create(const std::string &function_name,
                                    const std::string &what_arg = "Edge with negative weight") {
            std::string w{bad_graph::name("negative_edge") + what_arg + " when calling '" + function_name + "'."};
            return negative_edge(w.c_str());
        }

      private:
        explicit negative_edge(const char* what_arg) : bad_graph(what_arg) {}
    };

    ///
    /// @brief exception indicating a negative edge error
    ///
    /// This exception is thrown by the library when a graph has at least a negative edge.
    /// Negative edges are a problem in some search algorithms.
    ///
    /// @sa @ref exception for the base class of the library exceptions
    /// @sa @ref invalid_argument for exceptions indicating invalid arguments given to some function
    /// @sa @ref unexpected_nullptr for exceptions indicating an unexpected nullptr in entry
    /// @sa @ref parse_error for exceptions indicating a parse error
    /// @sa @ref bad_graph for exceptions indicating a graph-logical error in the usage of the @ref graph class
    /// @sa @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
    /// @sa @ref not_complete for exceptions indicating a non-complete graph error, throwable in some search algorithms
    /// @sa @ref has_cycle for exceptions indicating a cycle error, throwable in some algorithms
    ///
    /// @since version 1.0.0
    ///
    struct negative_weight_cycle final : public bad_graph {
        ///
        /// @brief create a negative_weight_cycle exception
        /// @param[in] function_name the function from which the exceptions occurs (returned by the __FUNCTION__ macro)
        /// @param[in] what_arg the explanatory string
        /// @return unexpected_nullptr object
        ///
        static negative_weight_cycle create(const std::string &function_name,
                                            const std::string &what_arg = "Negative-weight cycle") {
            std::string w{bad_graph::name("negative_weight_cycle") + what_arg + " when calling '" + function_name + "'."};
            return negative_weight_cycle(w.c_str());
        }

      private:
        explicit negative_weight_cycle(const char* what_arg) : bad_graph(what_arg) {}
    };

    ///
    /// @brief exception indicating a not-complete graph error
    ///
    /// This exception is thrown by the library when a graph is not complete.
    /// A non-complete graph is a problem in some search algorithms.
    ///
    /// @sa @ref exception for the base class of the library exceptions
    /// @sa @ref invalid_argument for exceptions indicating invalid arguments given to some function
    /// @sa @ref unexpected_nullptr for exceptions indicating an unexpected nullptr in entry
    /// @sa @ref parse_error for exceptions indicating a parse error
    /// @sa @ref bad_graph for exceptions indicating a graph-logical error in the usage of the @ref graph class
    /// @sa @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
    /// @sa @ref negative_weight_cycle for exceptions indicating a negative-weight cycle error, throwable in some search algorithms
    /// @sa @ref has_cycle for exceptions indicating a cycle error, throwable in some algorithms
    ///
    /// @since version 1.0.0
    ///
    struct not_complete final : public bad_graph {
        ///
        /// @brief create a not_complete exception
        /// @param[in] function_name the function from which the exceptions occurs (returned by the __FUNCTION__ macro)
        /// @param[in] what_arg the explanatory string
        /// @return unexpected_nullptr object
        ///
        static not_complete create(const std::string &function_name,
                                   const std::string &what_arg = "Not complete graph") {
            std::string w{bad_graph::name("not_complete") + what_arg + " when calling '" + function_name + "'."};
            return not_complete(w.c_str());
        }

      private:
        explicit not_complete(const char* what_arg) : bad_graph(what_arg) {}
    };

    ///
    /// @brief exception indicating a not-complete graph error
    ///
    /// This exception is thrown by the library when a graph is not complete.
    /// A non-complete graph is a problem in some search algorithms.
    ///
    /// @sa @ref exception for the base class of the library exceptions
    /// @sa @ref invalid_argument for exceptions indicating invalid arguments given to some function
    /// @sa @ref unexpected_nullptr for exceptions indicating an unexpected nullptr in entry
    /// @sa @ref parse_error for exceptions indicating a parse error
    /// @sa @ref bad_graph for exceptions indicating a graph-logical error in the usage of the @ref graph class
    /// @sa @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
    /// @sa @ref negative_weight_cycle for exceptions indicating a negative-weight cycle error, throwable in some search algorithms
    /// @sa @ref not_complete for exceptions indicating a non-complete graph error, throwable in some search algorithms
    ///
    /// @since version 1.0.0
    ///
    struct has_cycle final : public bad_graph {
        ///
        /// @brief create a not_complete exception
        /// @param[in] function_name the function from which the exceptions occurs (returned by the __FUNCTION__ macro)
        /// @param[in] what_arg the explanatory string
        /// @return unexpected_nullptr object
        ///
        static has_cycle create(const std::string &function_name,
                                const std::string &what_arg = "Graph with a cycle") {
            std::string w{bad_graph::name("has_cycle") + what_arg + " when calling '" + function_name + "'."};
            return has_cycle(w.c_str());
        }

      private:
        explicit has_cycle(const char* what_arg) : bad_graph(what_arg) {}
    };

    ///
    //! SECTION degree
    ///

    ///
    /// @brief the graph::Degree object
    ///
    /// This object represents the degree type for a graph directed or undirected,
    /// which is respectively a `std::pair<size_type, size_type>` and `size_type`.
    ///
    /// @tparam N nature of the specialized degree. It is internally used to
    /// distinguish between both possible natures of a graph.
    ///
    /// @since version 1.0.0
    ///
    ///

    template <Nature> class basic_degree;

    template <> class basic_degree<DIRECTED> {
      public:
        using value_type = std::pair<std::size_t, std::size_t>;
        using type = basic_degree<DIRECTED>;

        explicit basic_degree(value_type degree) : _deg(std::move(degree)) {};
        basic_degree(std::size_t in, std::size_t out) : basic_degree(std::make_pair(in, out)) {};

        [[nodiscard]] inline value_type get_degree() const {
            return _deg;
        }

        friend bool operator==(const type &t1, const type &t2) {
            return t1._deg == t2._deg;
        }
        friend bool operator==(const value_type &v, const type &t) {
            return t._deg == v;
        }
        friend bool operator==(const type &t, const value_type &v) {
            return t._deg == v;
        }

        friend bool operator< (const type &t1, const type &t2) {
            return t1._deg < t2._deg;
        }

        static basic_degree max() {
            return {std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max()};
        }
        static basic_degree min() {
            return {std::numeric_limits<std::size_t>::min(), std::numeric_limits<std::size_t>::min()};
        }

      private:
        value_type _deg;
    };

    template <> class basic_degree<UNDIRECTED> {
      public:
        using value_type = std::size_t;
        using type = basic_degree<UNDIRECTED>;

        explicit basic_degree(const value_type &d) : _deg(d) {}
        basic_degree(std::size_t in, std::size_t out) : basic_degree(std::max(in, out)) {}

        [[nodiscard]] inline value_type get_degree() const {
            return _deg;
        }

        friend bool operator==(const type &t1, const type &t2) {
            return t1._deg == t2._deg;
        }
        friend bool operator==(const value_type &v, const type &t) {
            return t._deg == v;
        }
        friend bool operator==(const type &t, const value_type &v) {
            return t._deg == v;
        }

        friend bool operator< (const type &t1, const type &t2) {
            return t1._deg < t2._deg;
        }

        static basic_degree max() {
            return basic_degree(std::numeric_limits<std::size_t>::max());
        }
        static basic_degree min() {
            return basic_degree(std::numeric_limits<std::size_t>::min());
        }

      private:
        value_type _deg;
    };

    ///
    //! SECTION helpers
    ///

    template <Nature> struct is_directed           : public std::false_type { };
    template <>       struct is_directed<DIRECTED> : public std::true_type  { };

    template <Nature> struct is_undirected             : public std::false_type { };
    template <>       struct is_undirected<UNDIRECTED> : public std::true_type  { };

#include <memory>
#include <utility>

    //! distinguish value type between map::iterator and shared_ptr: @see https://stackoverflow.com/a/31409532
    template <typename... >
    using void_t = void;

    template <typename T, typename = void>
    struct is_map_iterator : std::false_type { };

    /// *INDENT-OFF*
    template <typename T>
    struct is_map_iterator<T,
            void_t<decltype(++std::declval<T&>()),                     /// incrementable,
                   decltype(*std::declval<T&>()),                      /// dereferencable,
                   decltype(std::declval<T&>() == std::declval<T&>()), /// comparable
                   decltype((*std::declval<T&>()).second)> >           /// pair
            : std::true_type { };
    /// *INDENT-ON*

    /// pointer
    ///template <class V, class = typename std::enable_if<!is_map_iterator<V>::value>::type>
    ///inline V get_value(const V& v, const V&) { return v; }
    /// map iterator
    template <class V, class = typename std::enable_if<is_map_iterator<V>::value>::type>
    inline typename std::iterator_traits<V>::value_type::second_type get_value(const V &v, const V &end) {
        if (v == end) {
            return static_cast<typename std::iterator_traits<V>::value_type::second_type>(nullptr);
        }
        return (*v).second;
    }

#include <cxxabi.h> /// demangled_name, abi::__cxa_demangle

    ///
    /// @brief return the name of a type
    ///
    /// @tparam T object type to extract the name
    ///
    /// @example type_name(4.2f) == "float"
    ///
    //! @return the human readable name of @tparam T
    ///
    template <class T>
    std::string type_name() {
        int status;
        std::string tname{typeid(T).name()};
        char* demangled_name{abi::__cxa_demangle(tname.c_str(), nullptr, nullptr, &status)};

        if (status == 0) {
            tname = demangled_name;
        }
        std::free(demangled_name);

        /// Replace all occurences of 'to_replace' by 'replacement' in 'base'
        /// Usefull to get an human readable 'std::string'
        std::function<void(std::string &, std::string, std::string)> replace_all =
        [](std::string & base, const std::string & to_replace, const std::string & replacement) {
            for (std::string::size_type i{0}; (i = base.find(to_replace, i)) != std::string::npos; ) {
                base.replace(i, to_replace.length(), replacement);
                i += replacement.length();
            }
        };

        replace_all(tname, "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >", "std::string>");
        replace_all(tname, "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >",   "std::string");
        return tname;
    }

    /// Used into the graph::operator>> function
    template <class T>
    std::istream &read_T(std::istream &is, T &t) {
        is.ignore(std::numeric_limits<std::streamsize>::max(), '"');

        std::string str;
        std::getline(is, str, '"');
        std::stringstream ss;
        ss << str;
        if (!(ss >> t)) {
            GRAPH_THROW_WITH(parse_error, 5, "Bad type");
        }

        return is;
    }
    template<> inline
    std::istream &read_T<std::string>(std::istream &is, std::string &str) {
        is.ignore(std::numeric_limits<std::streamsize>::max(), '"');
        std::getline(is, str, '"');
        return is;
    }

    template <class C>
    std::istream &read_cost(std::istream &is, C &c) {
        std::string str = std::string(std::istreambuf_iterator<char>(is),
                                      std::istreambuf_iterator<char>());
        if (str.find_first_of('\"') == std::string::npos &&
                str.find_first_of("infinity") != std::string::npos)
            c = std::numeric_limits<C>::has_infinity ? std::numeric_limits<C>::infinity() :
                std::numeric_limits<C>::max();
        else {
            std::stringstream ss;
            ss << str;
            read_T(ss, c);
        }
        return is;
    }
} /// namespace detail

#endif /// ROOT_DETAIL_H
