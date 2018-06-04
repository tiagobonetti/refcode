#pragma once

#include "observable/removable.hpp"
#include "observable/token.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>

namespace observable {

template <typename T>
class value : public removable {
   public:
    using handler_type = std::function<void(T const&)>;

    template <typename Value>
    value(Value&& v) : _value(std::forward<Value>(v)) {}

    value(value const& other) = delete;
    value& operator=(value const& other) = delete;

    value(value&& other) = delete;
    value& operator=(value&& other) = delete;

    T const& get_value() { return _value; }

    template <typename Value>
    void set_value(Value&& v);

    template <typename Handler>
    token observe(Handler&& handler);

   private:
    bool remove_observer(std::size_t key) { return _observers.erase(key) > 0; }
    void notify() const;

    T _value;
    std::unordered_map<token::key_type, handler_type> _observers;
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

    auto key = token::unique_key();

    typename decltype(_observers)::const_iterator it;
    bool inserted;
    std::tie(it, inserted) = _observers.emplace(key, std::forward<Handler>(handler));

    if (!inserted) {
        return {};
    }

    return token{*this, key};
}

template <typename T>
void value<T>::notify() const {
    for (auto& entry : _observers) {
        entry.second(_value);
    }
}

}  // namespace observable
