#pragma once

#include <cstddef>
#include <sstream>

template <typename T>
class live_data {
   public:
    live_data(T&& v) : value(_value), _value(std::forward<T>(v)) {}

    live_data<T>& set(T&& v) {
        _value = std::forward<T>(v);
        return *this;
    }

    T const& value;

   private:
    T _value;
};
