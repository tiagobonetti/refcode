#pragma once

#include "auto_token/token.hpp"

#include <functional>
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>

namespace autotoken {

template <typename... Args>
class subject : public removable {
   public:
    using handler_type = std::function<void(Args...)>;

    template <typename Handler>
    token observe(Handler&& handler);

    void remove_observer(std::size_t key) final { _observers.erase(key); }
    void notify(Args... args) const;

   private:
    std::unordered_map<token::key_type, handler_type> _observers;
};

template <typename... Args>
template <typename Handler>
token subject<Args...>::observe(Handler&& h) {
    handler_type handler{std::forward<Handler>(h)};
    if (!handler) {
        return {};
    }

    auto key = token::unique_key();

    typename decltype(_observers)::const_iterator it;
    bool inserted;
    std::tie(it, inserted) = _observers.emplace(key, std::move(handler));

    if (!inserted) {
        return {};
    }

    return token{*this, key};
}

template <typename... Args>
void subject<Args...>::notify(Args... args) const {
    for (auto const& entry : _observers) {
        entry.second(args...);
    }
}

}  // namespace autotoken
