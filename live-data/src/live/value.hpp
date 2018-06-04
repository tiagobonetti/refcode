#pragma once

#include <cstddef>
#include <sstream>

namespace live {

template <typename T>
class value {
   public:
    value(T&& v) : _value(std::forward<T>(v)) {}

    T const& get_value() {
        return _value;
    }

    value<T>& set_value(T&& v) {
        _value = std::forward<T>(v);
        return *this;
    }

   private:
    T _value;
};

}  // namespace live
