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

/// #define COUNT_ARGS(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value

enum Nature {
    DIRECTED   = 'd',
    UNDIRECTED = 'u'
};

/*!
 @brief unnamed namespace with internal helper functions

 This namespace collects some functions that could not be defined inside the @ref graph class.
 */
namespace detail {
//!
//! @section exceptions
//!

    /*!
     @brief general exception of the @ref graph class

     This class is an extension of `std::exception` objects.
     It is used as the base class for all exceptions thrown
     by the @ref graph class for logical errors.
     This class can hence be used as "wildcard" to catch exceptions.

     Subclasses:
     - @ref invalid_argument for exceptions indicating invalid arguments given to some function
     - @ref unexpected_nullptr for exceptions indicating an unexpected nullptr in entry
     - @ref parse_error for exceptions indicating a parse error
     - @ref bad_graph for exceptions indicating a logical error in the usage of the @ref graph class
     */
    struct exception : public std::exception {
        const char *what() const noexcept override {
            return m.what();
        }

    protected:
        explicit exception(const char *what_arg) : m(what_arg) {}

        static std::string name(const std::string &exception_type) {
            return "[graph.exception." + exception_type + "] ";
        }

    private:
        std::logic_error m;
    };

    struct invalid_argument final : public exception {
        static invalid_argument
        create(const std::string &function_name, const std::string &what_arg = "Invalid argument") {
            std::string w{exception::name("invalid_argument") + what_arg + " when calling '" + function_name + "'."};
            return invalid_argument(w.c_str());
        }

    private:
        explicit invalid_argument(const char *what_arg) : exception(what_arg) {}
    };

    struct unexpected_nullptr final : public exception {
        static unexpected_nullptr create(const std::string &function_name,
                                         const std::string &what_arg = "Unexpected nullptr") {
            std::string w{exception::name("unexpected_nullptr") + what_arg + " when calling '" + function_name + "'."};
            return unexpected_nullptr(w.c_str());
        }

    private:
        explicit unexpected_nullptr(const char *what_arg) : exception(what_arg) {}
    };

    /// @param[in] byte The byte index where the error occured (or 0 if the position cannot be determined)
    struct parse_error final : public exception {
        static parse_error create(const std::string &function_name,
                                  std::size_t byte,
                                  const std::string &what_arg = "Bad format") {
            std::string w{exception::name("parse_error") + "parse error"
                          + (byte != 0 ? " at " + std::to_string(byte) : "")
                          + ": " + what_arg + " when calling '" + function_name + "'."};
            return parse_error(w.c_str(), byte);
        }

        /// The byte index of the last read character in the input file
        const std::size_t byte;

    private:
        explicit parse_error(const char *what_arg, std::size_t b) : exception(what_arg), byte(b) {}
    };

    /*!
     @brief specialized logical exception of the @ref graph class

     Subclasses:
     - @ref negative_edge for exceptions indicating a negative edge error, throwable in some search algorithms
     - @ref not_complete for exceptions indicating a non-complete graph error
     */
    struct bad_graph : public exception {
    protected:
        explicit bad_graph(const char *what_arg) : exception(what_arg) {}

        static std::string name(const std::string &exception_type) {
            return "[graph.exception.bad_graph." + exception_type + "] ";
        }
    };

    struct negative_edge final : public bad_graph {
        static negative_edge create(const std::string &function_name,
                                    const std::string &what_arg = "Edge with negative weight") {
            std::string w{bad_graph::name("negative_edge") + what_arg + " when calling '" + function_name + "'."};
            return negative_edge(w.c_str());
        }

    private:
        explicit negative_edge(const char *what_arg) : bad_graph(what_arg) {}
    };

    struct not_complete final : public bad_graph {
        static not_complete create(const std::string &function_name,
                                   const std::string &what_arg = "Not complete graph") {
            std::string w{bad_graph::name("not_complete") + what_arg + " when calling '" + function_name + "'."};
            return not_complete(w.c_str());
        }

    private:
        explicit not_complete(const char *what_arg) : bad_graph(what_arg) {}
    };

//!
//! @section degree
//!
    template <Nature N> class basic_degree;

    template <> class basic_degree<DIRECTED> {
    public:
        using value_type = std::pair<std::size_t, std::size_t>;

        basic_degree(const value_type& degree) : _deg(degree) {};
        basic_degree(std::size_t in, std::size_t out) : basic_degree(std::make_pair(in, out)) {};

        inline value_type get_degree() const { return _deg; }

        bool operator==(const basic_degree& d) const { return _deg == d._deg; }
        bool operator==(const value_type& v)   const { return _deg == v; }

        bool operator< (const basic_degree& d) const { return _deg < d._deg; }

        static basic_degree max() { return basic_degree(std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max()); }
        static basic_degree min() { return basic_degree(std::numeric_limits<std::size_t>::min(), std::numeric_limits<std::size_t>::min()); }

    private:
        value_type _deg;
    };

    template <> class basic_degree<UNDIRECTED> {
    public:
        using value_type = std::size_t;

        basic_degree(const value_type& d) : _deg(d) {}
        basic_degree(std::size_t in, std::size_t) : basic_degree(in) {}

        inline value_type get_degree() const { return _deg; }

        bool operator==(const basic_degree& d) const { return _deg == d._deg; }
        bool operator==(const value_type& v)   const { return _deg == v; }

        bool operator< (const basic_degree& d) const { return _deg < d._deg; }

        static basic_degree max() { return basic_degree(std::numeric_limits<std::size_t>::max()); }
        static basic_degree min() { return basic_degree(std::numeric_limits<std::size_t>::min()); }

    private:
        value_type _deg;
    };

//!
//! @section helpers
//!
#include <memory>
    //! distinguish value type between map::iterator and shared_ptr: @see https://stackoverflow.com/a/31409532
    template <typename... >
    using void_t = void;

    template <typename T, typename = void>
    struct is_map_iterator : std::false_type { };

    template <typename T>
    struct is_map_iterator<T,
            void_t<decltype(++std::declval<T&>()),                     /// incrementable,
                   decltype(*std::declval<T&>()),                      /// dereferencable,
                   decltype(std::declval<T&>() == std::declval<T&>()), /// comparable
                   decltype((*std::declval<T&>()).second)> >           /// pair
            : std::true_type { };

    /// pointer
    ///template <class V, class = typename std::enable_if<!is_map_iterator<V>::value>::type>
    ///inline V get_value(const V& v, const V&) { return v; }
    /// map iterator
    template <class V, class = typename std::enable_if<is_map_iterator<V>::value>::type>
    inline typename std::iterator_traits<V>::value_type::second_type get_value(const V &v, const V &end) {
        if (v == end)
            return static_cast<typename std::iterator_traits<V>::value_type::second_type>(nullptr);
        return (*v).second;
    }

#include <cxxabi.h> /// demangled_name, abi::__cxa_demangle

    //! @return the name of the @tparam T
    /// @example type_name(4.2f) == "float"
    template <class T>
    std::string type_name() {
        int status;
        std::string tname{typeid(T).name()};
        char *demangled_name{abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status)};

        if (status == 0)
            tname = demangled_name;
        std::free(demangled_name);

        /// Replace all occurences of 'to_replace' by 'replacement' in 'base'
        /// Usefull to get an human readable 'std::string'
        std::function<void(std::string&, std::string, std::string)> replace_all =
                [](std::string& base, std::string to_replace, std::string replacement) {
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
    std::istream& read_T(std::istream& is, T& t) {
        is.ignore(std::numeric_limits<std::streamsize>::max(), '"');

        std::string str;
        std::getline(is, str, '"');
        std::stringstream ss;
        ss << str;
        if (!(ss >> t))
            GRAPH_THROW_WITH(parse_error, 5, "Bad type");

        return is;
    }
    template<> inline
    std::istream& read_T<std::string>(std::istream& is, std::string& str) {
        is.ignore(std::numeric_limits<std::streamsize>::max(), '"');
        std::getline(is, str, '"');
        return is;
    }

    template <class C>
    std::istream& read_cost(std::istream& is, C& c) {
        std::string str = std::string(std::istreambuf_iterator<char>(is),
                                      std::istreambuf_iterator<char>());
        if (str.find_first_of("\"") == std::string::npos &&
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
