#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace unique {

template <typename... Args>
class subject2 {
   public:
    using handler_type = std::function<void(Args...)>;
    using deleter_type = std::function<void(handler_type*)>;
    using observer_type = std::unique_ptr<handler_type, deleter_type>;

    template <typename Handler>
    observer_type observe(Handler&&);

    void notify(Args...);

   private:
    void remove(handler_type*);

    std::vector<handler_type*> _observers;
};

template <typename... Args>
template <typename Handler>
auto subject2<Args...>::observe(Handler&& h) -> subject2<Args...>::observer_type {
    // store handler in a unique_ptr
    observer_type observer(new handler_type(std::forward<Handler>(h)),
                           [this](handler_type* h) {
                               remove(h);
                               delete h;
                           });
    if (!(*observer)) {
        return nullptr;
    }

    auto item = observer.get();
    _observers.emplace_back(item);

    return observer;
}

template <typename... Args>
void subject2<Args...>::notify(Args... args) {
    for (auto& observer : _observers) {
        (*observer)(args...);
    }
}

template <typename... Args>
void subject2<Args...>::remove(handler_type* h) {
    auto it = std::find(_observers.begin(), _observers.end(), h);
    _observers.erase(it);
}

}  // namespace unique
