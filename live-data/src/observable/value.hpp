#pragma once

#include "token.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>

namespace observable {

class removable {
   public:
    virtual bool remove_observer(token t) = 0;
};

template <typename T>
class value : public removable {
   public:
    using handler_type = std::function<void(T const&)>;

    template <typename Value>
    value(Value&& v) : _value(std::forward<Value>(v)) {}

    value(value const& other) = delete;
    value& operator=(value const& other) = delete;

    value(value&& other) = default;
    value& operator=(value&& other) = default;

    T const& get_value() { return _value; }

    template <typename Value>
    void set_value(Value&& v);

    template <typename Handler>
    token observe(Handler&& handler);

    bool remove_observer(token t) { return _observers.erase(t) > 0; }

   private:
    void notify() const;

    T _value;
    std::unordered_map<token, handler_type> _observers;
};

template <typename T>
template <typename Value>
void value<T>::set_value(Value&& v) {
    T new_value = std::forward<T>(v);

    if (_value != new_value) {
        _value = new_value;
        notify();
    }
}

template <typename T>
template <typename Handler>
token value<T>::observe(Handler&& handler) {
    if (!handler) {
        return {};
    }

    typename decltype(_observers)::const_iterator it;
    bool inserted;
    std::tie(it, inserted) = _observers.emplace(
        token::unique(), std::forward<Handler>(handler));

    if (!inserted) {
        return {};
    }

    return it->first;
}

template <typename T>
void value<T>::notify() const {
    for (auto& entry : _observers) {
        entry.second(_value);
    }
}

}  // namespace observable
