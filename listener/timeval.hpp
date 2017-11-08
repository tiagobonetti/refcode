#ifndef TIMEVAL_HPP_
#define TIMEVAL_HPP_
// C++
#include <ios>
#include <ostream>
#include <iomanip>
// Sys
#include <sys/time.h>

typedef struct timeval Timeval;

inline Timeval operator+(const Timeval& lhs, const Timeval& rhs) {
    return (Timeval{lhs.tv_sec + rhs.tv_sec, lhs.tv_usec + rhs.tv_usec});
}

inline Timeval operator-(const Timeval& lhs, const Timeval& rhs) {
    return (lhs.tv_usec >= rhs.tv_usec)
               ? Timeval{lhs.tv_sec - rhs.tv_sec, lhs.tv_usec - rhs.tv_usec}
               : Timeval{lhs.tv_sec - rhs.tv_sec - 1,
                         1000000 + lhs.tv_usec - rhs.tv_usec};
}

inline bool operator==(const Timeval& lhs, const Timeval& rhs) {
    return (lhs.tv_sec == rhs.tv_sec && lhs.tv_usec == rhs.tv_usec);
}

inline bool operator!=(const Timeval& lhs, const Timeval& rhs) {
    return (lhs.tv_sec != rhs.tv_sec || lhs.tv_usec != rhs.tv_usec);
}

inline bool operator>(const Timeval& lhs, const Timeval& rhs) {
    return (lhs.tv_sec > rhs.tv_sec ||
            (lhs.tv_sec == rhs.tv_sec && lhs.tv_usec > rhs.tv_usec));
}

inline bool operator>=(const Timeval& lhs, const Timeval& rhs) {
    return (lhs.tv_sec > rhs.tv_sec ||
            (lhs.tv_sec == rhs.tv_sec && lhs.tv_usec >= rhs.tv_usec));
}

inline bool operator<(const Timeval& lhs, const Timeval& rhs) {
    return (lhs.tv_sec < rhs.tv_sec ||
            (lhs.tv_sec == rhs.tv_sec && lhs.tv_usec < rhs.tv_usec));
}

inline bool operator<=(const Timeval& lhs, const Timeval& rhs) {
    return (lhs.tv_sec < rhs.tv_sec ||
            (lhs.tv_sec == rhs.tv_sec && lhs.tv_usec <= rhs.tv_usec));
}

inline std::ostream& operator<<(std::ostream& os, const Timeval& t) {
    std::ios fmt(nullptr);
    fmt.copyfmt(os);
    os << t.tv_sec << "." << std::setfill('0') << std::setw(6) << t.tv_usec;
    os.copyfmt(fmt);
    return os;
}

#endif  // TIMEVAL_HPP_
