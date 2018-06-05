#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace live {

template <typename T>
class data {
   public:
    using handler_type = std::function<void(T const&)>;
    using observer = std::shared_ptr<handler_type>;

    template <typename Value>
    data(Value&& v);

    template <typename Handler>
    observer observe(Handler&& handler);

    template <typename Value>
    void set_value(Value&& v);

    T const& get_value() const { return _value; }

   private:
    void notify();
    void erase_expired();

    T _value;
    std::vector<std::weak_ptr<handler_type>> _observers;
};

template <typename T>
template <typename Value>
data<T>::data(Value&& v) : _value(std::forward<Value>(v)) {}

template <typename T>
template <typename Value>
void data<T>::set_value(Value&& v) {
    if (_value == v) {
        return;
    }

    _value = std::forward<T>(v);
    notify();
}

template <typename T>
template <typename Handler>
auto data<T>::observe(Handler&& h) -> data<T>::observer {
    if (!h) {
        return nullptr;
    }

    auto observer = std::make_shared<handler_type>(std::forward<Handler>(h));
    _observers.emplace_back(std::weak_ptr<handler_type>(observer));
    return observer;
}

template <typename T>
void data<T>::notify() {
    bool missed = false;

    for (auto& weak : _observers) {
        auto observer = weak.lock();
        if (observer) {
            (*observer)(_value);
        } else {
            missed = true;
        }
    }

    if (missed) {
        erase_expired();
    }
}

template <typename T>
void data<T>::erase_expired() {
    _observers.erase(
        std::remove_if(_observers.begin(), _observers.end(),
                       [](std::weak_ptr<handler_type> weak) { return weak.expired(); }));
}

}  // namespace live
