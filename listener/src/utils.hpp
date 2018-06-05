#pragma once
// Libraries
#include <boost/make_unique.hpp>

#if __cplusplus < 201402L  // less then C++14
namespace std {
using boost::make_unique;

template <typename T>
constexpr auto cbegin(const T& t) -> decltype(t.cbegin()) { return t.cbegin(); }

template <typename T>
constexpr auto cend(const T& t) -> decltype(t.cend()) { return t.cend(); }
}  // namespace std
#endif
