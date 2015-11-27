#ifndef CLUE_MISC__
#define CLUE_MISC__

// Miscellaneous utilities

#include <clue/common.hpp>
#include <memory>
#include <sstream>

namespace clue {

struct place_holder_t {};
constexpr place_holder_t _{};

template<typename... Args>
inline void pass(Args&&... args) {}

template<class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


namespace details {

template<class A>
inline void insert_to_stream(std::ostream& os, A&& x) {
    os << x;
}

template<class A, class... Rest>
inline void insert_to_stream(std::ostream& os, A&& x, Rest&&... rest) {
    os << x;
    insert_to_stream(os, std::forward<Rest>(rest)...);
}

} // end namespace details

template<class... Args>
inline std::string sstr(Args&&... args) {
    std::stringstream ss;
    details::insert_to_stream(ss, std::forward<Args>(args)...);
    return ss.str();
}

inline std::string sstr() {
    return std::string();
}

template<class Seq>
struct Delimits {
    const Seq& seq;
    const char *delimiter;

    Delimits(const Seq& s, const char *delim)
        : seq(s), delimiter(delim) {}
};

template<class Seq>
inline Delimits<Seq> delimits(const Seq& seq, const char *delim) {
    return Delimits<Seq>(seq, delim);
}

template<class Seq>
inline std::ostream& operator << (std::ostream& out, const Delimits<Seq>& a) {
    auto it = a.seq.begin();
    auto it_end = a.seq.end();
    if (it != it_end) {
        out << *it;
        ++it;
        for(;it != it_end; ++it)
            out << a.delimiter << *it;
    }
    return out;
}

}

#endif